#include "audiomanager.h"

//  create the singleton
AudioManager AudioManager_SingletonInit;

AudioManager::AudioManager()
{

}

AudioManager::~AudioManager()
//  Clean up the audio devices and delete the samples we created
{
	audioDevices[0]->Close();
	audioDevices[1]->Close();
	delete audioDevices[0];
	delete audioDevices[1];
	
	std::map<std::string, SoundSample*>::iterator it;
	
	for (it = sampleMap.begin(); it !=sampleMap.end(); it++) {
		delete it->second;
	}
}

void AudioManager::Initialise()
//  Create our audio devices
{
	audioDevices[0] = new AudioDevice(0);
	audioDevices[1] = new AudioDevice(1);
}

void AudioManager::Update()
//  Update the audio devices
{
	audioDevices[0]->HandleAudio();
	audioDevices[1]->HandleAudio();
}

void AudioManager::Load(std::string file, TargetAudioDevice target)
//  Load a sample identified by file into target device
{
	if (sampleMap.count(file) == 0) {
		if (target == DSP0) {
			sampleMap[file] = new SoundSample(file.c_str(), audioDevices[0]);
		} else if (target == DSP1) {		
			sampleMap[file] = new SoundSample(file.c_str(), audioDevices[1]);	
		}
	}
}


//  Wrappers for SoundSample functions using strings to lookup
void AudioManager::Reset(std::string file)
{
	sampleMap[file]->Reset();
}

void AudioManager::Play(std::string file)
{
	sampleMap[file]->Play();
}

void AudioManager::PlayLoop(std::string file)
{
	sampleMap[file]->PlayLoop();
}

void AudioManager::Stop(std::string file)
{
	sampleMap[file]->Stop();
}