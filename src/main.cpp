#include <iostream>
#include <raylib.h>

#define SCREEN_WIDTH  800.0f
#define SCREEN_HEIGHT 600.0f

int main()
{
  std::cout << "Hello world\n";
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ADIV");

  Texture2D texture = LoadTexture("res/sprites/old_man.jpeg");
  float sprite_size = (float)texture.width / 8.0f;
  Rectangle source = { 0, 0, sprite_size, sprite_size };
  Rectangle dest = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, sprite_size / 2.0f, sprite_size / 2.0f };
  
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
	ClearBackground(WHITE);
	BeginDrawing();
	DrawTexturePro(texture, source, dest, {0, 0}, 0, WHITE);
	EndDrawing();

	if (IsKeyDown(KEY_C))
	  source.x += texture.width / 8.0f;

	if (IsKeyDown(KEY_D)) {
	  source.x += texture.width / 8.0f;
	  dest.x++;
	}
	if (IsKeyPressed(KEY_A))
	  dest.x++;
	if (IsKeyPressed(KEY_S))
	  dest.y += 10;
	if (IsKeyPressed(KEY_W))
	  dest.y -= 10;

  }
}
