#include "mobmanager.h"
#include <iostream>
#include "texturemanager.h"

//  Nothing to do here
MobManager::MobManager()
{
}

MobManager::~MobManager()
{
	//Nuffin
}

void MobManager::DrawMobs()
//  Loop through mobs and draw them if they're within the FoV
{
	if (!mobs.empty()) {
		std::list<MobData>::iterator i;
		for(i = mobs.begin(); i != mobs.end(); i++) {
			if (i->onscreen == true) {
				i->mob.Render();
			}
		}
	}
}

void MobManager::FindMobPositions(Vector2 playerPosition, Vector2 playerDirection, float FoV)
//  Loop through mob list and find where the mobs should be drawn on the screen, and at what size
{
	//  If we have some mobs
	if (!mobs.empty()) {
		std::list<MobData>::iterator i;
		
		//  use radians
		float fov = DegToRad(FoV);
		//  Make a wider FoV so mobs don't pop out of existance on the left side of the screen
		float wideFov = DegToRad(FoV+10);
		
		//  Safeguard, normalise the direction vector
		Vector2 b = playerDirection.Normalise();
		
		//  Find the direction vector of the leftmost edge of the draw area
		Vector2 c = b;
		b.Rotate(-wideFov/2);
			
		//  Loop through all mobs
		for(i = mobs.begin(); i != mobs.end(); i++) {
			//  Not onscreen as default
			i->onscreen = false;
			//  Tell the mob to target the player
			i->mob.SetTarget(playerPosition);

			//  Find vector from mob to player
			Vector2 a = (i->mob.GetPosition()-playerPosition);
			float dist = a.LengthSqr();
			
			//  Don't bother doing more difficult calcs if out of draw range
			if (dist < 400 && dist > 0.05f) {
				//  We're in draw range so find the actual distance
				dist = Sqrt(dist);
				//  Then normalise the direction vector to the mob
				a = a/dist;
				
				//  Find the angle from the centre of the screen
				float angle = ACos(c.Dot(a));
				
				//  If this angle is less than half the FoV then the mob is inside the viewable area
				if (angle < wideFov/2) {
					
					//  Now find the angle between the player-mob vector and the left-most vector
					angle = ACos(b.Dot(a));
					//  This allows us to interpolate a distance across the screen based on the angle
					
					//  Move the sprite to the correct position for the location of the mob
					//					leftmost extent of wide fov						rightmost extent of widefov				work out how height the centre must be
					i->mob.MoveSpriteTo(-(373.333*(FoV+10)/(FoV+10))+(angle/wideFov)*(746.666*(FoV+10)/(FoV+10)), (1.0f - (i->mob.GetHeight())/2.0f) * (512.0f/dist));
					//  Now scale the sprite down to the correct size
					i->mob.ScaleSprite(1.0/dist);
					
					//  Set the sprite's z-depth
					i->mob.SetDistance(800-(dist*4));
					
					//  It's on the screen!
					i->onscreen = true;			
				}
			}
		}
		//  Sort the mobs into order by depth for later drawing
		mobs.sort(MobManager::MobComparator);
	}
}

void MobManager::DEBUGFindMobPositions(Vector2 playerPosition, Vector2 playerDirection, float FoV)
//  Same function as above, but with loads of console output for debug
{
	if (!mobs.empty()) {
		std::list<MobData>::iterator i;
		float fov = DegToRad(FoV);
		float wideFov = DegToRad(FoV+20);
		Vector2 b = playerDirection.Normalise();
		Vector2 c = b;
		b.Rotate(-wideFov/2);
		std::cout << "Find Mob Positions" << std::endl;
		std::cout << "Player At: " << playerPosition << "\t Look Direction: " << b << std::endl;
		std::cout << "Field Of View: " << FoV << std::endl;
		
		for(i = mobs.begin(); i != mobs.end(); i++) {
			i->onscreen = false;
			Vector2 a = (i->mob.GetPosition()-playerPosition);
			float dist = a.LengthSqr();
			std::cout << "Distance Squared: " << dist << std::endl;
			
			if (dist < 400 && dist > 0.05f) {		//  Don't bother doing more difficult calcs if out of draw range
				std::cout << "Within Draw Range" << std::endl;
				dist = Sqrt(dist);
				a = a/dist;
				std::cout << "Distance: " << dist << "\tUnit Vector Direction: " << a << std::endl;
				float correcting = c.Dot(a);
				float angle = ACos(correcting);
				std::cout << "Angle From Centreline: " << angle*360/TWOPI << std::endl;
				
				if (angle < wideFov/2) {
					std::cout << "Within FoV" << std::endl;
					angle = ACos(b.Dot(a));
					std::cout << "Angle From Left Edge of Screen: " << angle*360/TWOPI << std::endl;
					i->mob.MoveSpriteTo(-(373.333*(FoV+10)/(FoV+10))+(angle/wideFov)*(746.666*(FoV+10)/(FoV+10)), (1.0f - (i->mob.GetHeight())/2.0f) * (512.0f/dist));
					i->mob.ScaleSprite(1.0/dist);
					i->mob.SetDistance(800-dist);
					
					std::cout << "Scaling Factor: " << 800/dist << std::endl;
					std::cout << "Position on Screen: (" << -320+(angle/fov)*640 << ", " << 0 << ")" << std::endl;
					
					i->onscreen = true;			
				}
			}
		}
		
		mobs.sort(MobManager::MobComparator);
	}
	std::cout << std::endl;
}


void MobManager::AddMob(Mob m)
//  DEPRECATED Add a single mob to the list
{
	if(mobs.size() < MAXMOBS) {
		MobData t;
		t.mob = m;
		t.onscreen = false;
		t.dead = false;
		mobs.push_back(t);
	}
}

void MobManager::LoadMobList(std::vector<Mob> m) 
//  Load a vector of mobs into the mob list
{
	std::vector<Mob>::iterator inputIterator;
	mobs.clear();
	
	for (inputIterator = m.begin(); inputIterator != m.end(); inputIterator++) {
		MobData temp;
		temp.mob = *inputIterator;
		temp.onscreen = false;
		temp.dead = false;
		mobs.push_back(temp);
	}
	
	//  Load the necessary textures in
	TexManager.LoadTexture("mob1.bmp");
	TexManager.LoadTexture("mob2.bmp");	
}


void MobManager::UpdateMobs(Level& level, double timeDif)
//  Loop through mob list and call update functions
{
	if (!mobs.empty()) {
		std::list<MobData>::iterator i;
		for(i = mobs.begin(); i != mobs.end(); i++) {
			i->mob.Update(level, timeDif);
		}	
	}
}

void MobManager::ShootMobs(float distanceToNearestWall, int damage)
//  Loop through mob list, check to see if any is in the centre of the screen (where bullets go)
{
	if (!mobs.empty()) {
		std::list<MobData>::iterator i;
		//  Iterate from end to beginning (closest to furthest)
		for(i = mobs.end(); i != mobs.begin();) {
			i--;
			//  If the mob is closer than the nearest wall and has 0,0 inside the sprite boundaries and is not dead
			if (((800-(i->mob.GetDistance()))/4 < distanceToNearestWall) && (i->mob.IsInside(Vector2(0.0f, 0.0f))) && !(i->mob.IsDead())) {
				//  Do damage
				i->mob.Damage(damage);
				//  Then stop looking
				break;
			}
		}
		
		//  Now look through the list for any who have died
		for(i = mobs.begin(); i != mobs.end(); i++) {
			if (i->mob.IsDead() && !i->dead) {
				i->dead = true;
				//  And choose the proper death animation
				if (i->mob.GetHealth() < -15) {
					i->mob.CatastrophicKill();
				} else {
					i->mob.Kill();
				}
			}
		}
	}
}

void MobManager::ClearMobs()
//  Empty the mob list
{
	mobs.clear();
}


bool MobManager::MobComparator(MobData lhs, MobData rhs)
//  Order mobs so that the first in the list is furthest away
{
		return rhs.mob.GetDistance() > lhs.mob.GetDistance();
}

int MobManager::CheckMobDealtDamage()
//  Check through all mobs, add up the damage they have dealt then
//  pass this out
{
	int output = 0;
	if (!mobs.empty()) {
		std::list<MobData>::iterator i;
		for (i = mobs.begin(); i != mobs.end(); i++) {
			output += i->mob.GetDamageDealt();
		}
	}
	return output;
}

void MobManager::SetMobHitpoints(int hp)
//  Set all mobs max hp to new value (when changing difficulty)
{
	if (!mobs.empty()) {
		std::list<MobData>::iterator i;
		for(i = mobs.begin(); i != mobs.end(); i++) {
			i->mob.SetHitpoints(hp);
		}	
	}
}

void MobManager::SetMobDamage(int damage)
//  Set all mobs damage to new value (when changing difficulty)
{
	if (!mobs.empty()) {
		std::list<MobData>::iterator i;
		for(i = mobs.begin(); i != mobs.end(); i++) {
			i->mob.SetDamage(damage);
		}	
	}
}

void MobManager::SetMobSpeed(float speed)
//  Set all mobs base speed to new value (when changing difficulty)
{
	if (!mobs.empty()) {
		std::list<MobData>::iterator i;
		for(i = mobs.begin(); i != mobs.end(); i++) {
			i->mob.SetSpeed(speed);
		}	
	}
}