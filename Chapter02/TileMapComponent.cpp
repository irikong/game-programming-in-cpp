#include "TileMapComponent.h"
#include <fstream>

TileMapComponent::TileMapComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
{
}

void TileMapComponent::Draw(SDL_Renderer* renderer)
{
	for (auto& texture : mTMTextures) {
		if (texture.idx == -1) continue;

		SDL_Rect srcRect{(texture.idx % TILES_PER_ROW) * TILE_WIDTH, (texture.idx / TILES_PER_ROW) * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT };
		SDL_Rect dstRect{ texture.posX, texture.posY, TILE_WIDTH, TILE_HEIGHT };

		SDL_RenderCopyEx(
			renderer,
			mTexture,
			&srcRect,
			&dstRect,
			0.0,
			nullptr,
			SDL_FLIP_NONE
		);
	}
}

void TileMapComponent::SetTMTextures(SDL_Texture* texture, const std::string& fileName)
{
	std::fstream fs;
	fs.open(fileName, std::ios::in);

	std::string line;
	while (!fs.eof()) {
		getline(fs, line, ',');

		size_t lf = line.find('\n');
		if (lf != std::string::npos) {
			std::string l(line.substr(0, lf));
			mTMTextures.emplace_back(std::stoi(l), (mTMTextures.size() % MAP_COL) * TILE_WIDTH, (mTMTextures.size() / MAP_COL) * TILE_HEIGHT);
			
			if (lf != line.size() - 1) {
				std::string r(line.substr(lf + 1));
				mTMTextures.emplace_back(std::stoi(r), (mTMTextures.size() % MAP_COL) * TILE_WIDTH, (mTMTextures.size() / MAP_COL) * TILE_HEIGHT);
			}
		}
		else {
			mTMTextures.emplace_back(std::stoi(line), (mTMTextures.size() % MAP_COL) * TILE_WIDTH, (mTMTextures.size() / MAP_COL) * TILE_HEIGHT);
		}
	}

	SetTexture(texture);

	fs.close();
}
