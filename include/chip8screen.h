#ifndef CHIP8_SCREEN
#define CHIP8_SCREEN

#include "config.h"
#include <stdbool.h>

struct chip8_screen{
    bool pixels[CHIP8_SCREEN_HEIGHT][CHIP8_SCREEN_WIDTH];
};

void chip8_screenset(struct chip8_screen *screen,int x,int y);
bool chip8_screenisset(struct chip8_screen *screen,int x,int y);
bool chip8_screendraw(struct chip8_screen *screen,int x,int y,const char * sprite,int n);
void screen_clear(struct chip8_screen * screen);
#endif