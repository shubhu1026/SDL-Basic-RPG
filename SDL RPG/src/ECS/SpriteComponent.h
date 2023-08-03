#pragma once

#include <SDL2/SDL.h>
#include "Components.h"
#include "../TextureManager.h"
#include "Animation.h"
#include <map>

class SpriteComponent : public Component
{
private:
	TransformComponent* m_transform;
	SDL_Texture* m_texture;
	SDL_Rect m_srcRect, m_destRect;

	bool m_animated = false;
	int m_frames = 0;
	int m_speed = 100;

public:
	int animIndex = 0;

	std::map<const char*, Animation> animations;

	SpriteComponent() = default;

	SpriteComponent(const char* path)
	{
		SetTex(path);
	}

	SpriteComponent(const char* path, bool isAnimated)
	{
		m_animated = isAnimated;

		Animation down = Animation(0, 3, 150);
		Animation left = Animation(1, 3, 150);
		Animation right = Animation(2, 3, 150);
		Animation up = Animation(3, 3, 150);
		Animation idleDown = Animation(0, 1, 150);
		Animation idleLeft = Animation(1, 1, 150);
		Animation idleRight = Animation(2, 1, 150);
		Animation idleUp = Animation(3, 1, 150);

		animations.emplace("Down", down);
		animations.emplace("Left", left);
		animations.emplace("Right", right);
		animations.emplace("Up", up);
		animations.emplace("IdleDown", idleDown);
		animations.emplace("IdleLeft", idleLeft);
		animations.emplace("IdleRight", idleRight);
		animations.emplace("IdleUp", idleUp);

		PlayAnimation("IdleDown");
		SetTex(path);
	}

	~SpriteComponent()
	{
		SDL_DestroyTexture(m_texture);
	}

	void SetTex(const char* path)
	{
		m_texture = TextureManager::LoadTexture(path);
	}

	void Init() override
	{
		m_transform = &entity->GetComponent<TransformComponent>();

		m_srcRect.x = m_srcRect.y = 0;
		m_srcRect.w = m_transform->width;
		m_srcRect.h = m_transform->height;
	}

	void Update() override
	{
		if (m_animated)
		{
			m_srcRect.x = m_srcRect.w * static_cast<int>((SDL_GetTicks() / m_speed) % m_frames);
		}

		m_srcRect.y = animIndex * m_transform->height;

		m_destRect.x = static_cast<int>(m_transform->position.x) - Game::camera.x;
		m_destRect.y = static_cast<int>(m_transform->position.y) - Game::camera.y;
		m_destRect.w = m_transform->width * m_transform->scale;
		m_destRect.h = m_transform->height * m_transform->scale;
	}

	void Draw() override
	{
		TextureManager::Draw(m_texture, m_srcRect, m_destRect);
	}

	void PlayAnimation(const char* animName)
	{
		m_frames = animations[animName].frames;
		animIndex = animations[animName].index;
		m_speed = animations[animName].speed;
	}
};
