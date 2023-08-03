#pragma once

#include "ECS.h"
#include "SDL2/SDL.h"

class TileComponent : public Component
{
public:
	SDL_Texture* tex;
	SDL_Rect srcRect, destRect;
	Vec2 position;

	TileComponent() = default;

	~TileComponent()
	{
		SDL_DestroyTexture(tex);
	}

	TileComponent(const char* path, int srcX, int srcY, int xPos, int yPos)
	{
		tex = TextureManager::LoadTexture(path);

		position.x = xPos;
		position.y = yPos;

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = 32;

		destRect.x = xPos;
		destRect.y = yPos;
		destRect.w = destRect.h = 64;
	}

	void Update() override
	{
		destRect.x = position.x - Game::camera.x;
		destRect.y = position.y - Game::camera.y;
	}

	void Draw() override
	{
		TextureManager::Draw(tex, srcRect, destRect);
	}
};