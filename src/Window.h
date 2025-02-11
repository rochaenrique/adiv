#pragma once
#include <string>
#include <raylib.h>
#include <raymath.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

struct WindowOptions
{
  WindowOptions()
	: Width{WINDOW_WIDTH},
	  Height{WINDOW_HEIGHT},
	  Title{"Adiv"}
  {};

  WindowOptions(unsigned w, unsigned h, const std::string& t)
	: Width{w},
	  Height{h},
	  Title{t}
  {};
  
  unsigned Width, Height;
  const std::string Title;
};

class Window
{
public:
  Window() : m_Data{WindowOptions()} {};
  Window(const WindowOptions& opt) : m_Data{opt} {};
  
  ~Window();

  static Window* Create(WindowOptions opt = WindowOptions());
  void Init();

  int		static GetWidth()	{ return GetScreenWidth(); }
  int		static GetHeight()  { return GetScreenHeight(); }
  Vector2	static GetSize()	{ return { (float)GetScreenWidth(), (float)GetScreenHeight() }; }
  Vector2	static GetCenter()  { return GetSize() / 2.0f; };
private:
  WindowOptions m_Data;
};
