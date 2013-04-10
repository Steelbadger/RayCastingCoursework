#include "texturemanager.h"

TextureManager TextureManager_SingletonInit;

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{}
	
void TextureManager::LoadTexture(std::string filename)
{
	if (textureMap.count(filename) == 0 ) {
		textureMap[filename] = CTexture();
		textureMap[filename].LoadBitmap(filename.c_str(), true, false);
		std::cout << "Successfull Load: " << filename << std::endl;
	}
}

void TextureManager::UploadTextureToBuffer(std::string filename, TextureBuffer target)
{
	if (target == BUFFER1 && textureInBuffer1 != filename) {
		textureMap[filename].Upload(TEXBUF480);	
	} else if (textureInBuffer2 != filename) {
		textureMap[filename].Upload(TEXBUF496);
	}
	
}

void TextureManager::SetTexture(std::string filename)
{
	textureMap[filename].Select();
}