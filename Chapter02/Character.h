#pragma once
#include "Actor.h"
#include "AnimSpriteComponent.h"
class Character : public Actor
{
public:
	Character(class Game* game);
	void UpdateActor(float deltaTime) override;
	void ProcessKeyboard(const uint8_t* state);
	float GetRightSpeed() const { return mRightSpeed; }
	float GetDownSpeed() const { return mDownSpeed; }
private:
	AnimSpriteComponent* mAsc;
	float mRightSpeed;
	float mDownSpeed;
};