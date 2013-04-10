#include "playstate.h"


PlayState::PlayState()
{}

PlayState::~PlayState()
{}


void PlayState::Initialise()
{
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

void PlayState::Update()
{
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

void PlayState::Render()
{
	testRenderer.DrawScene();
	testMobs.DrawMobs();
	testGun.Render();
}

