#ifndef CHIP8_STACK
#define CHIP8_STACK

#include "config.h"

struct chip8;
struct chip8_stack{
    unsigned short stack[CHIP8_STACKSIZE];
};

void chip8_push(struct chip8 *chip8,unsigned short val);
unsigned short chip8_pop(struct chip8 *chip8);


#endif