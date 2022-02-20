// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
	:mWindow(nullptr)
	, mRenderer(nullptr)
	, mTicksCount(0)
	, mIsRunning(true)
	, mLPaddleDir(0)
{

}

bool Game::Initialize()
{
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", // Window title
		100,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
		1024,	// Width of window
		768,	// Height of window
		0		// Flags (0 for no flags set)
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	//
	mLPaddlePos.x = 10.0f;
	mLPaddlePos.y = 768.0f / 2.0f;
	mRPaddlePos.x = 1024.0f - 10.0f - thickness;
	mRPaddlePos.y = 768.0f / 2.0f;
	balls.emplace_back(Ball{ 1024.0f / 2.0f, 768.0f / 2.0f, -200.0f, 235.0f });
	balls.emplace_back(Ball{ 1024.0f / 2.0f, 768.0f / 2.0f, 250.0f, 235.0f });

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// If we get an SDL_QUIT event, end loop
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	// Update paddle direction based on W/S keys
	mLPaddleDir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mLPaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mLPaddleDir += 1;
	}

	mRPaddleDir = 0;
	if (state[SDL_SCANCODE_I])
	{
		mRPaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_K])
	{
		mRPaddleDir += 1;
	}
}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();

	// Update paddle position based on direction
	if (mLPaddleDir != 0)
	{
		mLPaddlePos.y += mLPaddleDir * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (mLPaddlePos.y < (paddleH / 2.0f + thickness))
		{
			mLPaddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (mLPaddlePos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			mLPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	if (mRPaddleDir != 0)
	{
		mRPaddlePos.y += mRPaddleDir * 300.0f * deltaTime;
		if (mRPaddlePos.y < (paddleH / 2.0f + thickness))
		{
			mRPaddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (mRPaddlePos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			mRPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	for (Ball& ball : balls) {
		// Update ball position based on ball velocity
		ball.pos.x += ball.vel.x * deltaTime;
		ball.pos.y += ball.vel.y * deltaTime;

		// Bounce if needed
		// Did we intersect with the paddle?
		Vector2 paddlePos;
		paddlePos.x = (ball.vel.x < 0.0f ? mLPaddlePos.x + thickness : mRPaddlePos.x - thickness / 2.0f);
		paddlePos.y = (ball.vel.x < 0.0f ? mLPaddlePos.y : mRPaddlePos.y);

		float diff = paddlePos.y - ball.pos.y;
		diff = (0.0f < diff) ? diff : -diff;

		if (diff <= (paddleH / 2.0f) && paddlePos.x <= ball.pos.x && ball.pos.x <= (paddlePos.x + thickness / 2.0f)) {
			ball.vel.x *= -1;
		}

		// Did the ball go off the screen? (if so, end game)
		if (ball.pos.x <= 0.0f || ball.pos.x >= 1024.0f)
		{
			mIsRunning = false;
		}

		// Did the ball collide with the top wall?
		if (ball.pos.y <= thickness && ball.vel.y < 0.0f)
		{
			ball.vel.y *= -1;
		}
		// Did the ball collide with the bottom wall?
		else if (ball.pos.y >= (768 - thickness) &&
			ball.vel.y > 0.0f)
		{
			ball.vel.y *= -1;
		}
	}
}

void Game::GenerateOutput()
{
	// Set draw color to blue
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G 
		255,	// B
		255		// A
	);

	// Clear back buffer
	SDL_RenderClear(mRenderer);

	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	// Draw top wall
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1024,		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw paddle
	SDL_Rect lPaddle{
		static_cast<int>(mLPaddlePos.x),
		static_cast<int>(mLPaddlePos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &lPaddle);
	SDL_Rect rPaddle{
		static_cast<int>(mRPaddlePos.x),
		static_cast<int>(mRPaddlePos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &rPaddle);

	// Draw ball
	for (const Ball& ball : balls) {
		SDL_Rect rectBall{
		static_cast<int>(ball.pos.x - thickness / 2),
		static_cast<int>(ball.pos.y - thickness / 2),
		thickness,
		thickness
		};
		SDL_RenderFillRect(mRenderer, &rectBall);
	}

	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}
