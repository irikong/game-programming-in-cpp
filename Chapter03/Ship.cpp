// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Ship.h"
#include "SpriteComponent.h"
#include "InputComponent.h"
#include "CircleComponent.h"
#include "Game.h"
#include "Asteroid.h"
#include "Laser.h"

Ship::Ship(Game* game)
	:Actor(game)
	,mLaserCooldown(0.0f)
	,mRespawnCooldown(0.0f)
	,mIsAlive(true)
{
	// Create a sprite component
	mSC = new SpriteComponent(this, 150);
	mSC->SetTexture(game->GetTexture("Assets/Ship.png"));

	// Create an input component and set keys/speed
	mIC = new InputComponent(this);
	mIC->SetForwardKey(SDL_SCANCODE_W);
	mIC->SetBackKey(SDL_SCANCODE_S);
	mIC->SetClockwiseKey(SDL_SCANCODE_A);
	mIC->SetCounterClockwiseKey(SDL_SCANCODE_D);
	mIC->SetMaxForwardSpeed(300.0f);
	mIC->SetMaxAngularSpeed(Math::TwoPi);

	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.0f);
}

void Ship::UpdateActor(float deltaTime)
{
	if (!mIsAlive) {
		mRespawnCooldown -= deltaTime;
		if (mRespawnCooldown < 0.0f) {
			mIsAlive = true;
			SetPosition(Vector2(1024 >> 1, 768 >> 1));
			SetRotation(Math::PiOver2);
			mIC->SetVelocity(Vector2(0.0f, 0.0f));
			mSC->SetIsVisible(true);
		}

		return;
	}

	mLaserCooldown -= deltaTime;

	for (auto ast : GetGame()->GetAsteroids()) {
		if (Intersect(*mCircle, *(ast->GetCircle()))) {
			mSC->SetIsVisible(false);
			mIsAlive = false;
			mRespawnCooldown = 2.0f;
			ast->SetState(EDead);
			break;
		}
	}
}

void Ship::ActorInput(const uint8_t* keyState)
{
	if (mIsAlive && keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f)
	{
		// Create a laser and set its position/rotation to mine
		Laser* laser = new Laser(GetGame(), this);

		// Reset laser cooldown (half second)
		mLaserCooldown = 0.5f;
	}
}
