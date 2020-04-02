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

HINSTANCE g_hInstance;
HINSTANCE l_hInstance;
bool if_close = false;

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  string opis_kajko = "Niski, bardzo inteligentny i odwa�ny. Najlepszy przyjaciel Kokosza. Uosobienie cech pozytywnych, cho� od czasu do czasu, podobnie jak Kokosz, ulega r�nym pokusom (np.w Skarbach Mirmi�a wraz z Kokoszem ukrad� magiczn� ma�� Jagi). On zazwyczaj si� zastanawia nad napraw� wszelkich sytuacji, przyk�adowo wtedy, gdy Mirmi� traci wiar� w siebie.W pierwszych historiach nosi� miecz. Umie tak�e strzela� bardzo zr�cznie z �uku a tak�e umiej�tnie w�ada mieczem.Czasem bywa na Kokosza z�y za to, �e wyjada on mi�sne potrawy czy te� �e ma ich wi�cej ni� on.";

  switch (uMsg)
  {
    case WM_INITDIALOG:
    {
      HICON hIcon = LoadIconA(l_hInstance, MAKEINTRESOURCE(IDI_KAJKO));
      SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

      HMENU hMenu = LoadMenu(g_hInstance, MAKEINTRESOURCE(IDR_KAJKO));
      SetMenu(hwndDlg, hMenu);
      return FALSE;
    }

    case WM_COMMAND:
      switch (HIWORD(wParam))
      {
        case BN_CLICKED:
          switch (LOWORD(wParam))
          {
            case IDC_KAJKO:
              MessageBoxA(0, opis_kajko.c_str(), "Kajko - wikipedia", MB_OK);

            return TRUE;
          }
        }

      return FALSE;

    
    case WM_CLOSE:

      if (if_close == true) {
        DestroyWindow(hwndDlg);
        PostQuitMessage(0);
        return TRUE;
      }
      else
      {
        DestroyWindow(hwndDlg);
        if_close = true;
      }

    return TRUE;
  }
  return FALSE;
}

INT_PTR CALLBACK DialogProc2(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  string opis_kokosz = "Do�� t�gi i �ysy przyjaciel Kajka. Lubi du�o je��, jest r�wnie� do�� tch�rzliwy, a tak�e przes�dny (np. w Z�otym Pucharze wierzy, �e jego si�a zale�y od d�ugo�ci jego cienia); nie lubi, gdy mu si� przeszkadza podczas posi�ku gro��c innym �e doprowadzi to do r�koczyn�w. Mia� wuja, kt�ry by� w�a�cicielem karczmy, jednak�e od albumu �Woje Mirmi�a� �amignat zosta� �podmieniony� jako cz�onek rodziny bohatera. Kokosz cz�sto je, wypija eliksiry i napoje, kt�rych nie wolno kosztowa�, co wywo�uje u niego zmian� podej�cia do otoczenia.";

  switch (uMsg)
  {
    case WM_INITDIALOG:
    {

      HICON hIcon = LoadIconA(l_hInstance, MAKEINTRESOURCE(IDI_KOKOSZ));
      SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

      HMENU hMenu = LoadMenu(l_hInstance, MAKEINTRESOURCE(IDR_KOKOSZ));
      SetMenu(hwndDlg, hMenu);

      return FALSE;
    }

    case WM_COMMAND:
      switch (HIWORD(wParam))
      {
        case BN_CLICKED:
          switch (LOWORD(wParam))
          {
          case IDC_KOKOSZ:
            MessageBoxA(0, opis_kokosz.c_str(), "Kokosz - wikipedia", MB_OK);

            return TRUE;
          }
      }

     return FALSE;

    case WM_CLOSE:
      if (if_close == true) {
        DestroyWindow(hwndDlg);
        PostQuitMessage(0);
        return TRUE;
      }
      else
      {
        DestroyWindow(hwndDlg);
        if_close = true;
      }

     return TRUE;
  }
  return FALSE;
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  g_hInstance = hInstance;
  l_hInstance = hInstance;

  HWND hwndMainWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_KAJKOMENU), NULL, DialogProc);
  ShowWindow(hwndMainWindow, iCmdShow);

  HWND hwndMainWindow2 = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_KOKOSZMENU), NULL, DialogProc2);
  ShowWindow(hwndMainWindow2, iCmdShow);

  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}
