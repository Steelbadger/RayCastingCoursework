#include "mobmanager.h"
#include <iostream>
#include "texturemanager.h"

MobManager::MobManager()
{
}

MobManager::~MobManager()
{}


void MobManager::DrawMobs()
{
	if (!mobs.empty()) {
		std::list<MobData>::iterator i;
		for(i = mobs.begin(); i != mobs.end(); i++) {
			if (i->onscreen == true) {
				i->mob.Render();
//				i->onscreen = false;
			}
		}
	}
}

void MobManager::FindMobPositions(Vector2 playerPosition, Vector2 playerDirection, float FoV)
{
	if (!mobs.empty()) {
		std::list<MobData>::iterator i;
		float fov = DegToRad(FoV);
		float wideFov = DegToRad(FoV+10);
		Vector2 b = playerDirection.Normalise();
		Vector2 c = b;
		b.Rotate(-wideFov/2);
		
		for(i = mobs.begin(); i != mobs.end(); i++) {
			i->onscreen = false;
			i->mob.SetTarget(playerPosition);			
			Vector2 a = (i->mob.GetPosition()-playerPosition);
			float dist = a.LengthSqr();
			
			if (dist < 400 && dist > 0.05f) {		//  Don't bother doing more difficult calcs if out of draw range
				dist = Sqrt(dist);
				a = a/dist;
				
				float angle = ACos(c.Dot(a));
				
				if (angle < wideFov/2) {
					angle = ACos(b.Dot(a));
					i->mob.MoveSpriteTo(-(373.333*(FoV+10)/(FoV+10))+(angle/wideFov)*(746.666*(FoV+10)/(FoV+10)), (1.0f - (i->mob.GetHeight())/2.0f) * (512.0f/dist));
					i->mob.ScaleSprite(1.0/dist);
					i->mob.SetDistance(800-(dist*4));
					
					i->onscreen = true;			
				}
			}
		}
		
		mobs.sort(MobManager::MobComparator);
	}
}

void MobManager::DEBUGFindMobPositions(Vector2 playerPosition, Vector2 playerDirection, float FoV)
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
//				dist = dist / correcting;
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
	
	TexManager.LoadTexture("mob1.bmp");
	TexManager.LoadTexture("mob2.bmp");	
}

void MobManager::RemoveMob(MobData m)
{
//	mobs.remove(m);
}

void MobManager::UpdateMobs(Level& level, double timeDif)
{
	if (!mobs.empty()) {
		std::list<MobData>::iterator i;
		for(i = mobs.begin(); i != mobs.end(); i++) {
			i->mob.Update(level, timeDif);
		}	
	}
}

void MobManager::ShootMobs(float distanceToNearestWall, int damage)
{
	if (!mobs.empty()) {
		std::list<MobData>::iterator i;
		int count = mobs.size();
		for(i = mobs.end(); i != mobs.begin();) {
			i--;		
			if (((800-(i->mob.GetDistance()))/4 < distanceToNearestWall) && (i->mob.IsInside(Vector2(0.0f, 0.0f))) && !(i->mob.IsDead())) {
				i->mob.Damage(damage);
				break;
			}
		}

		for(i = mobs.begin(); i != mobs.end(); i++) {
			if (i->mob.IsDead() && !i->dead) {
				i->dead = true;
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
{
	mobs.clear();
}


bool MobManager::MobComparator(MobData lhs, MobData rhs)
{
		return rhs.mob.GetDistance() > lhs.mob.GetDistance();
}

int MobManager::CheckMobDealtDamage()
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
{
	if (!mobs.empty()) {
		std::list<MobData>::iterator i;
		for(i = mobs.begin(); i != mobs.end(); i++) {
			i->mob.SetHitpoints(hp);
		}	
	}
}

void MobManager::SetMobDamage(int damage)
{
	if (!mobs.empty()) {
		std::list<MobData>::iterator i;
		for(i = mobs.begin(); i != mobs.end(); i++) {
			i->mob.SetDamage(damage);
		}	
	}
}

void MobManager::SetMobSpeed(float speed)
{
	if (!mobs.empty()) {
		std::list<MobData>::iterator i;
		for(i = mobs.begin(); i != mobs.end(); i++) {
			i->mob.SetSpeed(speed);
		}	
	}
}