#include "Level.h"
#include <raymath.h>
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
  Vector2 size = { Window::GetWidth() / m_Map.grid.x, Window::GetHeight() / m_Map.grid.y };
  m_Camera.mapWidth = size.x * m_Map.width;
  m_Camera.camera->offset = Window::GetCenter();

  Vector2 worldPos = {0, 0};
  for (const Tile& tile : m_Map.tiles) {
	if (tile.pos.x >= worldPos.x) 
	  worldPos.x = tile.pos.x;
	m_Entities.push_back(Game::CreateTile(m_TileSprite,
										  tile,
										  m_Map.grid,
										  size,
										  adv::Collider(size, true)));
  }

  worldPos -= m_Map.playerInitialPos;
  m_Entities.push_back(Game::CreatePlayer(m_PlayerSprite, m_Camera, { worldPos.x * size.x, worldPos.y * size.y }));

    adv::Collider flagCollider(size, true);
  flagCollider.SetCollisionCallback([](const adv::Collision&, float) {
	//EventManager::Get().Emit<CheckPointEvent>();
  });

  // flag should be placed at the end (left most)
  Entity flag = Game::CreateTile(m_FlagSprite, Tile{ m_Map.flagPos, 0, 0 }, size, m_Map.grid, flagCollider);
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
