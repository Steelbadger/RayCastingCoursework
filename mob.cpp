#include "mob.h"
#include "texturemanager.h"

#include "PS2Defines.h"

#include <iostream>


Mob::Mob(): position(1.0f, 1.0f), target(0.0f, 0.0f), health(50)
{}

Mob::Mob(Vector2 pos, Vector2 size, std::string texture, Vector2 UVBase, Vector2 UVSize):
	position(pos),
	textureFile(texture),
	height(size.y),
	sprite(0.0f, 0.0f, size.x*256.0f, size.y*256.0f),
	health(50)
{
	sprite.SetUVs(UVBase.x, UVBase.y, UVSize.x, UVSize.y);
	TexManager.LoadTexture(textureFile);
}

Mob::~Mob()
{}


void Mob::MoveTo(float x, float y)
{
	position = Vector2(x, y);
}
void Mob::MoveTo(Vector2 newPos)
{
	position = newPos;
}

void Mob::MoveSpriteTo(float x, float y)
{
	sprite.MoveTo(x, y);
}

void Mob::MoveDelta(float x, float y)
{
	position += Vector2(x, y);
}
void Mob::MoveDelta(Vector2 movement)
{
	position += movement;
}

void Mob::Render()
{
	TexManager.UploadTextureToBuffer(textureFile, TextureManager::BUFFER1);
	TexManager.SetTexture(textureFile);
	sprite.Render();
}

bool Mob::IsInside(Vector2 point)
{
	Vector2 Pos = GetSpritePosition();
	Vector2 siz = GetSpriteSize();
	std::cout << "Compare Position: " << Pos << "\tSize: " << siz << std::endl;
	std::cout << "With: " << point << std::endl;
	if (point.x > (sprite.GetX() - sprite.GetWidth()*sprite.GetScale()*0.5f) &&
		point.x < (sprite.GetX() + sprite.GetWidth()*sprite.GetScale()*0.5f))
	{
		if (point.y > (sprite.GetY() - sprite.GetHeight()*sprite.GetScale()*0.5f) &&
			point.y < (sprite.GetY() + sprite.GetHeight()*sprite.GetScale()*0.5f))
		{
			return true;
		}
	}

	
	return false;
}