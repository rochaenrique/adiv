#include "Window.h"

Window* Window::Create(WindowOptions opt)
{
  return new Window(opt);
}

void Window::Init()
{
  InitWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str());
  SetTargetFPS(120);
}

Window::~Window()
{
  CloseWindow();
}
