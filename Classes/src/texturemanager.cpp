//////////////////////------------------//////////////////////
/*			
						By Ross Davies
															*/
//////////////////////------------------//////////////////////


#include "texturemanager.h"

//  Create the singleton
TextureManager TextureManager_SingletonInit;

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{}
	
void TextureManager::LoadTexture(std::string filename)
//  Load texture by filename string and store in map for later look up
{
	std::string texturelocation = "textures/";
	texturelocation+=filename;
	//  Only load the image if not previously loaded
	if (textureMap.count(filename) == 0 ) {
		textureMap[filename] = CTexture();
		
		textureMap[filename].LoadBitmap(texturelocation.c_str(), true, false);
		std::cout << "Successfull Load: " << filename << std::endl;
	}
}

void TextureManager::UploadTextureToBuffer(std::string filename, TextureBuffer target)
//  Send the texture (by filename) to the target buffer, ready for being selected (MUST LOAD BEFORE THIS)
{
	if (target == BUFFER1 && textureInBuffer1 != filename) {
		textureMap[filename].Upload(TEXBUF480);	
	} else if (textureInBuffer2 != filename) {
		textureMap[filename].Upload(TEXBUF496);
	}
	
}

void TextureManager::SetTexture(std::string filename)
//  Select a buffer for drawing (use string to find texture)
{
	textureMap[filename].Select();
}