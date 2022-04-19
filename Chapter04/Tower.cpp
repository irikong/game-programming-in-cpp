// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Tower.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Enemy.h"
#include "Bullet.h"
#include "AIComponent.h"
#include "AIState.h"

Tower::Tower(class Game* game)
:Actor(game)
{
	SpriteComponent* sc = new SpriteComponent(this, 200);
	sc->SetTexture(game->GetTexture("Assets/Tower.png"));
	
	mMove = new MoveComponent(this);
	//mMove->SetAngularSpeed(Math::Pi);
	
	mNextAttack = AttackTime;

	mAIC = new AIComponent(this);
	mAIC->RegisterState(new AIPatrol(mAIC, this));
	mAIC->RegisterState(new AIAttack(mAIC, this));
	mAIC->ChangeState("Patrol");
}

void Tower::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	
	mNextAttack -= deltaTime;
}

void Tower::Shoot()
{
	if (mNextAttack <= 0.0f) {
		Bullet* b = new Bullet(GetGame());
		b->SetPosition(GetPosition());
		b->SetRotation(GetRotation());
		mNextAttack += AttackTime;
	}
}
