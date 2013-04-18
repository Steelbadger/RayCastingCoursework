#ifndef __RAYCASTER_H__
#define __RAYCASTER_H__
//////////////////////--By Ross Davies--//////////////////////
/*		This class casts a SINGLE ray per function call
		from the position specified in the direction
		specified
		Also includes the struct definition for the object
		returned by the functions, and a few logical
		operators for use with that data					*/
//////////////////////////////////////////////////////////////
#include "vector2.h"
#include "level.h"

struct RayReturnData
{
	//  Constructors
	RayReturnData(float r, float UVBase, unsigned int texID):range(r), v(UVBase), TextureID(texID){};
	RayReturnData(float r, unsigned int texID):range(r), v(0), TextureID(texID){};	
	RayReturnData():range(999999.99f), v(0), TextureID(0){};
	
	//  The length of the ray segment (to first collision)
	float range;
	
	//  The UV (v-axis) of the point of intersection with the object
	//  that the ray collided with
	float v;
	
	//  The texture used (as specified by the level) by the object
	//  that the ray collided with
	unsigned int TextureID;
};


class RayCaster
{
public:
	//  Constructor
	RayCaster();
	
	//  Destructor
	~RayCaster();
		
	//  Cast a single Ray from base in direction on level
	RayReturnData CastRay(Vector2 base, Vector2 direction, Level& level);
	
	//
	RayReturnData AltCastRay(Vector2 base, Vector2 direction, Level& level);
	
private:

	//  SubRay functions
	//  Check ray intersection with 'horizontal' grid divisions (parallel to x)
	RayReturnData CheckHorizontalDivisions(Vector2 base, Vector2 direction, Level& level);
	//  Check ray intersection with 'vertical' grid divisions (parallel to y)
	RayReturnData CheckVerticalDivisions(Vector2 base, Vector2 direction, Level& level);
	
	//
	RayReturnData CheckInterleavedDivisions(Vector2 base, Vector2 direction, Level& level);
	
	//  The maximum range a ray can travel
	static const int MAXRANGE;
	
	//  the max range squared for speed of comparison
	static const float MAXRANGESQR;
	
};


//  Logical operators for comparison of the return values of the various ray functions
inline bool operator > (	const RayReturnData &r1,
									const RayReturnData &r2)
{
	return (r1.range > r2.range);
}

inline bool operator < (	const RayReturnData &r1,
									const RayReturnData &r2)
{
	return (r1.range < r2.range);
}

inline bool operator <= (	const RayReturnData &r1,
									const RayReturnData &r2)
{
	return r1.range <= r2.range;
}

inline bool operator >= (	const RayReturnData &r1,
									const RayReturnData &r2)
{
	return r1.range >= r2.range;
}

inline bool operator == (	const RayReturnData &r1,
									const RayReturnData &r2)
{
	return r1.range == r2.range;
}

inline RayReturnData Min (	const RayReturnData &r1,
							const RayReturnData &r2)
{
	return ((r1 < r2)? r1 : r2);
}

inline RayReturnData Max (	const RayReturnData &r1,
							const RayReturnData &r2)
{
	return ((r1 > r2)? r1 : r2);
}


#endif