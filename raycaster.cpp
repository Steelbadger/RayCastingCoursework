#include "raycaster.h"
#include "ps2maths.h"
#include <iostream>


const int RayCaster::MAXRANGE = 20;
const float RayCaster::MAXRANGESQR = 400.0f;

RayCaster::RayCaster()
{
}


RayCaster::~RayCaster()
{
}


RayReturnData RayCaster::CastRay(Vector2 base, Vector2 direction, Level& level)
{
	RayReturnData firstHorizCollision = CheckHorizontalDivisions(base, direction, level);
	RayReturnData firstVerticalCollision = CheckVerticalDivisions(base, direction, level);

	RayReturnData output = Min(firstHorizCollision, firstVerticalCollision);

//	RayReturnData output = CheckInterleavedDivisions(base, direction, level);
	
	return output;
}

RayReturnData RayCaster::AltCastRay(Vector2 base, Vector2 direction, Level& level)
{
	RayReturnData output = CheckInterleavedDivisions(base, direction, level);
	return output;
}

RayReturnData RayCaster::CheckInterleavedDivisions(Vector2 base, Vector2 direction, Level& level)
{
	bool checkHoriz = true;
	bool checkVerti = true;
	//  Catch case where ray is aligned to the x-axis and return very large distance and default wall
	if (Abs(direction.y) < 0.001) {
		checkHoriz = false;
//		std::cout << "Check Horiz Set To False" << std::endl;
	}
	if (Abs(direction.x) < 0.001) {
		checkVerti = false;
//		std::cout << "Check Verti Set To False" << std::endl;
	}	
	
	//  convert the y-direction (when level viewed in PLAN) into -1/+1 for up/down direction respectively
	int verticalDirection = int(direction.y/Abs(direction.y));	
	int horizontalDirection = int(direction.x/Abs(direction.x));
//	std::cout << "Vdir: " << verticalDirection << "\tHdir: " << horizontalDirection << std::endl;
	
	
	//  Offset for stepping along grid
	int offset = 1;
	
	//  Gradient for equation in x dependent on y
	float grad = direction.y/direction.x;
	
	//  next place ray intersects a grid line
	int nextYDivision;
	int nextXDivision;
	//  The corresponding x value of that grid square
	int matchingXDivision;
	int matchingYDivision;
	
	//  The actual position of the ray collision point
	float xPoI;
	float yPoI;
	
	//  The type of thing in the grid square hit;
	unsigned int hitType;
	RayReturnData firstHorizontalObject = RayReturnData(99999.99f, 1);
	RayReturnData firstVerticalObject = RayReturnData(99999.99f, 1);
	Vector2 pointOfIntersect;
	
	while(checkHoriz || checkVerti) {
//		std::cout << "Loop: " << offset << std::endl;
		if (checkHoriz == true) {
			//  If we're NOT already on a boundary, 
			if (Mod(base.y + offset*verticalDirection, 1.0f) != 0) {
				//  the next division is at floor(y) - 1 if direction is UPWARDS
				//							floor(y) + 1 if direction is DOWNWARDS
				nextYDivision = int(base.y) + offset * verticalDirection;
			//  If we ARE already on a boundary
			} else {
				//  the next division is at floor(y) - 2 if direction is UPWARDS
				//							floor(y) + 0 if direction is DOWNWARDS
				nextYDivision = int(base.y) + offset * (verticalDirection - 1);
			}

			//  Calculate Point of Intersection in x and y coords
			//  yPosition should be the TOP of the target cell when heading down
			//  this means yPoI = nextYDivision (nextY - 0.5 * (1-1))
			//  yPosition should be the BOTTOM of the target cell when heading up
			//  this means yPoI = nextYDivision + 1 (nextY - 0.5 * (-1-1)) = (nextY - 0.5 * -2) = (nextY + 1)
			yPoI = nextYDivision - (0.5 * (verticalDirection - 1));
			
			//  Sub this into the equation of the line to find the x coordinate of the intersection
			float c = base.y - grad * base.x;	
			xPoI = (yPoI - c)/grad;
			
			pointOfIntersect = Vector2(xPoI, yPoI);
			
			if ( Vector2(pointOfIntersect-base).LengthSqr() > MAXRANGESQR) {
				firstHorizontalObject = RayReturnData(99999.99f, 1);
				checkHoriz = false;
//				std::cout << "Out of Range: Check Horiz Set To False" << std::endl;					
			}			
			
			//  Floor the x intersect point to find grid coordinate
			matchingXDivision = int(xPoI);
			
			//  What's in the grid we've colliding with
			//  Level::At returns a wall if the point is outside of array bounds
			hitType = level.At(matchingXDivision, nextYDivision);
			
			//  0 is empty space, if it is anything other than zero then it's a kind of wall,
			//  so return the wall and distance to wall
			if ( hitType > 0 ) {
				firstHorizontalObject = RayReturnData(Vector2(pointOfIntersect-base).Length(), (0.5 * (verticalDirection + 1)) - Mod(xPoI,1) * verticalDirection  , hitType);
				checkHoriz = false;	
//				std::cout << "Found Wall: Check Horiz Set To False" << std::endl;					
			}
		}
		
		if (checkVerti == true) {
			//  If we're NOT already on a boundary, 
			if (Mod(base.x + offset*horizontalDirection, 1.0f) != 0) {
				//  the next division is at floor(y) - 1 if direction is UPWARDS
				//							floor(y) + 1 if direction is DOWNWARDS
				nextXDivision = int(base.x) + offset * horizontalDirection;
			//  If we ARE already on a boundary
			} else {
				//  the next division is at floor(y) - 2 if direction is UPWARDS
				//							floor(y) + 0 if direction is DOWNWARDS
				nextXDivision = int(base.x) + offset * (horizontalDirection - 1);
			}

			//  Calculate Point of Intersection in x and y coords
			//  yPosition should be the TOP of the target cell when heading down
			//  this means yPoI = nextYDivision (nextY - 0.5 * (1-1))
			//  yPosition should be the BOTTOM of the target cell when heading up
			//  this means yPoI = nextYDivision + 1 (nextY - 0.5 * (-1-1)) = (nextY - 0.5 * -2) = (nextY + 1)
			xPoI = nextXDivision - (0.5 * (horizontalDirection - 1));
			
			//  Sub this into the equation of the line to find the x coordinate of the intersection		
			float c = base.y - grad * base.x;
			
			yPoI = c + grad * xPoI;
			
			pointOfIntersect = Vector2(xPoI, yPoI);
			
			if ( Vector2(pointOfIntersect-base).LengthSqr() > MAXRANGESQR) {
				firstVerticalObject = RayReturnData(99999.99f, 1);
				checkVerti = false;
//				std::cout << "Out of Range: Check Verti Set To False" << std::endl;				
			}			
			
			//  Floor the x intersect point to find grid coordinate
			matchingYDivision = int(yPoI);

			hitType = level.At(nextXDivision, matchingYDivision);

			//  0 is empty space, if it is anything other than zero then it's a kind of wall,
			//  so return the wall and distance to wall		
			if ( hitType > 0 ) {
				firstVerticalObject = RayReturnData(Vector2(pointOfIntersect-base).Length(), (0.5 * (-horizontalDirection + 1)) + Mod(yPoI,1)*horizontalDirection , hitType);
				checkVerti = false;		
//				std::cout << "Found Wall: Check Verti Set To False" << std::endl;					
			}	
		}
		
		//  If we didn't return then increment and try again
		offset++;
	}
	
//	std::cout << "Done Loop" << std::endl;

	return Min(firstHorizontalObject, firstVerticalObject);


}

RayReturnData RayCaster::CheckHorizontalDivisions(Vector2 base, Vector2 direction, Level& level)
{
	//  Catch case where ray is aligned to the x-axis and return very large distance and default wall
	if (Abs(direction.y) < 0.001) {
		return RayReturnData(99999.99f, 1);
	}
	
	//  convert the y-direction (when level viewed in PLAN) into -1/+1 for up/down direction respectively
	int verticalDirection = int(direction.y/Abs(direction.y));	
	
	//  Offset for stepping along grid
	int offset = 1;
	
	//  Gradient for equation in x dependent on y
	float grad = direction.y/direction.x;
	
	//  next place ray intersects a grid line
	int nextYDivision;
	//  The corresponding x value of that grid square
	int matchingXDivision;
	
	//  The actual position of the ray collision point
	float xPoI;
	float yPoI;
	
	//  The type of thing in the grid square hit;
	unsigned int hitType;
	
	for(int i = 0; i < MAXRANGE; i++) {
		//  If we're NOT already on a boundary, 
		if (Mod(base.y + offset*verticalDirection, 1.0f) != 0) {
			//  the next division is at floor(y) - 1 if direction is UPWARDS
			//							floor(y) + 1 if direction is DOWNWARDS
			nextYDivision = int(base.y) + offset * verticalDirection;
		//  If we ARE already on a boundary
		} else {
			//  the next division is at floor(y) - 2 if direction is UPWARDS
			//							floor(y) + 0 if direction is DOWNWARDS
			nextYDivision = int(base.y) + offset * (verticalDirection - 1);
		}

		//  Calculate Point of Intersection in x and y coords
		//  yPosition should be the TOP of the target cell when heading down
		//  this means yPoI = nextYDivision (nextY - 0.5 * (1-1))
		//  yPosition should be the BOTTOM of the target cell when heading up
		//  this means yPoI = nextYDivision + 1 (nextY - 0.5 * (-1-1)) = (nextY - 0.5 * -2) = (nextY + 1)
		yPoI = nextYDivision - (0.5 * (verticalDirection - 1));
		
		//  Sub this into the equation of the line to find the x coordinate of the intersection
		float c = base.y - grad * base.x;	
		xPoI = (yPoI - c)/grad;
		
		Vector2 pointOfIntersect(xPoI, yPoI);
		
		//  Floor the x intersect point to find grid coordinate
		matchingXDivision = int(xPoI);
		
		//  What's in the grid we've colliding with
		//  Level::At returns a wall if the point is outside of array bounds
		hitType = level.At(matchingXDivision, nextYDivision);
		
		//  0 is empty space, if it is anything other than zero then it's a kind of wall,
		//  so return the wall and distance to wall
		if ( hitType > 0 ) {
			return RayReturnData(Vector2(pointOfIntersect-base).Length(), (0.5 * (verticalDirection + 1)) - Mod(xPoI,1) * verticalDirection  , hitType);
//			return RayReturnData(Vector2(pointOfIntersect-base).Length(), Mod(xPoI,1), hitType);
		}
		
		//  If we didn't return then increment and try again
		offset++;
	}
	//  Should never happen
	return RayReturnData(99999.99f, 1);
}

RayReturnData RayCaster::CheckVerticalDivisions(Vector2 base, Vector2 direction, Level& level)
{
	//  Catch case where ray is aligned to the y-axis and return very large distance and default wall
	if (Abs(direction.x) < 0.001) {
		return RayReturnData(99999.99f, 1);
	}
	
	//  convert the x-direction (when level viewed in PLAN) into -1/+1 for left/right direction respectively	
	int horizontalDirection = int(direction.x/Abs(direction.x));
	int verticalDirection = int(direction.y/Abs(direction.y));	
	
	//  Offset for stepping along grid
	int offset = 1;
	
	//  Gradient for equation in y dependent on x
	float grad = direction.y/direction.x;
	
	//  next place ray intersects a grid line
	int nextXDivision;
	//  The corresponding y value of that grid square
	int matchingYDivision;
	
	//  The actual position of the ray collision point
	float xPoI;
	float yPoI;
	
	//  The type of thing in the grid square hit;
	unsigned int hitType;
	
	for(int i = 0; i < MAXRANGE; i++) {
		//  If we're NOT already on a boundary, 
		if (Mod(base.x + offset*horizontalDirection, 1.0f) != 0) {
			//  the next division is at floor(y) - 1 if direction is UPWARDS
			//							floor(y) + 1 if direction is DOWNWARDS
			nextXDivision = int(base.x) + offset * horizontalDirection;
		//  If we ARE already on a boundary
		} else {
			//  the next division is at floor(y) - 2 if direction is UPWARDS
			//							floor(y) + 0 if direction is DOWNWARDS
			nextXDivision = int(base.x) + offset * (horizontalDirection - 1);
		}

		//  Calculate Point of Intersection in x and y coords
		//  yPosition should be the TOP of the target cell when heading down
		//  this means yPoI = nextYDivision (nextY - 0.5 * (1-1))
		//  yPosition should be the BOTTOM of the target cell when heading up
		//  this means yPoI = nextYDivision + 1 (nextY - 0.5 * (-1-1)) = (nextY - 0.5 * -2) = (nextY + 1)
		xPoI = nextXDivision - (0.5 * (horizontalDirection - 1));
		
		//  Sub this into the equation of the line to find the x coordinate of the intersection		
		float c = base.y - grad * base.x;
		
		yPoI = c + grad * xPoI;
		
		Vector2 pointOfIntersect(xPoI, yPoI);
		
		//  Floor the x intersect point to find grid coordinate
		matchingYDivision = int(yPoI);

		hitType = level.At(nextXDivision, matchingYDivision);

		//  0 is empty space, if it is anything other than zero then it's a kind of wall,
		//  so return the wall and distance to wall		
		if ( hitType > 0 ) {
			return RayReturnData(Vector2(pointOfIntersect-base).Length(), (0.5 * (-horizontalDirection + 1)) + Mod(yPoI,1)*horizontalDirection , hitType);
//			return RayReturnData(Vector2(pointOfIntersect-base).Length(), Mod(yPoI,1), hitType);			
		}
		//  If we didn't return then increment and try again		
		offset++;
	}
	//  Should never happen
	return RayReturnData(99999.99f, 1);
}
