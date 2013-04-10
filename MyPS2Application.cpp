#include "MyPS2Application.h"

#include <sps2tags.h>
#include <sps2regstructs.h>
#include <signal.h>
#include "PS2Defines.h"
#include "pad.h"
#include "sps2wrap.h"
#include "dma.h"
#include <time.h>

#define NUMSPRITES	(100)

bool MyPS2Application::quitting_ = false;

void sig_handle(int sig)
{
	MyPS2Application::set_quitting(true);
}

MyPS2Application::MyPS2Application()
{

}

void MyPS2Application::Run()
{
	Init();
	while(!quitting_)
	{
		Update();
		Render();
	}
	CleanUp();
}

void MyPS2Application::Init()
{
	// Initialise control pad 0
	if(!pad_init(PAD_0, PAD_INIT_LOCK | PAD_INIT_ANALOGUE | PAD_INIT_PRESSURE))
	{
		printf("Failed to initialise control pad\n");
		pad_cleanup(PAD_0);
		exit(0);
	}	
	
	// Allocate memory for the graphics data
	SPS2Manager.Initialise(1024);	// 1024 * 4K Pages = 4MB Total
	VIFStaticDMA.Initialise(512);	// 512 * 4K Pages = 2MB Static DMA
	VIFDynamicDMA.Initialise(256);	// 256 * 4K Pages * 2 Buffers =
									// 2MB Dynamic DMA
																	
	// Register our signal function for every possible signal (e.g. ctrl + c)
	for(int sig=0; sig<128; sig++)
		signal(sig, sig_handle);

	// Set up the DMA packet to clear the screen. We want to clear to black.
	SPS2Manager.InitScreenClear(0, 0, 0);
	
	startupState.Initialise();
	VIFDynamicDMA.Fire();	
	menuState.Initialise();
	playState.Initialise();
	currentState = &startupState;
	
}

void MyPS2Application::CleanUp()
{
	SPS2Manager.CleanUp();
	pad_cleanup(PAD_0);
}

void MyPS2Application::Update()
{
	// Tell DMAC to send previous graphics data to Graphics Synthesiser (GS)
	VIFDynamicDMA.Fire();
	
	// Read the control pad into data buffer
	pad_update(PAD_0);
	
	// Check for exit condition	
	if((pad[0].buttons & PAD_START)&&(pad[0].buttons & PAD_SELECT)) quitting_ = true;	
	
	currentState->Update();
	CheckState();

}

void MyPS2Application::Render()
{
	// All drawing commands should be between BeginScene and EndScene
	SPS2Manager.BeginScene();
	currentState->Render();
	SPS2Manager.EndScene();			
}

void MyPS2Application::CheckState()
{
	switch (currentState->GetReturn()) {
		case GameState::NONE :			return;
		case GameState::STARTUP : 		currentState = &startupState;
										break;
		case GameState::MENU : 			currentState = &menuState;
										break;
		case GameState::OPTIONS : 		currentState = &menuState;
										break;
		case GameState::HELP : 			currentState = &menuState;
										break;
		case GameState::GAMEACTIVE :	currentState = &playState;
										break;
		case GameState::GAMEPAUSED : 	currentState = &playState;
										break;
		case GameState::GAMEWIN :		currentState = &menuState;
										break;
		case GameState::GAMELOSE : 		currentState = &menuState;
										break;
		case GameState::QUIT :			quitting_ = true;
										break;
	}
}