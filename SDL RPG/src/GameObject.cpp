#include "GameObject.h"

#include "TextureManager.h"

GameObject::GameObject(const char* textureSheet, int x, int y)
{
	m_objTexture = TextureManager::LoadTexture(textureSheet);
	m_xPos = x;
	m_yPos = y;
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	m_xPos++;
	m_yPos++;

	m_srcRect.w = 16;
	m_srcRect.h = 20;
	m_srcRect.x = 0;
	m_srcRect.y = 0;

	int scale = 4;

	m_destRect.x = m_xPos;
	m_destRect.y = m_yPos;
	m_destRect.w = m_srcRect.w * scale;
	m_destRect.h = m_srcRect.h * scale;
}

void GameObject::Render()
{
	SDL_RenderCopy(Game::renderer, m_objTexture, &m_srcRect, &m_destRect);
}
