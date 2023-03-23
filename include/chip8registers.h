#ifndef CHIP8_REG
#define CHIP8_REG

#include "config.h"

struct chip8_reg{
    unsigned char V[CHIP8_REGNUM];
    unsigned short I;
    unsigned char ST;
    unsigned char DT;
    unsigned short PC;
    unsigned char SP;
};

#endif