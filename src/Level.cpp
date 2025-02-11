#include "Level.h"
#include "Game.h"
#include "ecs/Registry.h"
#include "Window.h"

Level::Level(const Map& map, const adv::Sprite& pSprite, const adv::Sprite& tSprite)
  : m_Map{map}, m_PlayerSprite{pSprite}, m_TileSprite{tSprite}, m_Camera{adv::Camera()}
{}

void Level::Load()
{
  m_Entities.push_back(Game::CreatePlayer(m_PlayerSprite, m_Camera, m_Map.playerInitialPos));
  
  Vector2 size = { Window::GetWidth() / m_Map.grid.x, Window::GetHeight() / m_Map.grid.y };
  for (size_t i = 0; i < m_Map.tiles.size() && i < m_Map.grid.y; i++) 
	for (size_t j = 0; j < m_Map.tiles[i].size() && j < m_Map.grid.x; j++) 
	  m_Entities.push_back(Game::CreateTile(m_TileSprite, m_Map.tiles[i][j], size, i, m_Map.grid.y));

  std::cout << "Loaded " << m_Entities.size() << " entities\n";
}

extern Registry registry;
void Level::UnloadECS()
{
  for (const Entity e : m_Entities)
	registry.DestroyEntity(e);
}
