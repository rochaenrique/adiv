#pragma once
#include <raylib.h>
#include <memory>
#include <vector>
#include <unordered_map>
#include "ecs/ECS.h"
#include "ecs/components/Sprite.h"
#include "ecs/components/Camera.h"
#include "ecs/components/Animator.h"
#include "Animation.h"
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
  
  TexturePack(const std::string& file, Vector2 grid, float scale)
	: filename{file}, grid{grid}, scale{scale}, texture{nullptr}
  {}
  
  const std::string filename;
  Vector2 grid;
  float scale;
  std::shared_ptr<Texture2D> texture;

  bool IsLoaded() const { return texture != nullptr; };
};
class Level
{
public:
  Level();
    
  void Load();
  void Unload();

  Map& GetMap() { return m_Map; }
  adv::Animator& GetPlayerAnimator() { return m_PlayerAnimator; }

  void AddTexturePack(const std::string&, Vector2, float, TextureType);
  void LoadTextures();
  void UnloadTextures();

  const adv::Camera& GetCamera() const { return m_Camera; }
  adv::Animation& GetAnimation() { return m_PreAnim; }

  bool IsLoaded() const { return m_Loaded; };
  
private:
  Map m_Map;
  adv::Sprite m_PlayerSprite;
  adv::Sprite m_TileSprite;
  adv::Sprite m_FlagSprite;
  adv::Animator m_PlayerAnimator;
  adv::Animation m_PreAnim = adv::Animation({{ 3.0f, 0.0f, 3.0f }});
  bool m_Loaded = false;
  
  adv::Camera m_Camera;
  std::vector<Entity> m_Entities {};
  std::unordered_map<TextureType, TexturePack> m_TexturePacks;
};
