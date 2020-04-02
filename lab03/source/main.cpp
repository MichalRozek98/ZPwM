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

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
    case WM_COMMAND:
      switch (HIWORD(wParam))
      {
        case BN_CLICKED:
          switch (LOWORD(wParam))
          {
            case IDC_BUTTON1:
              
              HWND hwndEditBox = GetDlgItem(hwndDlg, IDC_EDIT1);
              int iTextLength = GetWindowTextLength(hwndEditBox);
              CHAR szText[500];
              GetWindowText(hwndEditBox, szText, iTextLength + 1);
              string text = szText;

              for (int i = 0; i < text.length(); i++)
              {
                if (isdigit(text[i]) == false)
                {
                  MessageBoxA(0, "Incorrect form of text !", "Application", MB_OK);
                  SetWindowText((HWND)hwndEditBox, "");
                  return FALSE;
                }
              }

              if (stoi(text) < 0 || stoi(text) > 40)
              {
                MessageBoxA(0, "Write number from the range 0-40 !", "Application", MB_OK);
                return FALSE;
              }
              else if (text.empty())
              {
                MessageBoxA(0, "textBox is Empty !", "Application", MB_OK);
                return FALSE;
              }
              
              

              HWND hwndStaticBox1 = GetDlgItem(hwndDlg, IDC_STATIC1);
              HWND hwndStaticBox2 = GetDlgItem(hwndDlg, IDC_STATIC2);
            
              int textNumber = stoi(text);
              int remember = 0;

                steps++;
                SetWindowText((HWND)hwndStaticBox1, ("Steps: " + to_string(steps)).c_str());
                if (textNumber == computer_random)
                {
                  srand((int)time(0));
                  SetWindowText((HWND)hwndStaticBox2, ("Number: " + to_string(computer_random)).c_str());
                  SetWindowText((HWND)hwndEditBox, "");
                  steps = 0;
                  computer_random = rand() % 41;
                }
                else
                {
                  if (computer_random < textNumber) // if number is greater than our
                  {
                    SetWindowText((HWND)hwndStaticBox2, "Your number is smaller");
                  }
                  else if(computer_random > textNumber) // if number is smaller than our
                  {
                    SetWindowText((HWND)hwndStaticBox2, "Your number is greater");
                  }
                }

              return TRUE;
          }
      }

    return FALSE;

    case WM_CLOSE:
      DestroyWindow(hwndDlg); // destroying window
      PostQuitMessage(0); // put in the loop information about ending application
      return TRUE;

    return TRUE;
  }
  return FALSE;
}



int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  srand((int)time(0));
  computer_random = rand() % 41;
  HWND hwndMainWindow = CreateDialog(hinstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  ShowWindow(hwndMainWindow, iCmdShow);
  
  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}