#include <iostream>
#include <raylib.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

int main()
{
  std::cout << "Hello world\n";
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ADIV");
  
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
	ClearBackground(RED);
	BeginDrawing();
	EndDrawing();
  }
}
