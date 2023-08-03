#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

class ColliderComponent;

class Game
{
public:
	Game();
	~Game();

	void Init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen);

	void HandleEvents();
	void Update();
	void Render();
	void Clean();

	bool IsRunning() { return isRunning; }

	static void AddTile(int srcx, int srcY, int xPos, int yPos);
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static std::vector<ColliderComponent*> colliders;
	static bool isRunning;
	static SDL_Rect camera;

private:
	int cnt = 0;
	SDL_Window* m_window;
};

