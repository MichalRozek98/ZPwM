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

int computer_random;
int steps = 0;
HINSTANCE g_hInstance;


INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_INITDIALOG:
  {
    HICON hIcon = LoadIconA(g_hInstance, MAKEINTRESOURCE(IDI_DRUGA));
    SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

    HMENU hMenu = LoadMenu(g_hInstance, MAKEINTRESOURCE(IDR_MENU1));
    SetMenu(hwndDlg, hMenu);
    return FALSE;
  }

    case WM_CLOSE:
      DestroyWindow(hwndDlg); // destroying window
      PostQuitMessage(0); // put in the loop information about ending application
      return TRUE;

    return TRUE;
  }
  return FALSE;
}




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  g_hInstance = hInstance;
  HWND hwndMainWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  ShowWindow(hwndMainWindow, iCmdShow);

  HICON LoadIconA(HINSTANCE hInstance, LPCSTR lpIconName);

  
  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}