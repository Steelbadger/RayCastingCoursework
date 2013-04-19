//////////////////////------------------//////////////////////
/*			
						By Ross Davies
															*/
//////////////////////------------------//////////////////////

#include "player.h"

#include "level.h"

#include "PS2Defines.h"
#include "pad.h"
#include "texturemanager.h"

#include <time.h>

//  Setup the player and HP bar/ammo counter
Player::Player():
	healthBar(0, -240, 256, 32),
	ammo(66, 192, 64, 64),
	ammoImg("ammo.bmp"),
	MAXHP(100)	
{}

Player::~Player()
{}


void Player::Initialise(Vector2 pos, Vector2 dir, Level& lev)
//  Set the player's initial conditions (position and direction)
//  Initialise weapons and set UVs for ammo sprite
{
	//  Set the position and direction of the player to start
	position = pos;
	direction = dir;
	
	//  Player recieves a pointer to the level for collision checks
	level = &lev;
	
	//  We have max health
	hitpoints = MAXHP;
	
	//  Initialise the guns and select the pistol
	pistol.Init();
	shotgun.Init();
	activeWeapon = &pistol;
	
	//  Base speed
	speed = 3.0f;
	
	//  Base sensitivity
	sensitivity = 4.0f;
	
	//  Red semi-transparent HP bar
	healthBar.SetColour(0x00FF00);
	healthBar.SetAlpha('l');
	healthBar.SetDepth(850);
	
	//  Rumble off
	rumbleIntensity = 0;
	
	//  Setup the tex coords for the ammo image
	ammo.SetUVs(0,32, 64, 32);
	ammo.SetDepth(850);
	
	// load the ammo texture
	TexManager.LoadTexture(ammoImg);
}

void Player::Update(double timeDif)
//  Check input and act upon it, take a delta-time into account to equalise the speed across variable
//  framerates
{
	//  First turn as indicated by right analogue stick
	direction.Rotate(pad[0].axes[2]*timeDif * sensitivity);
	direction.NormaliseSelf();
	
	//  Find the direction vector we wish to strafe along
	Vector2 strafeDirection = direction;
	strafeDirection.Rotate(90);
	
	//  We want to go slower backwards, so create a multiplier that halves reverse speed
	float reverseMult;
	if (pad[0].axes[1] > 0) {
		reverseMult = 0.5f;
	} else {
		reverseMult = 1.0f;
	}
	
	//  Sum up our translations		unit direction			forward/backwards component							unit strafe					stafe component
	Vector2 translationVector = direction * (-pad[0].axes[1] * timeDif * speed * reverseMult) + strafeDirection * (pad[0].axes[0] * timeDif * speed);

	//  Check for the player firing their weapon
	firedWeapon = false;
	if ((pad[0].pressed & PAD_L2) && !activeWeapon->IsFiring() && activeWeapon->HasAmmo()) {
		firedWeapon = true;
		activeWeapon->Fire();
		if (activeWeapon == &pistol) {
			if (rumbleIntensity < 10) {
				rumbleIntensity = 10;		//  Tiny bit of rumble on pistol shot
			}
		} else {
			if (rumbleIntensity < 15) {
				rumbleIntensity = 15;		//  bit more rumble on shotgun shot
			}			
		}
	}
	
	//  Decrease rumble over time
	if(rumbleIntensity > 0) {
		rumbleIntensity--;
	}
	
	//  Set the new rumble
	set_actuator(0,rumbleIntensity,rumbleIntensity);

	
	//  Reload the weapon on pressing square only if weapon is not already animating
	if ((pad[0].pressed & PAD_SQUARE) && activeWeapon != &pistol && !activeWeapon->IsFiring()) {
		activeWeapon->Reload();
	}
	
	//  Switch weapon
	if ((pad[0].pressed & PAD_L1) && !activeWeapon->IsFiring()) {
		if (activeWeapon == &pistol) {
			activeWeapon = &shotgun;
			ammo.SetUVs(0,0,32,32);
		} else {
			activeWeapon = &pistol;
			ammo.SetUVs(0,32,64,32);
		}
	}	

	//  Decompose movement into individual components
	float x, y;
	float xCollisionBuffer;
	float yCollisionBuffer;
	
	//  Positions used to check collision are the base + translation +/- 0.2 (direction of travel) to stop intersection of 
	//  projection screen and level
	x = position.x + (translationVector.x);
	xCollisionBuffer = (translationVector.x/Abs(translationVector.x))*0.2;
	y = position.y + (translationVector.y);
	yCollisionBuffer = (translationVector.y/Abs(translationVector.y))*0.2;
	
	//  Check collision of player and level

	if (level->At(x + xCollisionBuffer, position.y) == 0) {
		position.x += translationVector.x;
	}
	
	if (level->At(position.x, y + yCollisionBuffer) == 0) {
		position.y += translationVector.y;
	}
	
	//  Update the weapon animation
	activeWeapon->Update();
	
	//  Update the health bar
	UpdateHealthBar();
}

void Player::Render()
//  Draw assets and HUD to screen
{
	activeWeapon->Render();
	healthBar.Render();
	
	TexManager.UploadTextureToBuffer(ammoImg, TextureManager::BUFFER1);
	TexManager.SetTexture(ammoImg);
	
	//  Draw ammo counter if useing shotgun
	if (activeWeapon == &shotgun) {
		ammo.SetWidth(64);
		for (int i = 0; i < shotgun.GetAmmo(); i++) {
			ammo.MoveTo(66+(i*64), 192);
			ammo.Render();
		}
	//  Otherwise we have infinite ammo	
	} else {
		ammo.SetWidth(128);
		ammo.MoveTo(66+64, 192);
		ammo.Render();
	}
}

bool Player::HasFired()
//  function used by playstate to check for player firing weapon
{
	return firedWeapon;
}

int Player::GetGunMaxDamage()
//  Find the damage of the currently used gun
{
	return activeWeapon->GetDamage();
}

void Player::Damage(int damage)
//  Hurt the player and rumble to indicate
{
	hitpoints -= damage;
	rumbleIntensity = 180;
}

bool Player::IsDead()
{
	if (hitpoints <= 0)
		return true;
	return false;
}

Vector2 Player::GetPosition()
{
	return position;
}

Vector2 Player::GetDirection()
{
	return direction;
}

void Player::UpdateHealthBar()
//  Changes the width and colour of the healthbar based on current HP as
//  a proportion of max
{
	float portion = float(hitpoints)/float(MAXHP);
	healthBar.SetWidth(portion * 256.0f);
	
	//  More red as more damage is taken
	int redness = int(0xFF * (1-portion));
	//  Less green as more damage is taken
	int greenness = int(0xFF * portion);
	
	//  Add the colours up in channels
	int colour = (redness << 16) | (greenness << 8) | (0x00);
	
	//  set the colour
	healthBar.SetColour(colour);

}