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

#define FRAMES_TO_CHANGE 5

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
  Level(const std::string&);

  bool AttemptLoad();
  void Load();
  void Unload();

  Map& GetMap() { return m_Map; }
  adv::Animator& GetPlayerAnimator() { return m_PlayerAnimator; }

  void AddTexturePack(const std::string&, Vector2, float, TextureType);
  void LoadTextures();
  void UnloadTextures();

  const adv::Camera& GetCamera() const { return m_Camera; }
  
private:
  Map m_Map;
  adv::Sprite m_PlayerSprite;
  adv::Sprite m_TileSprite;
  adv::Sprite m_FlagSprite;
  adv::Animator m_PlayerAnimator;
  size_t m_ChangeCounter = FRAMES_TO_CHANGE;
  
  adv::Camera m_Camera;
  const std::string m_Name;
  std::vector<Entity> m_Entities {};
  std::unordered_map<TextureType, TexturePack> m_TexturePacks;
};
