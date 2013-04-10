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

	// Set up the DMA packet to clear the screen. We want to clear to blue.
	SPS2Manager.InitScreenClear(0, 0x25, 0x10);
	
	testPosition = Vector2(2.0, 2.0);
	testDirection = Vector2(1.0, 1.0);
	testRenderer.SetLevel(&testLevel);
	testRenderer.SetPosition(testPosition);
	testRenderer.SetDirection(testDirection);
	testRenderer.SetFoV(60.0f);
	testRenderer.InitTextures();

	Mob testMob(Vector2(3.0f, 3.0f), Vector2(1.4f, 1.4f), "testEnemy.bmp", Vector2(0,0), Vector2(64, 64));
	testMobs.AddMob(testMob);
	testMob = Mob(Vector2(2.0f, 3.0f), Vector2(1.5f, 1.5f), "testEnemy.bmp", Vector2(0,0), Vector2(64, 64));
	testMobs.AddMob(testMob);
	testMob = Mob(Vector2(18.0f, 2.0f), Vector2(1.6f, 1.6f), "testEnemy.bmp", Vector2(0,0), Vector2(64, 64));
	testMobs.AddMob(testMob);
	timeDiff = 10.0f;
	
	testGun.Init();

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
	
	
	lastTime = currentTime;
	currentTime = clock();
	timeDiff = (currentTime - lastTime)/CLOCKS_PER_SEC;

	
	testDirection.Rotate(pad[0].axes[2]*timeDiff * 4.0f);
	testRenderer.SetDirection(testDirection);
	testStrafe = testDirection;
	testStrafe.Rotate(90);
	Vector2 translationVector = testDirection.Normalise() * (-pad[0].axes[1] * timeDiff * 3.0f) + testStrafe.Normalise() * (pad[0].axes[0] * timeDiff * 3.0f);
	int x, y;
	x = testPosition.x + (translationVector.x) + (translationVector.x/Abs(translationVector.x))*0.2;
	y = testPosition.y + (translationVector.y) + (translationVector.y/Abs(translationVector.y))*0.2;

	if (pad[0].pressed & PAD_CROSS) {
		std::cout << "TimeDiff: " << timeDiff << "\tFrameRate: " << 1/timeDiff << std::endl;
	}
	
	if (pad[0].pressed & PAD_L2) {
		testMobs.ShootMobs(testRenderer.GetCentreWallDistance());
		testGun.Fire();
	}		
	
	if (testLevel.At(x, testPosition.y) == 0) {
		testPosition.x += translationVector.x;
	}
	if (testLevel.At(testPosition.x, y) == 0) {
		testPosition.y += translationVector.y;
	}
	
	testRenderer.ConstructDepthMap();	
	testRenderer.SetPosition(testPosition);	
	testRenderer.BuildScene();
	testMobs.FindMobPositions(testPosition, testDirection, 60.0f);
	testGun.Update();
		

}

void MyPS2Application::Render()
{
	// All drawing commands should be between BeginScene and EndScene
	SPS2Manager.BeginScene();
	testRenderer.DrawScene();
	testMobs.DrawMobs();
	testGun.Render();
	SPS2Manager.EndScene();			
}