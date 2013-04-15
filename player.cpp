#include "player.h"

#include "level.h"

#include "PS2Defines.h"
#include "pad.h"

#include <time.h>

Player::Player()
{}

Player::~Player()
{}


void Player::Initialise(Vector2 pos, Vector2 dir, Level& lev)
{
	position = pos;
	direction = dir;
	level = &lev;
	hitpoints = 100;
	
	pistol.Init();
	shotgun.Init();
	activeWeapon = &pistol;
}

void Player::Update(double timeDif)
{
	direction.Rotate(pad[0].axes[2]*timeDif * 4.0f);
	Vector2 strafeDirection = direction;
	strafeDirection.Rotate(90);
	
	Vector2 translationVector = direction.Normalise() * (-pad[0].axes[1] * timeDif * 3.0f) + strafeDirection.Normalise() * (pad[0].axes[0] * timeDif * 3.0f);
	float x, y;
	x = position.x + (translationVector.x) + (translationVector.x/Abs(translationVector.x))*0.2;
	y = position.y + (translationVector.y) + (translationVector.y/Abs(translationVector.y))*0.2;

	firedWeapon = false;
	if ((pad[0].pressed & PAD_L2) && !activeWeapon->IsFiring() && activeWeapon->HasAmmo()) {
		firedWeapon = true;
		activeWeapon->Fire();
	}
	
	if ((pad[0].pressed & PAD_SQUARE) && activeWeapon != &pistol && !activeWeapon->IsFiring()) {
		activeWeapon->Reload();
	}
	
	if ((pad[0].pressed & PAD_L1) && !activeWeapon->IsFiring()) {
		if (activeWeapon == &pistol) {
			activeWeapon = &shotgun;
		} else {
			activeWeapon = &pistol;
		}
	}	
	
	if (level->At(x, position.y) == 0) {
		position.x += translationVector.x;
	}
	
	if (level->At(position.x, y) == 0) {
		position.y += translationVector.y;
	}
	activeWeapon->Update();
}

void Player::Render()
{
	activeWeapon->Render();
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