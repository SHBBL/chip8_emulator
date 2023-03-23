#ifndef CHIP8_MEMORY
#define CHIP8_MEMORY

#include "config.h"

struct chip8_mem{
    unsigned char memory[CHIP8_MEMSIZE];
};

void chip8_memset(struct chip8_mem * mem,int index,unsigned char val);
unsigned char chip8_memget(struct chip8_mem * mem,int index);
unsigned short chip8_getword(struct chip8_mem * mem,int index);

#endif