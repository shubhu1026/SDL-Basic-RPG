#pragma once

#include "../Game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{
public:
	TransformComponent* transform;
	SpriteComponent* sprite;

	void Init() override
	{
		transform = &entity->GetComponent<TransformComponent>();
		sprite = &entity->GetComponent<SpriteComponent>();
	}

	void Update() override
	{
		if (Game::event.type == SDL_KEYDOWN)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				transform->velocity.y = -1;
				sprite->PlayAnimation("Up");
				break;
			case SDLK_a:
				transform->velocity.x = -1;
				sprite->PlayAnimation("Left");
				break;
			case SDLK_s:
				transform->velocity.y = 1;
				sprite->PlayAnimation("Down");
				break;
			case SDLK_d:
				transform->velocity.x = 1;
				sprite->PlayAnimation("Right");
				break;
			default:
				break;
			}
		}

		if (Game::event.type == SDL_KEYUP)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				transform->velocity.y = 0;
				sprite->PlayAnimation("IdleUp");
				break;
			case SDLK_a:
				transform->velocity.x = 0;
				sprite->PlayAnimation("IdleLeft");
				break;
			case SDLK_s:
				transform->velocity.y = 0;
				sprite->PlayAnimation("IdleDown");
				break;
			case SDLK_d:
				transform->velocity.x = 0;
				sprite->PlayAnimation("IdleRight");
				break;
			case SDLK_ESCAPE:
				Game::isRunning = false;
				break;
			default:
				break;
			}
		}
	}
};