// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"

class Tower : public Actor
{
public:
	Tower(class Game* game);
	void UpdateActor(float deltaTime) override;

	class Enemy* GetTarget() const { return mTarget; }
	void SetTarget(class Enemy* e) { mTarget = e; }
	float GetAttackRange() const { return AttackRange; }

	void Shoot();

private:
	class MoveComponent* mMove;
	class AIComponent* mAIC;
	class Enemy* mTarget;
	float mNextAttack;
	const float AttackTime = 2.5f;
	const float AttackRange = 100.0f;
};
