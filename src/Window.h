#pragma once
#include <string>

#define WINDOW_WIDTH  1600
#define WINDOW_HEIGHT 900

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

  unsigned GetWidth() const { return m_Data.Width; };
  unsigned GetHeight() const { return m_Data.Height; };

private:
  WindowOptions m_Data;
};
