#ifndef CHIP8_KEYBOARD
#define CHIP8_KEYBOARD

#include "config.h"
#include <stdbool.h>

struct chip8_keyboard{
    bool keyboard[CHIP8_KEYNUM];
    const char* keyboardmap;
};

int chip8_keypressed(struct chip8_keyboard * keyboad,char key);
bool keyispressed(struct chip8_keyboard * keyboad,int key);
void key_up(struct chip8_keyboard * keyboad,int key);
void key_down(struct chip8_keyboard * keyboad,int key);
void keymapset(struct chip8_keyboard * keyboad,const char* kmap);

#endif