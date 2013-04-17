#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "vector2.h"
#include "raycaster.h"
#include "primitives.h"
#include "texture.h"
 
#include "ps2maths.h"

#include <map>
#include <string>

class Renderer
{
public:
	Renderer();
	~Renderer();
	
	void DrawScene();
	void SetLevel(Level* level);
	void SetFoV(float fov);
	float GetFoV(){return (FoV*180/PI);}
	void InitTextures();
	void BuildScene();
	
	void ConstructDepthMap();
	float GetCentreWallDistance();
	
	void OutputDepthMap();
	void SetPosition(Vector2 pos);
	void SetDirection(Vector2 dir);
	
	void UVSnapShot();
	
private:
	static const int RES = 160;
	static const int NUMTEXTURES = 16;

	float depthMap[RES+1];
	PS2SpriteT sprites[RES];
	RayReturnData rayMap[RES+1];
	float FoV;
	Level* currentLevel;
	Vector2 position;
	Vector2 direction;
	
	static std::map<unsigned int, int> colourMap;
	std::string textureFile;
	static Vector2 textureMap[NUMTEXTURES];

	
	RayCaster raycaster;
};
#endif