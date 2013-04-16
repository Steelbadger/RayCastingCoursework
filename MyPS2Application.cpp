#include "MyPS2Application.h"

#include <sps2tags.h>
#include <sps2regstructs.h>
#include <signal.h>
#include "PS2Defines.h"
#include "pad.h"
#include "sps2wrap.h"
#include "dma.h"
#include <time.h>
#include "audiomanager.h"

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
	Audio.Initialise();
	
	//  Initialise our startup screen (this renders a loading image to the screen)
	//  Then fire this to the GS to give us something to look at while we load the rest
	startupState.Initialise();
	VIFDynamicDMA.Fire();	
	
	//  Initialise the rest of the states (sets up depth and UVs for all associated graphics)
	//  Also loads in level 1 to the playState ready to be played.
	menuState.Initialise();
	playState.Initialise();
	pauseState.Initialise();
	helpState.Initialise();
	winState.Initialise();
	loseState.Initialise();
	optionsState.Initialise();
	
	//  And finally set our state machine to startup.
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
	
	//  Run the current state, then check for any changes to the state after logic is complete
	currentState->Update();
	Audio.Update();
	CheckState();

}

void MyPS2Application::Render()
{
	// Render the active state
	SPS2Manager.BeginScene();
	currentState->Render();
	SPS2Manager.EndScene();			
}

void MyPS2Application::CheckState()
{
	GameState* oldstate = currentState;
	switch (currentState->GetReturn()) {
		case GameState::NONE :			return;
		case GameState::STARTUP : 		currentState = &startupState;
//										std::cout << "Enter State: Startup" << std::endl;		
										break;
		case GameState::MENU : 			currentState = &menuState;
//										std::cout << "Enter State: Menu" << std::endl;			
										break;
		case GameState::OPTIONS : 		currentState = &optionsState;
										currentState->PriorState(oldstate);	
//										std::cout << "Enter State: Options" << std::endl;											
										break;
		case GameState::HELP : 			currentState = &helpState;
										currentState->PriorState(oldstate);
//										std::cout << "Enter State: Help" << std::endl;											
										break;									
		case GameState::GAMEACTIVE :	currentState = &playState;
//										std::cout << "Enter State: GameActive" << std::endl;
										playState.StartPlayMusic();
										playState.PassOptions(optionsState.GetOptions());
										break;
		case GameState::GAMEPAUSED : 	currentState = &pauseState;
										currentState->PriorState(&playState);
//										std::cout << "Enter State: Paused" << std::endl;											
										break;
		case GameState::GAMEWIN :		currentState = &winState;
										currentState->PriorState(&playState);
//										std::cout << "Enter State: Win" << std::endl;			
										break;
		case GameState::GAMELOSE : 		currentState = &loseState;
										currentState->PriorState(&playState);		
										break;
		case GameState::QUIT :			quitting_ = true;
										break;
	}
	
}