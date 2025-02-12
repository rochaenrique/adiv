#include "Level.h"
#include "Game.h"
#include "ecs/Registry.h"
#include "ecs/components/SpriteAnimation.h"
#include "Window.h"
#include "Event.h"
#include "EventManager.h"

Level::Level(const Map& map, const adv::Sprite& pSprite, const adv::Sprite& tSprite, const adv::Sprite& fSprite)
  : m_Map{map},
	m_PlayerSprite{pSprite}, m_TileSprite{tSprite}, m_FlagSprite{fSprite},
	m_Camera{adv::Camera()}
{}

extern Registry registry;

void Level::Load()
{
  m_Entities.push_back(Game::CreatePlayer(m_PlayerSprite, m_Camera, m_Map.playerInitialPos));


  Vector2 size = { Window::GetWidth() / m_Map.grid.x, Window::GetHeight() / m_Map.grid.y };
  for (size_t i = 0; i < m_Map.tiles.size() && i < m_Map.grid.y; i++) 
	for (size_t j = 0; j < m_Map.tiles[i].size() && j < m_Map.grid.x; j++) 
	  m_Entities.push_back(Game::CreateTile(m_TileSprite, m_Map.tiles[i][j], size, i, m_Map.grid.y, adv::Collider(size)));

  //TODO: TEST
  adv::Collider flagCollider(size.x * .1f, size.y);
  flagCollider.SetCollisionCallback([](const adv::Collision&, float) {
	EventManager::Get().Emit<CheckPointEvent>();
  });
  
  Entity flag = Game::CreateTile(m_FlagSprite, { 1, 5 }, size, 5, m_Map.grid.y, flagCollider);
  adv::Animation anim({
	  {
		.from	  = 0.0f,
		.to		  = 6.0f,
		.duration = 1.5f,
	  }
	});
  
  adv::SpriteAnimation animator;
  animator.Insert(1, anim); 
  animator.ChangeTo(1);
  registry.AddComponent(flag, animator);

  std::cout << "Loaded " << m_Entities.size() << " entities\n";
}

void Level::Unload()
{
  for (const Entity e : m_Entities)
	registry.DestroyEntity(e);
  m_Entities.clear();
}
