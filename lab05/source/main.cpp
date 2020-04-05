#include "res.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <time.h>
#include <cmath>
#include <math.h>
#include <cstdlib>
#include <ctime>

using namespace std;
int x1, x2 = 0;




LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  static int xClick;
  static int yClick;

  switch (message)
  {
  case WM_INITDIALOG:
    return FALSE;

  case WM_CREATE:
    break;

  case WM_PAINT:
    HDC hDC;
    hDC = GetDC(hwnd);
    BitBlt(hDC, 0, 0, 800, 600, GetDC(0), x1, x2, SRCCOPY);
    SelectObject(hDC, GetStockObject(ANSI_VAR_FONT));
    SetTextColor(hDC, RGB(255, 0, 0));
    TextOut(hDC, 0, 0, TEXT("Text output to client area."), 27);
    ReleaseDC(hwnd, hDC);
    break;

  case WM_MOVE:
  {
    x1 = LOWORD(lParam);
    x2 = HIWORD(lParam);

    return 0;
  }
  break;


  case WM_CLOSE:
    DestroyWindow(hwnd); // destroying window
    PostQuitMessage(0); // put in the loop information about ending application
    return TRUE;

    return TRUE;
  }
  return FALSE;
}




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  HWND hwndMainWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, WndProc);
  ShowWindow(hwndMainWindow, iCmdShow);


  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}