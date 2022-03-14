#include "Ship.h"
#include "Game.h"
#include "Character.h"

Character::Character(Game* game)
	:Actor(game),
	mRightSpeed(0.0f),
	mDownSpeed(0.0f)
{
	mAsc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/Character01.png"),
		game->GetTexture("Assets/Character02.png"),
		game->GetTexture("Assets/Character03.png"),
		game->GetTexture("Assets/Character04.png"),
		game->GetTexture("Assets/Character05.png"),
		game->GetTexture("Assets/Character06.png")
	};
	mAsc->SetAnimTextures(std::string("walk"), anims, true);
	anims = {
		game->GetTexture("Assets/Character07.png"),
		game->GetTexture("Assets/Character08.png"),
		game->GetTexture("Assets/Character09.png"),
		game->GetTexture("Assets/Character10.png"),
		game->GetTexture("Assets/Character11.png"),
		game->GetTexture("Assets/Character12.png"),
		game->GetTexture("Assets/Character13.png"),
		game->GetTexture("Assets/Character14.png"),
		game->GetTexture("Assets/Character15.png")
	};
	mAsc->SetAnimTextures(std::string("jump"), anims, false);
	anims = {
		game->GetTexture("Assets/Character16.png"),
		game->GetTexture("Assets/Character17.png"),
		game->GetTexture("Assets/Character18.png")
	};
	mAsc->SetAnimTextures(std::string("attack"), anims, false);

	mAsc->SetCurrAnim(std::string("walk"));
}

void Character::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	// Update position based on speeds and delta time
	Vector2 pos = GetPosition();
	pos.x += mRightSpeed * deltaTime;
	pos.y += mDownSpeed * deltaTime;
	// Restrict position to left half of screen
	if (pos.x < 25.0f)
	{
		pos.x = 25.0f;
	}
	else if (pos.x > 500.0f)
	{
		pos.x = 500.0f;
	}
	if (pos.y < 25.0f)
	{
		pos.y = 25.0f;
	}
	else if (pos.y > 743.0f)
	{
		pos.y = 743.0f;
	}
	SetPosition(pos);
}

void Character::ProcessKeyboard(const uint8_t* state)
{
	mRightSpeed = 0.0f;
	mDownSpeed = 0.0f;
	// right/left
	if (state[SDL_SCANCODE_RIGHT])
	{
		mRightSpeed += 250.0f;
	}
	if (state[SDL_SCANCODE_LEFT])
	{
		mRightSpeed -= 250.0f;
	}
	// up/down
	if (state[SDL_SCANCODE_DOWN])
	{
		mDownSpeed += 300.0f;
	}
	if (state[SDL_SCANCODE_UP])
	{
		mDownSpeed -= 300.0f;
	}

	if (state[SDL_SCANCODE_Z])
	{
		mAsc->SetCurrAnim("walk");
	}
	if (state[SDL_SCANCODE_X])
	{
		mAsc->SetCurrAnim("attack");
	}
	if (state[SDL_SCANCODE_C])
	{
		mAsc->SetCurrAnim("jump");
	}
}
