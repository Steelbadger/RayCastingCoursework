//////////////////////------------------//////////////////////
/*			
						By Ross Davies
															*/
//////////////////////------------------//////////////////////

#include "renderer.h"
#include "texturemanager.h"

#include "primitives.h"

#include <iostream>

//  No longer used, can be used to render block colours instead of textures
std::map<unsigned int, int> Renderer::colourMap;

//  Texture map UV bases (put in texture index, get out UV coords)
Vector2 Renderer::textureMap[NUMTEXTURES];

Renderer::Renderer()
//  On construction create the sprites we need to represent the terrain then
//  move each one to the horizontal centreline on the screen
{
	for (int i = 0; i < RES; i++) {
		sprites[i].SetWidth(640.0f/RES);
		sprites[i].MoveTo(-320.0f + i * (640.0f/RES) + (320.0f/RES), 0.0f);
		sprites[i].MoveTo(-320.0f + i * (640.0f/RES), 0.0f);		
	}
	
	//  Set the texture we will use for walls
	textureFile = "WallSheet8BitIndexed.bmp";
	
	//  Precalculate the UV base coordiantes of the textures we'll use
	for (int i = 0; i < NUMTEXTURES; i++)
	{
		textureMap[i] = Vector2((i%4)*64, (i/4)*64);
	}
}

void Renderer::InitTextures()
//  Load textures to memory (call after dma managers have been set up)
{
	TexManager.LoadTexture(textureFile);	
}

Renderer::~Renderer()
{
}


void Renderer::DrawScene()
//  Load textures to buffer and render each environment sprite
{
	TexManager.UploadTextureToBuffer(textureFile, TextureManager::BUFFER2);
	TexManager.SetTexture(textureFile);
	for (int i = 0; i < RES; i++) {
		sprites[i].Render();
	}
	
}

void Renderer::BuildScene()
//  For each bar of the depth map
//  Set that sprite to correct height (skew sprite for improved apparent image)
//  And set the UVs for each corner to what they should be
{
	for (int i = 0; i < RES; i++) {
		//  Skew the sprite (each edge set to correct height for distance to camera)
		sprites[i].Skew(640/rayMap[i].range,640/rayMap[i+1].range);
		//  Set the depth of the sprite so z-sorting is done properly
		sprites[i].SetDepth(800-((rayMap[i].range + rayMap[i+1].range)*2));
		//  Find the texture of the slice from the ray map
		int textureOfSlice = rayMap[i].TextureID;
		//  Find the UV coordinates of that texture
		Vector2 baseOfTexture = textureMap[textureOfSlice-1];
		//  Find what portion of the texture this sprite should show
		Vector2 offset(rayMap[i].v, rayMap[i+1].v);
		//  multiply by 64, the width of a single wall texture block
		offset *= 64;
		//  Set these UVs to the sprite
		sprites[i].SetUVs(baseOfTexture.x + offset.x, baseOfTexture.y, 
							offset.y - offset.x, 64);							
			
	}	
}

void Renderer::UVSnapShot()
//  Output the UVs of the centre scanline
{

	std::cout << textureMap[rayMap[RES/2].TextureID-1] << "\t";
	std::cout << textureMap[rayMap[RES/2].TextureID-1].x + (64*rayMap[RES/2].v) << std::endl;


}


void Renderer::SetLevel(Level* level)
//  Set the level we wish to render
{
	currentLevel = level;
}

void Renderer::SetFoV(float fov)
//  Set the field of view (input in degrees, stored as radians)
{
	FoV = DegToRad(fov);
}

void Renderer::ConstructDepthMap()
//  Build the depth map from which we build the scene
{
	//  an array of all the directions we wish to shoot rays
	Vector2 directions[RES+1];
	//  an array of the correction factors to remove the fish-eye effect
	float corrections[RES+1];
	
	//  Temp is where we create the rays before we push them to the array
	Vector2 temp;
	temp = direction;
	float FoV2 = FoV/2;
	float FoVRES = FoV/RES;
	//  First ray is at the left-most side of the screen
	temp.Rotate(-FoV2);
	
	//  Then step across the screen rotating temp by a little each time to find the new ray direction
	for (int i = 0; i <= RES; i++) {
		directions[i] = temp;
		corrections[i] = Cos(-FoV2 + i*FoVRES);
		temp.Rotate(FoVRES);
	}
	
	//  Now build the depth may by casting rays for each direction and store the return data
	RayReturnData ray;	
	for (int i = 0; i <= RES; i++) {
		ray = raycaster.CastRay(position, directions[i], *currentLevel);
		rayMap[i] = ray;
		rayMap[i].range = rayMap[i].range * corrections[i];
	}
}

void Renderer::OutputDepthMap()
//  Output the depth map to the console out
{
	for (int i = 0; i <= RES; i++) {
		std::cout << depthMap[i] << " : ";
	}
	std::cout << std::endl;	
}

float Renderer::GetCentreWallDistance()
//  Find the distance to the wall segment in the centre of the screen (for checking bullet collision)
{
	return rayMap[(RES-2)/2].range;
}

void Renderer::SetPosition(Vector2 pos)
//  Give the renderer the position from which to render the scene
{
	position = pos;
}

void Renderer::SetDirection(Vector2 dir)
//  Give the renderer the direction in which the scene should look
{
	direction = dir;
}