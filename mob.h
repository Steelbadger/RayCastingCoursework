#ifndef __MOB_H__
#define __MOB_H__
#include "primitives.h"
#include "texture.h"
#include <string>

#include "vector2.h"

class Mob
{	
public:	
	Mob();
	Mob(Vector2 position, Vector2 size, std::string texture, Vector2 UVBase, Vector2 UVSize);

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
	void Damage(int dmg){health -= dmg;}
	void ScaleSprite(float scaleFactor){sprite.UniformScale(scaleFactor);}
	void Render();
	float GetHeight(){return height;}

private:
	Vector2 position;
	Vector2 target;
	float height;
	int health;
	PS2SpriteT sprite;
	std::string textureFile;
	Vector2 spriteTextureBase;
	Vector2 spriteTextureSize;
};



#endif