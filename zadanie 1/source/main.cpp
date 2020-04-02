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
  string opis_kajko = "Niski, bardzo inteligentny i odwa¿ny. Najlepszy przyjaciel Kokosza. Uosobienie cech pozytywnych, choæ od czasu do czasu, podobnie jak Kokosz, ulega ró¿nym pokusom (np.w Skarbach Mirmi³a wraz z Kokoszem ukrad³ magiczn¹ maœæ Jagi). On zazwyczaj siê zastanawia nad napraw¹ wszelkich sytuacji, przyk³adowo wtedy, gdy Mirmi³ traci wiarê w siebie.W pierwszych historiach nosi³ miecz. Umie tak¿e strzelaæ bardzo zrêcznie z ³uku a tak¿e umiejêtnie w³ada mieczem.Czasem bywa na Kokosza z³y za to, ¿e wyjada on miêsne potrawy czy te¿ ¿e ma ich wiêcej ni¿ on.";

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
  string opis_kokosz = "Doœæ têgi i ³ysy przyjaciel Kajka. Lubi du¿o jeœæ, jest równie¿ doœæ tchórzliwy, a tak¿e przes¹dny (np. w Z³otym Pucharze wierzy, ¿e jego si³a zale¿y od d³ugoœci jego cienia); nie lubi, gdy mu siê przeszkadza podczas posi³ku gro¿¹c innym ¿e doprowadzi to do rêkoczynów. Mia³ wuja, który by³ w³aœcicielem karczmy, jednak¿e od albumu „Woje Mirmi³a” £amignat zosta³ „podmieniony” jako cz³onek rodziny bohatera. Kokosz czêsto je, wypija eliksiry i napoje, których nie wolno kosztowaæ, co wywo³uje u niego zmianê podejœcia do otoczenia.";

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
