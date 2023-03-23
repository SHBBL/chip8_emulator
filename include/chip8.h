#ifndef CHIP8_H
#define CHIP8_H

#include <stddef.h>
#include "config.h"
#include "chip8memory.h"
#include "chip8registers.h"
#include "chip8stack.h"
#include "chip8keyboard.h"
#include "chip8screen.h"

struct chip8{
    struct chip8_mem memory;
    struct chip8_reg registers;
    struct chip8_stack stack;
    struct chip8_keyboard keyboard;
    struct chip8_screen screen;
};

void chip8_init(struct chip8 *chip8);
void chip8_load(struct chip8 *chip8,char * buff,size_t size);
void chip8_exec(struct chip8 *chip8,unsigned short opcode);

#endif