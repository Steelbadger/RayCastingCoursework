#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__
//////////////////////--By Ross Davies--//////////////////////
/*		The texturemanger is a wrapper and lookup
		to abstract away the need to have actual
		texture objects in every class with sprites
		instead those objects now just hold strings
		which are used to lookup in this object for
		the requested texture.	SINGLETON					*/
//////////////////////////////////////////////////////////////
#include "texture.h"
#include "singleton.h"

#include <string>
#include <map>


#define TexManager TextureManager::GetSingleton()

class TextureManager : public CSingleton<TextureManager>
{
public:
	//  Enumerate the two target buffers
	enum TextureBuffer {BUFFER1, BUFFER2};
	
	//  Constructor
	TextureManager();
	
	//  Destructor
	virtual ~TextureManager();
	
	//  Load the texture from the file to memory
	void LoadTexture(std::string filename);
	//  Send the texture to the target buffer
	void UploadTextureToBuffer(std::string filename, TextureBuffer target);
	//  Make this texture active
	void SetTexture(std::string filename);
	
private:
	//  The files currently stored in the two buffers
	std::string textureInBuffer1;
	std::string textureInBuffer2;
	
	//  Look up for actual texture files referenced by the strings
	std::map<std::string, CTexture> textureMap;
};

#endif
