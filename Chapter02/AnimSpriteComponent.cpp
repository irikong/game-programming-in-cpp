// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AnimSpriteComponent.h"
#include "Math.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(24.0f)
	, mCurrAnim("")
{
}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);
	if (mCurrAnim.empty()) return;

	std::vector<SDL_Texture*>& currAnim = mAnimTextures[mCurrAnim].first;
	bool isLoop = mAnimTextures[mCurrAnim].second;

	if (currAnim.size() > 0)
	{
		// Update the current frame based on frame rate
		// and delta time
		mCurrFrame += mAnimFPS * deltaTime;
		// Wrap current frame if needed
		while (mCurrFrame >= mAnimTextures[mCurrAnim].first.size())
		{
			mCurrFrame -= isLoop ? currAnim.size() : 1;
		}

		// Set the current texture
		SetTexture(currAnim[static_cast<int>(mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::string& animName, const std::vector<SDL_Texture*>& textures, bool isLoop)
{
	mAnimTextures[animName].first = textures;
	mAnimTextures[animName].second = isLoop;
}
