#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__

#include "texture.h"
#include "singleton.h"

#include <string>
#include <map>


#define TexManager TextureManager::GetSingleton()

class TextureManager : public CSingleton<TextureManager>
{
public:
	enum TextureBuffer {BUFFER1, BUFFER2};
	TextureManager();
	virtual ~TextureManager();
	
	void LoadTexture(std::string filename);
	void UploadTextureToBuffer(std::string filename, TextureBuffer target);
	void SetTexture(std::string filename);
	
private:
	std::string textureInBuffer1;
	std::string textureInBuffer2;
	
	std::map<std::string, CTexture> textureMap;
};

#endif
