#include "optionsstate.h"

#include "texturemanager.h"
#include "pad.h"


OptionsState::OptionsState():
	optionsTitle(0, -128, 300, 150),
	difficulty(-80, 32, 128, 32),
	diffOpt(80, 32, 128, 32),
	rumble(-80, 64, 128, 32),
	rumbleOpt(80, 64, 128, 32),
	sensitivity(-80, 96, 128, 32),
	sensOpt(80, 96, 128, 32),
	back(0, 170, 128, 32),
	reset(0, 128, 128, 32),
	cursorPos(DIFFICULTY),
	difficultyImg("diff.bmp"),
	rumbleImg("rumble.bmp"),
	sensitivityImg("sensitivity.bmp"),
	titleImg("optionstitle.bmp"),
	background(NULL),
	darkenOverlay(0,0,640, 512),	
	GameState(GameState::OPTIONS)	
{

}

OptionsState::~OptionsState()
{

}

void OptionsState::Initialise()
{
	originalOptions.difficulty = Options::EASY;
	originalOptions.rumble = Options::RUMBLEOFF;
	originalOptions.sensitivity = Options::MED;
	
	currentOptions = originalOptions;
	
	optionsTitle.SetUVs(0,0,256,128);
	optionsTitle.SetDepth(901);
	
	difficulty.SetUVs(0,128,256,64);
	difficulty.SetDepth(901);
	
	diffOpt.SetUVs(0,0,256,64);
	diffOpt.SetDepth(901);
	
	rumble.SetUVs(0,128,256,64);
	rumble.SetDepth(901);
	
	rumbleOpt.SetUVs(0,0,256,64);
	rumbleOpt.SetDepth(901);	
	
	sensitivity.SetUVs(0, 192, 256, 64);
	sensitivity.SetDepth(901);

	sensOpt.SetUVs(0, 64, 256, 64);
	sensOpt.SetDepth(901);
	
	back.SetUVs(0, 192, 256, 64);
	back.SetDepth(901);	
	
	reset.SetUVs(0, 192, 256, 64);
	reset.SetDepth(901);		
	
	darkenOverlay.SetColour(0x000000);
	darkenOverlay.SetAlpha('l');
	darkenOverlay.SetDepth(900);
	
	TexManager.LoadTexture(difficultyImg);
	TexManager.LoadTexture(rumbleImg);
	TexManager.LoadTexture(sensitivityImg);	
	TexManager.LoadTexture(titleImg);

	analogueDelay = 10;
}

void OptionsState::Update()
{
	CheckInput();
	SetDifficultyOptionSprite();
	SetRumbleOptionSprite();	
	SetSensitivityOptionSprite();	
}

void OptionsState::Render()
{
	if (background != NULL) {
		if (background->GetState() == GameState::GAMEACTIVE) {
			background->Render();
		}
	}
	darkenOverlay.Render();
	
	TexManager.UploadTextureToBuffer(difficultyImg, TextureManager::BUFFER1);
	TexManager.UploadTextureToBuffer(titleImg, TextureManager::BUFFER2);
	TexManager.SetTexture(titleImg);
	
	optionsTitle.Render();
	difficulty.Render();
	back.Render();
	
	TexManager.UploadTextureToBuffer(rumbleImg, TextureManager::BUFFER2);
	TexManager.SetTexture(difficultyImg);
	
	diffOpt.Render();
	
	TexManager.UploadTextureToBuffer(sensitivityImg, TextureManager::BUFFER1);
	TexManager.SetTexture(rumbleImg);
	
	rumbleOpt.Render();
	rumble.Render();
	reset.Render();
	
	TexManager.SetTexture(sensitivityImg);
	
	sensitivity.Render();
	sensOpt.Render();
}

void OptionsState::PriorState(GameState* prior)
{
	background = prior;
	if (background->GetState() == GameState::GAMEACTIVE) {
		darkenOverlay.SetAlpha('l');
	} else {
		darkenOverlay.SetAlpha(0xFF);
	}
}

Options OptionsState::GetOptions()
{
	return currentOptions;
}

PS2SpriteT& OptionsState::GetOptionOfCursor()
{
	switch (cursorPos) {
		case DIFFICULTY: 	return difficulty;
		case RUMBLE:		return rumble;
		case SENSITIVITY:	return sensitivity;
		case RETURN:		return back;
		case RESET:			return reset;
	}
	
	//  Should Never Happen
	return back;
}

void OptionsState::SetDifficultyOptionSprite()
{
	int offs = int(currentOptions.difficulty);
	diffOpt.SetUVs(0, 64*offs, 256, 64);
}

void OptionsState::SetRumbleOptionSprite()
{
	int offs = int(currentOptions.rumble);
	rumbleOpt.SetUVs(0, 64*offs, 256, 64);
}

void OptionsState::SetSensitivityOptionSprite()
{
	int offs = int(currentOptions.sensitivity);
	sensOpt.SetUVs(0, 64*offs, 256, 64);
}

void OptionsState::CheckInput()
{
	if (pad[0].pressed & PAD_CROSS) {
		switch (cursorPos) {
			case DIFFICULTY: 	currentOptions.difficulty = Options::Difficulty(int(currentOptions.difficulty)+1);
								break;
			case RUMBLE:		currentOptions.rumble = Options::Rumble(int(currentOptions.rumble)+1);
								break;
			case SENSITIVITY:	currentOptions.sensitivity = Options::Sensitivity(int(currentOptions.sensitivity)+1);
								break;
			case RESET:			currentOptions = originalOptions;
								break;
			case RETURN:		value = background->GetState();
								if (value == GameState::GAMEACTIVE) {
									value = GameState::GAMEPAUSED;
								}
								SaveOptions();
								break;
		}
	}
	
	if (pad[0].pressed & PAD_CIRCLE) {
		currentOptions = originalOptions;	
		value = background->GetState();
		if (value == GameState::GAMEACTIVE) {
			value = GameState::GAMEPAUSED;
		}		
	}
	
	if (pad[0].axes[1] == 0 && pad[0].axes[0] == 0) {
		analogueDelay = 10;
	}
	int optionsOffset;
	optionsOffset = 0;
	if (pad[0].axes[0] < 0) {
		analogueDelay -= pad[0].axes[0];
		if (analogueDelay > 10) {
			analogueDelay = 0;		
			optionsOffset--;
		}
	}	
	if (pad[0].axes[0] > 0) {
		analogueDelay += pad[0].axes[0];
		if (analogueDelay > 10) {
			analogueDelay = 0;		
			optionsOffset++;
		}
	}
	
	if (pad[0].pressed & PAD_RIGHT) {
		optionsOffset++;
	}
	
	if (pad[0].pressed & PAD_LEFT) {
		optionsOffset--;
	}	
		
	int tempOption;
	
	switch (cursorPos) {
		case DIFFICULTY: 	tempOption = int(currentOptions.difficulty) + optionsOffset;
							tempOption = (tempOption > 3 ? 3 : tempOption%4);
							tempOption = (tempOption < 0 ? 0 : tempOption%4);
							currentOptions.difficulty = Options::Difficulty(tempOption);
							break;
		case RUMBLE:		tempOption = int(currentOptions.rumble) + optionsOffset;
							tempOption = (tempOption > 1 ? 1 : tempOption%2);
							tempOption = (tempOption < 0 ? 0 : tempOption%2);
							currentOptions.rumble = Options::Rumble(tempOption);
							break;
		case SENSITIVITY:	tempOption = int(currentOptions.sensitivity) + optionsOffset;
							tempOption = (tempOption > 2 ? 2 : tempOption%3);
							tempOption = (tempOption < 0 ? 0 : tempOption%3);
							currentOptions.sensitivity = Options::Sensitivity(tempOption);		
							break;
		default:			break;
	}	
	
	
	int cursorTemp = int(cursorPos);
	if (pad[0].pressed & PAD_UP) {
		cursorTemp--;
		GetOptionOfCursor().UniformScale(1.0);
	}
	
	if (pad[0].axes[1] < 0) {
		analogueDelay -=pad[0].axes[1];
		if (analogueDelay > 10) {
			analogueDelay = 0;		
			cursorTemp--;
			GetOptionOfCursor().UniformScale(1.0);
		}
	}	
		
	
	if (pad[0].pressed & PAD_DOWN) {
		cursorTemp++;
		GetOptionOfCursor().UniformScale(1.0);		
	}
	
	if (pad[0].axes[1] > 0) {
		analogueDelay += pad[0].axes[1];
		if (analogueDelay > 10) {
			analogueDelay = 0;
			cursorTemp++;
			GetOptionOfCursor().UniformScale(1.0);
		}
	}

	cursorTemp = (cursorTemp > 4 ? 4 : cursorTemp);
	cursorTemp = (cursorTemp < 0 ? 0 : cursorTemp);
	cursorPos = OptionSelect(cursorTemp);
	GetOptionOfCursor().UniformScale(1.5);			
}

void OptionsState::SaveOptions()
{
	originalOptions = currentOptions;
}

