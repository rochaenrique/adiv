#pragma once
#include <raylib.h>
#include <memory>
#include <vector>
#include <unordered_map>
#include "ecs/ECS.h"
#include "ecs/components/Sprite.h"
#include "ecs/components/Camera.h"
#include <raymath.h>

struct Tile
{
  Vector2 pos;
  size_t texture, spriteIndex;
};

struct Map
{
  Vector2 playerInitialPos, flagPos;
  Vector2 grid;
  size_t width;
  std::vector<Tile> tiles {};
};

enum TextureType { PLAYER, FLAG, TILE };

struct TexturePack
{
  TexturePack() {};
  
  TexturePack(const std::string& file, Vector2 grid)
	: filename{file}, grid{grid}
  {};
  
  const std::string filename;
  Vector2 grid;
  std::shared_ptr<Texture2D> texture = nullptr;

  bool IsLoaded() const { return texture != nullptr; };
};

class Level
{
public:
  Level();
    
  void Load();
  void Unload();

  Map& GetMap() { return m_Map; };

  void AddTexturePack(const std::string&, Vector2, TextureType);
  void LoadTextures();
  void UnloadTextures();

  const adv::Camera& GetCamera() const { return m_Camera; }
  
private:
  Map m_Map;
  adv::Sprite m_PlayerSprite;
  adv::Sprite m_TileSprite;
  adv::Sprite m_FlagSprite;
  
  adv::Camera m_Camera;
  std::vector<Entity> m_Entities {};
  std::unordered_map<TextureType, TexturePack> m_TexturePacks;
};
