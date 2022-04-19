// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AIState.h"
#include "AIComponent.h"
#include "Actor.h"
#include "Enemy.h"
#include "Game.h"
#include "Math.h"
#include "Tower.h"
#include <SDL/SDL_log.h>

void AIPatrol::Update(float deltaTime)
{
	mTower->SetRotation(mTower->GetRotation() + Math::Pi * deltaTime);

	Enemy* e = mTower->GetGame()->GetNearestEnemy(mTower->GetPosition());
	if (e) {
		float dist = (e->GetPosition() - mTower->GetPosition()).Length();
		if (dist < mTower->GetAttackRange()) {
			mTower->SetTarget(e);
			mOwner->ChangeState("Attack");
		}
	}
}

void AIPatrol::OnEnter()
{
	SDL_Log("Entering %s state", GetName());
}

void AIPatrol::OnExit()
{
	SDL_Log("Exiting %s state", GetName());
}

void AIDeath::Update(float deltaTime)
{
	SDL_Log("Updating %s state", GetName());
}

void AIDeath::OnEnter()
{
	SDL_Log("Entering %s state", GetName());
}

void AIDeath::OnExit()
{
	SDL_Log("Exiting %s state", GetName());
}

void AIAttack::Update(float deltaTime)
{
	Enemy* e = mTower->GetTarget();
	if (!e) {
		mOwner->ChangeState("Patrol");
		return;
	}

	Vector2 dir = e->GetPosition() - mTower->GetPosition();
	float dist = dir.Length();
	if (dist < mTower->GetAttackRange()) {
		mTower->SetRotation(Math::Atan2(-dir.y, dir.x));
		mTower->Shoot();
	}
	else {
		mOwner->ChangeState("Patrol");
	}
}

void AIAttack::OnEnter()
{
	SDL_Log("Entering %s state", GetName());
}

void AIAttack::OnExit()
{
	SDL_Log("Exiting %s state", GetName());
}
