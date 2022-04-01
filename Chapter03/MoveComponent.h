// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Component.h"
#include "Math.h"

class MoveComponent : public Component
{
public:
	// Lower update order to update first
	MoveComponent(class Actor* owner, int updateOrder = 10);

	void Update(float deltaTime) override;
	
	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetMass() const { return mMass; }
	Vector2 GetVelocity() const { return mVelocity; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetMass(float mass) { mMass = mass; }
	void SetVelocity(Vector2 velocity) { mVelocity = velocity; }

	void AddForce(Vector2 force) { mSumOfForce += force; }
private:
	// Controls rotation (radians/second)
	float mAngularSpeed;
	float mMass;
	Vector2 mSumOfForce;
	Vector2 mVelocity;
};
