#include "Level.h"
#include <cassert>
#include <raymath.h>
#include "Game.h"
#include "ecs/Registry.h"
#include "ecs/components/SpriteAnimation.h"
#include "Window.h"
#include "Event.h"
#include "EventManager.h"

Level::Level(const std::string& name)
  : m_Camera{adv::Camera()}, m_Name{name}
{}

bool Level::AttemptLoad()
{
  bool res;
  if ((res = (m_ChangeCounter == 0))) Load();
  else m_ChangeCounter--;
  
  return res;
}

extern Registry registry;

void Level::Load()
{
  LoadTextures();
  Vector2 cellSize = { Window::GetWidth() / m_Map.grid.x, Window::GetHeight() / m_Map.grid.y };

  //player
  Vector2 playerPos = { m_Map.playerInitialPos.x * cellSize.x, (m_Map.grid.y - m_Map.playerInitialPos.y) * cellSize.y };
  Vector2 playerSize = m_PlayerSprite.GetSize() * m_PlayerSprite.GetScale();
  m_Camera.mapWidth = cellSize.x * m_Map.width;
  m_Camera.camera->offset = Window::GetCenter();
  
  m_Entities.push_back(Game::CreatePlayer(m_PlayerSprite, playerSize, m_Camera, playerPos, m_PlayerAnimator));

  //flag
  adv::Collider flagCollider(cellSize, true);
  flagCollider.SetCollisionCallback([](const adv::Collision&, float) {
	EventManager::Get().Emit<CheckPointEvent>();
  });
  Entity flag = Game::CreateTile(m_FlagSprite, Tile{ m_Map.flagPos, 0, 0 }, m_Map.grid, cellSize, flagCollider);
  
  adv::SpriteAnimation animator;
  animator.Insert(1, adv::Animation({{ 0.0f, 6.0f, 1.5f }})); 
  animator.ChangeTo(1);
  registry.AddComponent(flag, animator);
  m_Entities.push_back(flag);

  //tiles  
  for (const Tile& tile : m_Map.tiles) 
	m_Entities.push_back(Game::CreateTile(m_TileSprite, tile, m_Map.grid, cellSize, adv::Collider(cellSize, true)));


  std::cout << "Loaded " << m_Entities.size() << " entities\n";
}

void Level::Unload()
{
  for (const Entity e : m_Entities)
	registry.DestroyEntity(e);
  
  m_Entities.clear();
  UnloadTextures();
  std::cout << "Unloaded level\n";
}

void Level::AddTexturePack(const std::string& file, Vector2 grid, float scale, const TextureType type)
{
  m_TexturePacks.insert(std::pair(type, TexturePack(file, grid, scale)));
}

void Level::LoadTextures()
{
  for (auto& [type, tp] : m_TexturePacks) 
	tp.texture = std::make_shared<Texture2D>(LoadTexture(tp.filename.c_str()));

  assert(m_TexturePacks.size() == 3);
  
  TexturePack& playerTex = m_TexturePacks[TextureType::PLAYER];
  m_PlayerSprite = { playerTex.texture, playerTex.grid, 0, playerTex.scale };

  TexturePack& tileTex = m_TexturePacks[TextureType::TILE];
  m_TileSprite = { tileTex.texture, tileTex.grid, 0, tileTex.scale };

  TexturePack& flagTex = m_TexturePacks[TextureType::FLAG];
  m_FlagSprite = { flagTex.texture, flagTex.grid, 0, flagTex.scale };
}

void Level::UnloadTextures()
{
  for (auto& [type, tp] : m_TexturePacks) 
	if (tp.IsLoaded() && tp.texture.use_count() == 1) {
	  UnloadTexture(*tp.texture);
	  tp.texture.reset();
	}
}
