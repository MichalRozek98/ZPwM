#pragma once
#include <GL\glew.h>
#include <GL\freeglut.h>

class Draw
{
public:
  Draw() {}

  void DrawString(void* font, const char s[], float x, float y);
  void Draw_O(int x, int y, int z = 60, int a = 0);
  void Draw_X(int x, int y);
  void Draw_Win(int x1, int y1, int x2, int y2);
  void DrawLines();

};
