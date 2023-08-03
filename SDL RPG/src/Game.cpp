#include "Game.h"

#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0, 0, 800, 640 };

bool Game::isRunning = false;

std::vector<ColliderComponent*> Game::colliders;

auto& player(manager.AddEntity());
auto& wall(manager.AddEntity());

const char* mapFile = "assets/terrain_ss.png";

enum groupLabels : std::size_t
{
	groupMap, 
	groupPlayers, 
	groupEnemies, 
	groupColliders
};

auto& tiles(manager.GetGroup(groupMap));
auto& players(manager.GetGroup(groupPlayers));
auto& enemies(manager.GetGroup(groupEnemies));

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen)
		flags = SDL_WINDOW_FULLSCREEN;

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems initialised!" << std::endl;

		m_window = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
		if (m_window)
		{
			std::cout << "Window Created!" << std::endl;
		}

		renderer = SDL_CreateRenderer(m_window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer Created!" << std::endl;
		}

		isRunning = true;
	}
	else
	{
		isRunning = false;
	}

	map = new Map();
	Map::LoadMap("assets/map.map", 25, 20);
	
	player.AddComponent<TransformComponent>(3);
	player.AddComponent<SpriteComponent>("assets/00-guide.png", true);
	player.AddComponent<KeyboardController>();
	player.AddComponent<ColliderComponent>("player");
	player.AddGroup(groupPlayers);
}

void Game::HandleEvents()
{
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::Update()
{
	manager.Refresh();
	manager.Update();

	camera.x = player.GetComponent<TransformComponent>().position.x - 400;
	camera.y = player.GetComponent<TransformComponent>().position.y - 320;

	if (camera.x < 0)
		camera.x = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;

	if (camera.y < 0)
		camera.y = 0;
	if (camera.y > camera.h)
		camera.y = camera.h;
}

void Game::Render()
{
	SDL_RenderClear(renderer);

	for (auto& e : tiles)
	{
		e->Draw();
	}

	for (auto& e : players)
	{
		e->Draw();
	}

	for (auto& e : enemies)
	{
		e->Draw();
	}

	SDL_RenderPresent(renderer);
}

void Game::Clean()
{
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}

void Game::AddTile(int srcX, int srcY, int xPos, int yPos)
{
	auto& tile(manager.AddEntity());
	tile.AddComponent<TileComponent>(mapFile, srcX, srcY, xPos, yPos);
	tile.AddGroup(groupMap);
}
