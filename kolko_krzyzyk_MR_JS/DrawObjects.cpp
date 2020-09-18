#include "DrawObjects.h"

/*Draw::Draw()
{
}*/


void Draw::DrawString(void* font, const char s[], float x, float y)
{
  unsigned int i;
  glRasterPos2f(x, y);
  for (i = 0; i < strlen(s); i++)
  {
    glutBitmapCharacter(font, s[i]);
  }
}

void Draw::Draw_O(int x, int y, int z, int a)
{
  glPushMatrix();
  glTranslatef(x, y, z);
  glRotatef(a, 1, 0, 0);
  glutSolidTorus(60.0, 60.5, 16, 16);
  glPopMatrix();
}

void Draw::Draw_X(int x, int y)
{
  glLineWidth(10.0f);
  glBegin(GL_LINES);
  glColor3f(0, 0, 0);
  glVertex2f(x - 50, y - 50);
  glVertex2f(x + 50, y + 50);
  glVertex2f(x - 50, y + 50);
  glVertex2f(x + 50, y - 50);
  glEnd();
}

void Draw::Draw_Win(int x1, int y1, int x2, int y2)
{
  glLineWidth(10.0f);
  glBegin(GL_LINES);
  glColor3f(1, 0, 0);
  glVertex2f(x1, y1);
  glVertex2f(x2, y2);
  glColor3f(0, 0, 0);
  glEnd();
}

void Draw::DrawLines()
{
  glLineWidth(4.0f);
  glBegin(GL_LINES);
  glColor3f(0, 0, 0);

  glVertex2f(210, 30);
  glVertex2f(210, 570);
  glVertex2f(390, 30);
  glVertex2f(390, 570);

  glVertex2f(30, 210);
  glVertex2f(570, 210);
  glVertex2f(30, 390);
  glVertex2f(570, 390);

  glEnd();
}
