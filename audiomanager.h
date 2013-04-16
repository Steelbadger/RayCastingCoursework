#ifndef __AUDIOMANAGER_H__
#define __AUDIOMANAGER_H__

#include "audio.h"
#include "singleton.h"
#include <string>
#include <map>

#define Audio AudioManager::GetSingleton()

class AudioManager : public CSingleton<AudioManager>
{
public:
	enum TargetAudioDevice {DSP0, DSP1};
	AudioManager();
	virtual ~AudioManager();
	void Update();
	void Initialise();
	void Load(std::string, TargetAudioDevice);
	void Reset(std::string);
	void Stop(std::string);
	void Play(std::string);
	void PlayLoop(std::string);
	
private:
	AudioDevice* audioDevices[2];
	std::map<std::string, SoundSample*> sampleMap;
};


#endif