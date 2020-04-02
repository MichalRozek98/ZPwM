#include <iostream>
#include <Windows.h>
#include <string>
#include <time.h>
#include <cmath>
#include <math.h>
#include <cstdlib>
#include <ctime>

using namespace std;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  srand((int)time(0));
  int number_find = 0;

  AllocConsole(); // turn on the consol for write a value to the variable number_find which is " our number "
  FILE* fpstdin = stdin, * fpstdout = stdout, * fpstderr = stderr;

  freopen_s(&fpstdin, "CONIN$", "r", stdin);
  freopen_s(&fpstdout, "CONOUT$", "w", stdout);
  freopen_s(&fpstderr, "CONOUT$", "w", stderr);

  do
  {
    cout << "Write your number (0-40): ";
    cin >> number_find;
  } while (number_find < 0 || number_find > 40);

  HWND myConsole = GetConsoleWindow();
  ShowWindow(myConsole, 0); // turning off console and continue the program
  
  
  int rand_number = 41;
  int remember = 0; // remember the last of a drawn number
  int searchingForNumber = rand() % 41; //initial random number

  do
  {
    searchingForNumber = rand() % rand_number + remember; //drawing a number from the updated range
    if (number_find == searchingForNumber) // congratulations when computer drew a number
    {
      string congratulations = "Congratulations, your number: " + to_string(searchingForNumber) + " has been drew at random!";
      MessageBoxA(0, congratulations.c_str(), "Application", MB_OK);
    }
    else
    {
      string show_question = "Is that number greater than: " + to_string(searchingForNumber) + " ?";
      int iRetKey = MessageBoxA(0, show_question.c_str(), "Application", MB_YESNO);

      if (iRetKey == IDYES) // if number is greater than our
      {
        rand_number -= (searchingForNumber - (remember - 1)); // -1 is for case when for example search is 22, after that our random is = 41 - (22 - (0 - 1))
                                                             // so random is = 41 - (22 + 1) = 41 - 23 = 18, so rand() % 18 gives us numbers from 0 to 17
                                                            // if there is no "-1" it would be a number from 0 to 16
        remember = searchingForNumber + 1; // remember is search plus 1 so 22 + 1 = 23, all it gives us rand() % 18 + 23, so our compartment is from (0+23) to (17+23)
                                          // so it gives us from 23 to 40 and it is correct option
      }
      else // if number is smaller than our
      {
        rand_number = abs(remember - searchingForNumber); // abs is in case of negative numbers and after that is works great
      }
    }
  } while (number_find != searchingForNumber); // end loop when we drew a number

  return 0;
}