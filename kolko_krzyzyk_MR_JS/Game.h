#pragma once

#include <math.h>
#include <stdlib.h>
#include "DrawObjects.h"
#include <SFML/Network.hpp>
#include <iostream>
#include "TCP.h"

#define PI 3.14159f
int winner_looser_matrix[3][3];
int matrix_help[3][3];
int x_label[3];
int y_label[3];
bool move = false;
bool somebody_wins = false;
Draw* draw = new Draw();
bool first_receive = false;
bool if_is_draw = false;


void X_or_O(int i, int j) {
  if (who == 's') {
    if (winner_looser_matrix[i][j] == -1) {
      draw->Draw_X(x_label[j], y_label[i]);
      winner_looser_matrix[i][j] = 2; // X
      move = true;
    }
  }
  else {
    if (winner_looser_matrix[i][j] == -1) {
      draw->Draw_O(x_label[j], y_label[i]);
      winner_looser_matrix[i][j] = 1; // O
      move = false;
    }
  }
}

void SendTheMatrix() {
  if (who == 's') {
    if (socket.send(winner_looser_matrix, sizeof(winner_looser_matrix)) != sf::Socket::Done)
      return;
  }
  else {
    if (socket.send(winner_looser_matrix, sizeof(winner_looser_matrix)) != sf::Socket::Done)
      return;
  }
}

void ReceiveTheMatix() {

  if (who == 's') {
    socket.receive(matrix_help, sizeof(matrix_help), received_server);
  }
  else {
    socket.receive(matrix_help, sizeof(matrix_help), received_client);
  }

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      if (matrix_help[i][j] != winner_looser_matrix[i][j] && winner_looser_matrix[i][j] == -1) {
        winner_looser_matrix[i][j] = matrix_help[i][j];

        if (winner_looser_matrix[i][j] == 1) {
          draw->Draw_O(x_label[j], y_label[i]);
        }
        else if (winner_looser_matrix[i][j] == 2) {
          draw->Draw_X(x_label[j], y_label[i]);
        }
      }
    }
  }

}

void init(void) {
  glClearColor(1, 1, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  move = false;
  somebody_wins = false;


  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      winner_looser_matrix[i][j] = -1;
      matrix_help[i][j] = -1;
    }
  }

  int value = 115;

  for (int i = 0; i < 3; ++i) {
    x_label[i] = value;
    value += 185;
  }
  value -= 185;
  for (int i = 0; i < 3; ++i) {
    y_label[i] = value;
    value -= 185;
  }
  first_receive = false;
  if_is_draw = false;
}

void WhoIsTheWinner() {
  int iterator_x_w = 0;
  int iterator_o_w = 0;
  int iterator_x_k = 0;
  int iterator_o_k = 0;
  int iterator_x_b = 0;
  int iterator_o_b = 0;
  int iterator_x_s = 0;
  int iterator_o_s = 0;
  int iterator_move = 0;
  int if_empty_matrix = 0;

  int x, y;

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      if (winner_looser_matrix[i][j] == 1) {
        ++iterator_o_w;
      }
      else if (winner_looser_matrix[i][j] == 2) {
        ++iterator_x_w;
      }
      if (winner_looser_matrix[j][i] == 1) {
        ++iterator_o_k;
      }
      else if (winner_looser_matrix[j][i] == 2) {
        ++iterator_x_k;
      }
      ++iterator_move;

      if (winner_looser_matrix[i][i] == 1) {
        ++iterator_o_b;
      }
      else if (winner_looser_matrix[i][i] == 2) {
        ++iterator_x_b;
      }
      if (winner_looser_matrix[i][2 - i] == 1) {
        ++iterator_o_s;
      }
      else if (winner_looser_matrix[i][2 - i] == 2) {
        ++iterator_x_s;
      }

      int value_w = 485;
      int value_k = 115;

      for (int k = 0; k < i; ++k) {
        value_w -= 185;
      }
      for (int l = 0; l < i; ++l) {
        value_k += 185;
      }

      if (somebody_wins == false) {
        if (iterator_o_w == 3 || iterator_x_w == 3) {
          draw->Draw_Win(65, value_w, 535, value_w);
        }
        if (iterator_o_k == 3 || iterator_x_k == 3) {
          draw->Draw_Win(value_k, 65, value_k, 535);
        }
        if (iterator_o_b == 9 || iterator_x_b == 9) {
          draw->Draw_Win(65, 535, 535, 65);
        }
        if (iterator_o_s == 9 || iterator_x_s == 9) {
          draw->Draw_Win(65, 65, 535, 535);
        }
      }

      if (iterator_o_w == 3 || iterator_o_k == 3 || iterator_o_b == 9 || iterator_o_s == 9) {
        draw->DrawString(GLUT_BITMAP_HELVETICA_18, "Mr. O wins", 430, 580);
        somebody_wins = true;
        break;
      }
      else if (iterator_x_w == 3 || iterator_x_k == 3 || iterator_x_b == 9 || iterator_x_s == 9) {
        draw->DrawString(GLUT_BITMAP_HELVETICA_18, "Mr. X wins", 430, 580);
        somebody_wins = true;
        break;
      }
    }

    iterator_o_w = 0;
    iterator_x_w = 0;
    iterator_x_k = 0;
    iterator_o_k = 0;
  }



  if (!somebody_wins) {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (winner_looser_matrix[i][j] == 1 || winner_looser_matrix[i][j] == 2) {
          ++if_empty_matrix;
        }
      }
    }

    if (if_empty_matrix == 9) {
      draw->DrawString(GLUT_BITMAP_HELVETICA_18, "Draw", 450, 580);
      if_is_draw = true;
    }
  }

}

void mouse(int button, int state, int x, int y) {

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
      if (!somebody_wins)
      {
        if (x > 40 && (600 - y) > 40 && x < 200 && (600 - y) < 200) {
          X_or_O(2, 0);
        }
        else if (x > 200 && (600 - y) > 40 && x < 400 && (600 - y) < 200) {
          X_or_O(2, 1);
        }
        else if (x > 400 && (600 - y) > 40 && x < 560 && (600 - y) < 200) {
          X_or_O(2, 2);
        }
        else if (x > 40 && (600 - y) > 200 && x < 200 && (600 - y) < 400) {
          X_or_O(1, 0);
        }
        else if (x > 200 && (600 - y) > 200 && x < 400 && (600 - y) < 400) {
          X_or_O(1, 1);
        }
        else if (x > 400 && (600 - y) > 200 && x < 560 && (600 - y) < 400) {
          X_or_O(1, 2);
        }
        else if (x > 40 && (600 - y) > 400 && x < 200 && (600 - y) < 560) {
          X_or_O(0, 0);
        }
        else if (x > 200 && (600 - y) > 400 && x < 400 && (600 - y) < 560) {
          X_or_O(0, 1);
        }
        else if (x > 400 && (600 - y) > 400 && x < 560 && (600 - y) < 560) {
          X_or_O(0, 2);
        }
      }
    }

    glFlush();
    SendTheMatrix();
    WhoIsTheWinner();
    if (!somebody_wins && !if_is_draw)
      ReceiveTheMatix();
    WhoIsTheWinner();
    glFlush();
    glutPostRedisplay();

  if (x > 255 && (600 - y) > 580 && x < 345 && (600 - y) < 600) {
    glClearColor(1, 1, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        winner_looser_matrix[i][j] = -1;
        matrix_help[i][j] = -1;
      }
    }

    move = false;
    somebody_wins = false;
    first_receive = false;
    if_is_draw = false;
    received_client = 0;
    received_server = 0;

    glutPostRedisplay();

    socket.disconnect();

    if (who == 's')
    {
      while (listener.listen(port) != sf::Socket::Done)
        return;

      while (listener.accept(socket) != sf::Socket::Done)
        return;
    }
    else
    {
      while (server == sf::IpAddress::None);

      while (socket.connect(server, port) != sf::Socket::Done)
        return;
    }
  }
}


void display(void) {
  draw->DrawLines();
  draw->DrawString(GLUT_BITMAP_HELVETICA_18, "Mr. X starts - server", 5, 580);
  draw->DrawString(GLUT_BITMAP_HELVETICA_18, "Play again", 258, 580);
  glFlush();
  if (who == 'c' && !first_receive) {
    first_receive = true;
    ReceiveTheMatix();
  }
  glFlush();
}

void resize(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, w, 0, h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}