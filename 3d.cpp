#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <sys/ioctl.h>

using namespace std;

string COLOURS[] = {"\u001b[31m", "\u001b[32m", "\u001b[34m", "\u001b[33m", "\u001b[35m"};

struct Screen {
  int width, height;
  char **buffer;
};


void cleanBuffer(struct Screen *screen) {
  char **buf = screen->buffer;
  for (int y = 0; y<screen->height; y++){
    for (int x = 0; x<screen->width; x++) buf[y][x] = ' ';
  }
}


void projectString(
    struct Screen *screen, 
    string message,
    int xPos, int yPos ) {

  char **buf = screen->buffer;
  for (int i = 0;
      i < screen->width &&
      i < message.length(); i++) {

    buf[yPos][xPos + i] = message[i];
  }
}


void drawOnce(struct Screen *screen) {

  char **buf = screen->buffer;
  int sWidth  = screen->width,
      sHeight = screen->height;

  string clearLine   = "\x1B[2K";     // Erase the entire current line.
  string clearScreen = "\033[2J";     // Erase the entire screen.
  string putPixel    = "\033[%d;%dH"; // Move to coord [col;rowH.

  cout << clearScreen ;

  for (int y = 0; y < sHeight; y++) {
    for (int x = 0; x < sWidth; x++) {
        printf(putPixel.c_str(), y, x);
        printf("%c", buf[y][x]);
    }
  }
}


int main(){
  struct winsize window_dimen;
  ioctl(STDOUT_FILENO,
        TIOCGWINSZ,
        &window_dimen);

  // initialize a new 'screen' struct
  struct Screen screen;
  screen.width = window_dimen.ws_col;
  screen.height = window_dimen.ws_row;
  
  // create a 2d buffer representing the screen
  char **buffer = new char*[screen.height];
  for (int i = 0; i < screen.height; i++)
    buffer[i] = new char[screen.width];
  screen.buffer = buffer;

  string message[] = {"             *********************\n               ",
                      "        *******************************\n          ",
                      "    ******** ______           ______********\n      ",
                      "  ********  |  __   \\ \\    / /  __  \\ ********\n ",
                      " ********   | |  | | \\ \\  / /| |  | |  ********\n ",
                      "********    | |  | |  \\ \\/ / | |  | |   ********\n",
                      " ********   | |  | |   \\  /  | |  | |  ********\n  ",
                      "  ********  |_____/     \\/   |______/  ********\n   ",
                      "    ********                        *******\n      ",
                      "        *******************************\n          ",
                      "             *********************\n               "};

  int color = 0;
  int blobX = 1, blobY = 1;
  int blobXVeloc = 1, blobYVeloc = 1;
  cleanBuffer(&screen);
  while(true){

    for (int i = 0; i < 11; i++) projectString(&screen, message[i], blobX, blobY+i);

    printf("%s",COLOURS[color].c_str());
    drawOnce(&screen);
    cleanBuffer(&screen);

    if (blobX+48 == screen.width  || blobX == 0){
      blobXVeloc *= -1;
      color = rand() % 5;
    }
    if (blobY+11  == screen.height || blobY == 0){
      blobYVeloc *= -1;
      color = rand() % 5;
    } 
      


    blobX += blobXVeloc;
    blobY += blobYVeloc;
    usleep(50000);
  }

  printf("\n");
}

