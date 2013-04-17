#include "renderer.h"
#include "texturemanager.h"

#include "primitives.h"

#include <iostream>


std::map<unsigned int, int> Renderer::colourMap;
Vector2 Renderer::textureMap[NUMTEXTURES];

Renderer::Renderer()
{
	for (int i = 0; i < RES; i++) {
		sprites[i].SetWidth(640.0f/RES);
		sprites[i].MoveTo(-320.0f + i * (640.0f/RES) + (320.0f/RES), 0.0f);
		sprites[i].MoveTo(-320.0f + i * (640.0f/RES), 0.0f);		
	}
	
	textureFile = "WallSheet8BitIndexed.bmp";
	
	for (int i = 0; i < NUMTEXTURES; i++)
	{
		textureMap[i] = Vector2((i%4)*64, (i/4)*64);
		//std::cout << textureMap[i] << std::endl;
	}
}

void Renderer::InitTextures()
{
	TexManager.LoadTexture(textureFile);	
}

Renderer::~Renderer()
{
}


void Renderer::DrawScene()
{
	TexManager.UploadTextureToBuffer(textureFile, TextureManager::BUFFER2);
	TexManager.SetTexture(textureFile);
	for (int i = 0; i < RES; i++) {
		sprites[i].Render();
	}
	
}

void Renderer::BuildScene()
{
	//  For each bar of the depth map
	//  Set that sprite to correct height
	for (int i = 0; i < RES; i++) {
		sprites[i].Skew(640/rayMap[i].range,640/rayMap[i+1].range);
		sprites[i].SetDepth(800-((rayMap[i].range + rayMap[i+1].range)*2));
		int textureOfSlice = rayMap[i].TextureID;	
		Vector2 baseOfTexture = textureMap[textureOfSlice-1];
		Vector2 offset(rayMap[i].v, rayMap[i+1].v);
		offset *= 64;		
		sprites[i].SetUVs(baseOfTexture.x + offset.x, baseOfTexture.y, 
							offset.y - offset.x, 64);							
			
	}	
}

void Renderer::UVSnapShot()
{

	std::cout << textureMap[rayMap[RES/2].TextureID-1] << "\t";
	std::cout << textureMap[rayMap[RES/2].TextureID-1].x + (64*rayMap[RES/2].v) << std::endl;


}


void Renderer::SetLevel(Level* level)
{
	currentLevel = level;
}

void Renderer::SetFoV(float fov)
{
	FoV = DegToRad(fov);
}

void Renderer::ConstructDepthMap()
{
	Vector2 directions[RES+1];
	float corrections[RES+1];
	Vector2 temp;
	temp = direction;
	float FoV2 = FoV/2;
	float FoVRES = FoV/RES;
	temp.Rotate(-FoV2);
	for (int i = 0; i <= RES; i++) {
//		std::cout << "FirstLoop i: " << i << std::endl;
		directions[i] = temp;
		corrections[i] = Cos(-FoV2 + i*FoVRES);
		temp.Rotate(FoVRES);
	}
	RayReturnData ray;	
	for (int i = 0; i <= RES; i++) {
//		std::cout << "SecondLoop i: " << i << std::endl;	
		ray = raycaster.CastRay(position, directions[i], *currentLevel);
		rayMap[i] = ray;
		rayMap[i].range = rayMap[i].range * corrections[i];
	}
}

void Renderer::OutputDepthMap()
{
	for (int i = 0; i <= RES; i++) {
		std::cout << depthMap[i] << " : ";
	}
	std::cout << std::endl;	
}

float Renderer::GetCentreWallDistance()
{
	return rayMap[(RES-2)/2].range;
}

void Renderer::SetPosition(Vector2 pos)
{
	position = pos;
}

void Renderer::SetDirection(Vector2 dir)
{
	direction = dir;
}