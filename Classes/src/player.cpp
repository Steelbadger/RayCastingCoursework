#include "player.h"

#include "level.h"

#include "PS2Defines.h"
#include "pad.h"
#include "texturemanager.h"

#include <time.h>

Player::Player():
	healthBar(0, -240, 256, 32),
	ammo(66, 192, 64, 64),
	ammoImg("ammo.bmp"),
	MAXHP(100)	
{}

Player::~Player()
{}


void Player::Initialise(Vector2 pos, Vector2 dir, Level& lev)
{
	position = pos;
	direction = dir;
	level = &lev;
	hitpoints = MAXHP;
	
	pistol.Init();
	shotgun.Init();
	activeWeapon = &pistol;
	speed = 3.0f;
	sensitivity = 4.0f;
	healthBar.SetColour(0x00FF00);
	healthBar.SetAlpha('l');
	healthBar.SetDepth(850);
	rumbleIntensity = 0;
	ammo.SetUVs(0,32, 64, 32);
	ammo.SetDepth(850);
	TexManager.LoadTexture(ammoImg);
}

void Player::Update(double timeDif)
{
	direction.Rotate(pad[0].axes[2]*timeDif * sensitivity);
	Vector2 strafeDirection = direction;
	strafeDirection.Rotate(90);
	float reverseMult;
	if (pad[0].axes[1] > 0) {
		reverseMult = 0.5f;
	} else {
		reverseMult = 1.0f;
	}
	Vector2 translationVector = direction.Normalise() * (-pad[0].axes[1] * timeDif * speed * reverseMult) + strafeDirection.Normalise() * (pad[0].axes[0] * timeDif * speed);
	float x, y;
	x = position.x + (translationVector.x) + (translationVector.x/Abs(translationVector.x))*0.2;
	y = position.y + (translationVector.y) + (translationVector.y/Abs(translationVector.y))*0.2;

	firedWeapon = false;
	if ((pad[0].pressed & PAD_L2) && !activeWeapon->IsFiring() && activeWeapon->HasAmmo()) {
		firedWeapon = true;
		activeWeapon->Fire();
		if (rumbleIntensity < 50) {
			rumbleIntensity = 50;
		}
	}
	
	if(rumbleIntensity > 0) {
		rumbleIntensity--;
	}
	
	set_actuator(0,0,rumbleIntensity);

	
	if ((pad[0].pressed & PAD_SQUARE) && activeWeapon != &pistol && !activeWeapon->IsFiring()) {
		activeWeapon->Reload();
	}
	
	if ((pad[0].pressed & PAD_L1) && !activeWeapon->IsFiring()) {
		if (activeWeapon == &pistol) {
			activeWeapon = &shotgun;
			ammo.SetUVs(0,0,32,32);
		} else {
			activeWeapon = &pistol;
			ammo.SetUVs(0,32,64,32);
		}
	}	
	
	if (level->At(x, position.y) == 0) {
		position.x += translationVector.x;
	}
	
	if (level->At(position.x, y) == 0) {
		position.y += translationVector.y;
	}
	activeWeapon->Update();
	UpdateHealthBar();
}

void Player::Render()
{
	activeWeapon->Render();
	healthBar.Render();
	
	TexManager.UploadTextureToBuffer(ammoImg, TextureManager::BUFFER1);
	TexManager.SetTexture(ammoImg);
	if (activeWeapon == &shotgun) {
		ammo.SetWidth(64);
		for (int i = 0; i < shotgun.GetAmmo(); i++) {
			ammo.MoveTo(66+(i*64), 192);
			ammo.Render();
		}
	} else {
		ammo.SetWidth(128);
		ammo.MoveTo(66+64, 192);
		ammo.Render();
	}
}

bool Player::HasFired()
{
	return firedWeapon;
}

int Player::GetGunMaxDamage()
{
	return activeWeapon->GetDamage();
}

void Player::Damage(int damage)
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
{
	float portion = float(hitpoints)/float(MAXHP);
	healthBar.SetWidth(portion * 256.0f);
	int redness = int(0xFF * (1-portion));
	int greenness = int(0xFF * portion);
	int colour = (redness << 16) | (greenness << 8) | (0x00);
	
	healthBar.SetColour(colour);

}