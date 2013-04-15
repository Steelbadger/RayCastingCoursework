#ifndef __MOB_H__
#define __MOB_H__
#include "animatedsprite.h"
#include "texture.h"
#include <string>

#include "vector2.h"

class Level;

class Mob
{	
public:	
	Mob();
	Mob(Vector2 position);

	~Mob();

	Vector2 GetPosition(){return position;}
	Vector2 GetSpritePosition(){return Vector2(sprite.GetX(), sprite.GetY());}
	Vector2 GetSpriteSize(){return Vector2(sprite.GetWidth(), sprite.GetHeight())*sprite.GetScale();}
	bool IsInside(Vector2 point);
	void MoveTo(float x, float y);
	void MoveTo(Vector2 newPos);
	void MoveSpriteTo(float x, float y);
	void MoveDelta(float x, float y);
	void MoveDelta(Vector2 movement);
	void SetDistance(float z){sprite.SetDepth(z);}
	float GetDistance(){return sprite.GetZ();}
	bool IsDead(){return (health > 0? false : true);}
	bool DoneDying(){return !sprite.IsAnimating();}
	void Kill();
	int GetHealth(){return health;}
	void SetTarget(Vector2 tar){target = tar;}
	void CatastrophicKill();
	void Damage(int dmg);
	void AnimateAttack();
	void ScaleSprite(float scaleFactor){sprite.UniformScale(scaleFactor);}
	void Update(Level& level, double timeDif);
	void Render();
	float GetHeight(){return height;}
	void AnimateWalk();
	int GetDamageDealt(){return damageDealt;}
	void WalkToTarget(Level& level, double timeDif);

private:
	Vector2 position;
	Vector2 target;
	float height;
	int health;
	AnimatedSprite sprite;
	std::string currentTex;
	std::string texFile;
	std::string megaKillTex;
	Vector2 spriteTextureBase;
	Vector2 spriteTextureSize;
	bool attacking;
	bool wasAttacking;
	int damageDealt;
};



#endif