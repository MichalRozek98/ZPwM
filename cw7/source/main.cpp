#include "res.h"
#include <iostream>
#include <Windows.h>
#pragma comment( lib, "winmm.lib")

MMRESULT mmResult;
HWAVEOUT hwo = NULL; //Uchwyt do interfejsu urz¹dzenia odtwarzaj¹cego
UINT devId; //Identyfikator urz¹dzenia
WAVEFORMATEX pcmWaveFormat; //Definicja formatu dŸwieku
WAVEHDR whdr;
double frequency = 1000;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
    //case WM_INITDIALOG:
    case WM_COMMAND:
      switch (HIWORD(wParam))
      {
        case BN_CLICKED:
          waveOutSetVolume(hwo, 0xFFFFFFFF);

          ShowWindow(hWnd, SW_SHOW);
          SetForegroundWindow(hWnd);
          SetFocus(hWnd);

          pcmWaveFormat.wFormatTag = WAVE_FORMAT_PCM; //Format próbek
          pcmWaveFormat.nChannels = 1; //Liczba kana³ów
          pcmWaveFormat.nSamplesPerSec = 44100L; //Próbkowanie
          pcmWaveFormat.wBitsPerSample = 8; //Iloœæ bitów na próbkê
          pcmWaveFormat.nAvgBytesPerSec = 44100L; //Iloœæ Bajtów na sekundê
          pcmWaveFormat.nBlockAlign = 1;
          pcmWaveFormat.cbSize = 0;
          pcmWaveFormat.nAvgBytesPerSec = 44100L; //Iloœæ Bajtów na sekundê
          pcmWaveFormat.nBlockAlign = 1;
          pcmWaveFormat.cbSize = 0;


          for (devId = 0; devId < waveOutGetNumDevs(); devId++)
          {
            mmResult = waveOutOpen(&hwo, devId, &pcmWaveFormat, 0, 0, CALLBACK_NULL);
            if (mmResult == MMSYSERR_NOERROR) break;
          }
          if (mmResult != MMSYSERR_NOERROR)
          {
            MessageBox(hWnd, TEXT("Nie znaleziono karty d¿wiekowej o wymaganych paramtrach"), TEXT("Error"), MB_OK);
            return mmResult;
          }

          ZeroMemory(&whdr, sizeof(WAVEHDR));
          whdr.lpData = new char[pcmWaveFormat.nAvgBytesPerSec * 1]; //Iloœæ bajtów na sekundê
          whdr.dwBufferLength = pcmWaveFormat.nAvgBytesPerSec * 1; // razy iloœæ sekund
          whdr.dwUser = 0;
          whdr.dwFlags = 0;
          whdr.dwLoops = 0;
          whdr.dwBytesRecorded = 0;
          whdr.lpNext = 0;
          whdr.reserved = 0;
          //Wype³nienie buffora próbek próbkami sygna³u o czêstotliwoœci 880 Hz
          for (int i = 0; i < whdr.dwBufferLength; i++)
          {
            whdr.lpData[i] = 128 * sin(i * frequency / (double)pcmWaveFormat.nSamplesPerSec) + 128;
          }

          switch (LOWORD(wParam))
          {
            case IDC_PLAY:

                //waveOutSetVolume(hwo, 0xFFFFFFFF);
                PlaySound("bensound-thejazzpiano.wav", NULL, SND_ASYNC);
                SetFocus(NULL);
              return TRUE;
          
            case IDC_PLAY_NOICE:

              //Ustawienie g³oœnoœci
              //waveOutSetVolume(hwo, 0xFFFFFFFF);
              mmResult = waveOutPrepareHeader(hwo, &whdr, sizeof(WAVEHDR)); //Wys³anie nag³ówka
              if (mmResult != MMSYSERR_NOERROR) //Przygotowanie
              { //urz¹dzenia
                MessageBox(hWnd, TEXT("Nie mo¿na zainicjowaæ karty"), TEXT("Error"), MB_OK);
                return mmResult;
              }

              mmResult = waveOutWrite(hwo, &whdr, sizeof(WAVEHDR));//Wys³anie próbek do urzadzenia
              if (mmResult != MMSYSERR_NOERROR)
              {
                MessageBox(hWnd, TEXT("Nie mo¿na za³adowaæ próbek"), TEXT("Error"), MB_OK);
                return mmResult;
              }

              return TRUE;

            case IDC_STOP:
               PlaySound(NULL, NULL, SND_ASYNC);
               mmResult = waveOutUnprepareHeader(hwo, &whdr, sizeof(WAVEHDR));
               mmResult = waveOutClose(hwo);
               delete[] whdr.lpData;
               SetFocus(NULL);
              return TRUE;

            case IDC_FF:
             
              waveOutSetVolume(hwo, 0xFFFFFFFF);
              PlaySound("bensound-thejazzpiano.wav", NULL, SND_ASYNC || SND_LOOP);
              SetFocus(NULL);
              return TRUE;

            case IDC_REV:
              return TRUE;

            case IDC_500:
              frequency = 500;
              MessageBoxA(0,"Czêstotlwioœæ zosta³a ustawiona na: 500 Hz","Informacja", MB_OK);
              SetFocus(NULL);
              return TRUE;

            case IDC_1000:
              frequency = 1000;
              MessageBoxA(0, "Czêstotlwioœæ zosta³a ustawiona na: 1000 Hz", "Informacja", MB_OK);
              SetFocus(NULL);
              return TRUE;

            case IDC_1500:
              frequency = 1500;
              MessageBoxA(0, "Czêstotlwioœæ zosta³a ustawiona na: 1500 Hz", "Informacja", MB_OK);
              SetFocus(NULL);
              return TRUE;

            case IDC_2000:
              frequency = 2000;
              MessageBoxA(0, "Czêstotlwioœæ zosta³a ustawiona na: 2000 Hz", "Informacja", MB_OK);
              SetFocus(NULL);
              return TRUE;
          }
      }

    case WM_CLOSE:
      DestroyWindow(hWnd); // destroying window
      PostQuitMessage(0); // put in the loop information about ending application
      return TRUE;
  }
  return FALSE;
}




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  HWND hWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, WndProc);
  ShowWindow(hWnd, iCmdShow);

  if (hWnd == NULL) return(FALSE);

  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}