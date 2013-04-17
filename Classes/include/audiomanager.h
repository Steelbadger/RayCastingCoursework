#ifndef __AUDIOMANAGER_H__
#define __AUDIOMANAGER_H__
//////////////////////--By Ross Davies--//////////////////////
/*		Wrapper for the audio devices, singleton as it
		handles all audio across entire program and
		should only be able to exist once (limited number
		of audio hardware channels)							*/
//////////////////////////////////////////////////////////////

#include "audio.h"
#include "singleton.h"
#include <string>
#include <map>

#define Audio AudioManager::GetSingleton()

class AudioManager : public CSingleton<AudioManager>
{
public:
	//  Enumerate the audio devices
	enum TargetAudioDevice {DSP0, DSP1};
	
	//  Contructor
	AudioManager();
	
	//  Destructor closes devices
	virtual ~AudioManager();
	
	//  Update sends the next audio packet to the relevant
	//  devices
	void Update();
	
	//  Opens audio devices
	void Initialise();
	
	//  load file by string for use in target audio device
	void Load(std::string, TargetAudioDevice);
	
	//  Reset audio (file by string) to beginning of track
	void Reset(std::string);
	
	//  Stop the track (file by string)
	void Stop(std::string);
	
	//  Play the track (file by string) in it's designated device
	void Play(std::string);
	
	//  Play the track (file by string) in it's designated device on a loop
	void PlayLoop(std::string);
	
private:

	//  The target audio devices
	AudioDevice* audioDevices[2];
	
	//  Storage for the tracks, referencable by filename string
	std::map<std::string, SoundSample*> sampleMap;
};


#endif