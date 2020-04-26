#include "res.h"
#include <Windows.h>
#include <cstdio>
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

// zmienne globalne
HBITMAP hBitmap;
BITMAP bitmap_info = {};
RECT bitmap_rect = {};
HINSTANCE hInst;

HWND hWndButton;
HWND hWndButtonSendMessage;
HWND hWndEditMessage;
HWND hWndButtonCreateClient;
SOCKET serverSocket;
HWND hWndButtonCreateServer;
int iResult;
WSADATA wsaData;
bool gNotExit = true;
SOCKET clientSocket;

const int LINES = 30;

char asMessageList[LINES][1024];

typedef struct
{
  HWND hwnd;
  SOCKET socket;
  bool server;
} sData;

struct sockaddr_in siClients[100];
int iCountConnectedClients = 0;

void displayMessageList(HDC hDC)
{
  SelectObject(hDC, GetStockObject(ANSI_VAR_FONT));
  SetTextColor(hDC, RGB(255, 0, 0));
  for (int i = 0; i < LINES; i++)
    TextOut(hDC, 0, 70 + i * 20, asMessageList[i], strlen(asMessageList[i]));
}

// metoda umozliwiajaca zmiane rozmiarow okna
VOID ResizeWindowRect(HWND hwnd, RECT Rect)
{
  AdjustWindowRect(&Rect, GetWindowLongW(hwnd, GWL_STYLE), TRUE);

  SetWindowPos(hwnd, 0,
    Rect.left, Rect.top,
    Rect.right - Rect.left, Rect.bottom - Rect.top,
    SWP_NOMOVE | SWP_NOZORDER);
}

void sendMessage(HWND hwnd)
{
  char sMessage[1024];
  //for (int i = 1; i < LINES - 1; i++)
  //strcpy_s(asMessageList[i], asMessageList[i + 1]);
  HWND hWndEditMessage;
  hWndEditMessage = GetDlgItem(hwnd, ID_EDIT_MESSAGE);
  GetWindowText(hWndEditMessage, sMessage, 1024);
  //GetWindowText(hWndEditMessage, sMessage, 1024);
  //MessageBox(0, sMessage, TEXT("Wiadomoœæ"), MB_OK);
  ////Wymusznie odmalowania okna
  RECT rc;
  GetClientRect(hwnd, &rc); //Pobranie obszaru klienciego
  InvalidateRect(hwnd, NULL, false); //Uniewaznienie tresci
  RedrawWindow(hwnd, &rc, NULL, RDW_ERASE); //Wymuszenie odmalowania niewa¿nej

  //Przygotuj opis adresu serwera
  struct sockaddr_in si_other;
  memset((char*)&si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(SERVER_PORT);
  si_other.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDR);
  int slen = sizeof(si_other);
  if (sendto(clientSocket, sMessage, strlen(sMessage), 0, (struct sockaddr*)
    & si_other, slen) == SOCKET_ERROR) {
    MessageBox(0, TEXT("Nie uda³o sie wys³aæ danych b³ad"), TEXT("WinSockError"),MB_OK);
      return;
  }
}

void recieveMessage(sData* data)
{
  struct sockaddr_in si_other;
  int slen, recv_len;
  char buf[1024];
  while (gNotExit)
  {
    //Wyzerowanie buffora
    memset(buf, '\0', 1024);
    //Odbierz dane
    if ((recv_len = recvfrom(serverSocket, buf, 1024, 0,
      (struct sockaddr*) & si_other, &slen)) == SOCKET_ERROR)
    {
      MessageBox(0, TEXT("Nie uda³o sie odebraæ danych - b³¹d"), TEXT("WinSockError"), MB_OK);
      return;
    }
    if (data->server) {
      //Sprawdz czy ju¿ kiedyæ widziano tego klienta
      bool bFound = false;
      for (int i = 0; i < iCountConnectedClients; i++)
      {
        if (siClients[i].sin_addr.S_un.S_addr == si_other.sin_addr.S_un.S_addr)
        {
          bFound = true;
          break;
        }
      }
      if (!bFound) //Jeœli nie by³o jeszcze nigdy to dodaj do listy
        memcpy(&siClients[iCountConnectedClients++], &si_other, sizeof(si_other));

      for (int i = 0; i < iCountConnectedClients; i++)
      {
        //Przeœlij do innych osób
        int slen = sizeof(si_other);
        //send the message
        if (sendto(serverSocket, buf, strlen(buf), 0, (struct sockaddr*)
          & siClients[i], slen) == SOCKET_ERROR)
        {
          MessageBox(0, TEXT("Nie uda³o sie wys³aæ danych b³ad"), TEXT("WinSockError"),MB_OK);
            return;
        }
      }
    }
  }

  for (int i = 1; i < LINES - 1; i++)
    strcpy_s(asMessageList[i], asMessageList[i + 1]);

  sprintf_s(asMessageList[LINES - 1], "%s:%d %s", inet_ntoa(si_other.sin_addr),
    ntohs(si_other.sin_port), buf);

  //Wymusznie odmalowania okna
  RECT rc;
  GetClientRect(data->hwnd, &rc); //Pobranie obszaru klienciego
  InvalidateRect(data->hwnd, NULL, false); //Uniewaznienie tresci
  RedrawWindow(data->hwnd, &rc, NULL, RDW_ERASE); //Wymuszenie odmalowania niewaznej
}

void createServer(HWND hwnd)
{
  //Utwórz Soket Serwera
  if ((serverSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
  {
    MessageBox(0, TEXT("Nie mo¿na utworzyæ socketa"), TEXT("WinSock Error"), MB_OK);
    //WSAGetLastError()
  }
  //Przygotój opis adresu serwera
  struct sockaddr_in server, si_other;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(SERVER_PORT);
  //Podpiêcie adresy pod socket
  if (bind(serverSocket, (struct sockaddr*) & server, sizeof(server)) ==
    SOCKET_ERROR)
  {
    MessageBox(0, TEXT("Nie mo¿na utworzyæ i podpi¹æ socketa pod wybrany adres"), TEXT("WinSock Error"), MB_OK);
    return;
  }

  //Odbierz dane
  //recieveMessage();
  sData* data = new sData;
  data->hwnd = hwnd;
  data->socket = serverSocket;
  data->server = true;
  DWORD dwThreadId;
  CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&recieveMessage, data, 0, &dwThreadId);
}

void createClient(HWND hwnd)
{
  //Utwórz Soket Klienta
  if ((clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
  {
    MessageBox(0, TEXT("Nie mo¿na utworzyæ socketa"), TEXT("WinSock Error"), MB_OK);
    //WSAGetLastError()
  }

  sendMessage(hwnd);
  sData* data = new sData;
  data->hwnd = hwnd;
  data->socket = clientSocket;
  data->server = false;
  DWORD dwThreadId;
  CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&recieveMessage, data, 0,
    &dwThreadId);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
  case WM_INITDIALOG:

    // Initializacja Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
    {
      MessageBox(0, TEXT("Initialization error."), TEXT("WinSock"), MB_OK);
      return 1;
    }

    //hWndButton = CreateWindow(TEXT("BUTTON"), TEXT("B1"), BS_FLAT | WS_VISIBLE | WS_CHILD, 10, 20, 50, 20, hwnd, (HMENU)210, hInst, NULL);
    hWndButtonSendMessage = CreateWindow(TEXT("BUTTON"), TEXT("Wyœlij wiadomoœæ"), BS_FLAT | WS_VISIBLE | WS_CHILD, 10, 20, 150, 20, hwnd, (HMENU)ID_SEND_MESSAGE, hInst, NULL);
    hWndEditMessage = CreateWindow(TEXT("EDIT"), TEXT("Moja Wiadomoœæ"), WS_BORDER | WS_VISIBLE | WS_CHILD, 10, 50, 250, 20, hwnd, (HMENU)ID_EDIT_MESSAGE, hInst, NULL);
    hWndButtonCreateServer = CreateWindow(TEXT("BUTTON"), TEXT("Stwórz Serwer"),BS_FLAT | WS_VISIBLE | WS_CHILD, 170, 20, 150, 20, hwnd, (HMENU)ID_CREATE_SERVER, hInst, NULL);
    hWndButtonCreateClient = CreateWindow(TEXT("BUTTON"), TEXT("Stwórz Klienta"),BS_FLAT | WS_VISIBLE | WS_CHILD, 350, 20, 150, 20, hwnd, (HMENU)ID_CREATE_CLIENT, hInst, NULL);
    // obraz ³adowany z pliku
    //hBitmap = (HBITMAP)LoadImageW(GetModuleHandleW(NULL), L"./../source/mountains.bmp",
    //  IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    // dopasowanie rozmiarow okna
    //GetObjectW(hBitmap, sizeof(bitmap_info), &bitmap_info);
    //bitmap_rect = { 0, 0, bitmap_info.bmWidth, bitmap_info.bmHeight };
   // ResizeWindowRect(hwnd, bitmap_rect);
    break;

  case WM_PAINT:
    HDC hDC;
    hDC = GetDC(hwnd);

    HDC hDCBitmap;
    hDCBitmap = CreateCompatibleDC(hDC); //Utworzenie kopatybilengo kontekstu
    SelectObject(hDCBitmap, hBitmap); //Wybranie bitmapy w kontekscie
    displayMessageList(hDC);

    /*if (TRUE)
    {
      RECT rWndRect;
      GetClientRect(hwnd, &rWndRect);
      StretchBlt(hDC, 0, 0, rWndRect.right, rWndRect.bottom,
        hDCBitmap, 0, 0, bitmap_info.bmWidth, bitmap_info.bmHeight,
        SRCCOPY);
    }*/

    DeleteDC(hDCBitmap); //Usuniecie kontekstu
    ReleaseDC(hwnd, hDC);
    break;

  /*case WM_SIZING:
    bitmap_rect = { 0,0,WMSZ_RIGHT,WMSZ_BOTTOM };
    ResizeWindowRect(hwnd, bitmap_rect);
    break;*/

  case WM_COMMAND:
    if (HIWORD(wParam) == 0) //Sprawdzy czy identyfikator pozycji w menu
      switch (LOWORD(wParam))
      {
      case ID_MENU_EXIT:
        PostQuitMessage(0);
        break;
      case ID_SEND_MESSAGE:
        sendMessage(hwnd);
        break;
      case ID_CREATE_SERVER:
        createServer(hwnd);
        break;
      case ID_CREATE_CLIENT:
        createClient(hwnd);
        break;
      }
    break;

  case WM_CLOSE:
    // Posprz¹tanie Winsock
    closesocket(serverSocket);
    WSACleanup();
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

  if (hwndMainWindow == NULL) return(FALSE);

  ShowWindow(hwndMainWindow, iCmdShow);

  

  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}