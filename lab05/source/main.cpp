#include "res.h"
#include <iostream>
#include <Windows.h>

HBITMAP hBitmap;
BITMAP bitmapInformation = {};
RECT bitmapRectangle = {};


VOID ResizeWindowRectangle(HWND hwnd, RECT Rectangle)
{
  AdjustWindowRect(&Rectangle, GetWindowLongW(hwnd, GWL_STYLE), TRUE);

  SetWindowPos(hwnd, 0,
    Rectangle.left, Rectangle.top,
    Rectangle.right - Rectangle.left, Rectangle.bottom - Rectangle.top,
    SWP_NOMOVE | SWP_NOZORDER);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
  case WM_INITDIALOG:
    hBitmap = (HBITMAP)LoadImageW(GetModuleHandleW(NULL), L"./../source/mountains.bmp",
      IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObjectW(hBitmap, sizeof(bitmapInformation), &bitmapInformation);
    break;

  case WM_PAINT:
    HDC hDC;
    hDC = GetDC(hwnd);
    /*BitBlt(hDC, 0, 0, 800, 600, GetDC(0), x, y, SRCCOPY);
    SelectObject(hDC, GetStockObject(ANSI_VAR_FONT));
    SetTextColor(hDC, RGB(255, 0, 0));
    TextOut(hDC, 0, 0, TEXT("Text output to client area."), 27);
    ReleaseDC(hwnd, hDC);
    break;*/

    HDC hdcBitmap;
    hdcBitmap = CreateCompatibleDC(hDC);
    SelectObject(hdcBitmap, hBitmap);

    if (TRUE)
    {
      RECT rWndRect;
      GetClientRect(hwnd, &rWndRect);
      StretchBlt(hDC, rWndRect.left, rWndRect.top, rWndRect.right - rWndRect.left, rWndRect.bottom - rWndRect.top,
        hdcBitmap, 0, 0, bitmapInformation.bmWidth, bitmapInformation.bmHeight, SRCCOPY);
    }

    DeleteDC(hdcBitmap);
    ReleaseDC(hwnd, hDC);
    break;

  case WM_SIZING:
  {
    bitmapRectangle = { 0, 0, WMSZ_RIGHT, WMSZ_BOTTOM };
    ResizeWindowRectangle(hwnd, bitmapRectangle);
  }
  break;


  case WM_CLOSE:
    DeleteObject(hBitmap);
    DestroyWindow(hwnd); // destroying window
    PostQuitMessage(0); // put in the loop information about ending application
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