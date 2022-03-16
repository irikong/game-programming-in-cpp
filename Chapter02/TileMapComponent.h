#pragma once
#include "SpriteComponent.h"
#include "Math.h"
#include <vector>
#include <string>

class TileMapComponent : public SpriteComponent
{
public:
	TileMapComponent(class Actor* owner, int drawOrder = 10);
	void Draw(SDL_Renderer* renderer) override;
	void SetTMTextures(SDL_Texture* texture, const std::string& fileName);

private:
	struct TMTexture
	{
		int idx;
		int posX, posY;

		TMTexture(int _idx, int _x, int _y) : idx(_idx), posX(_x), posY(_y) {}
	};
	std::vector<TMTexture> mTMTextures;

	const int MAP_ROW = 24;
	const int MAP_COL = 32;
	// TILES INFO
	const int TILE_WIDTH = 32;
	const int TILE_HEIGHT = 32;
	const int TILES_PER_ROW = 8;
	const int TILES_PER_COL = 24;
};
