#include "chip8.h"
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <assert.h>
#include "SDL2/SDL.h"
#include <stdio.h>

const char chip8_charset[] = {0xF0, 0x90, 0x90, 0x90, 0xF0, 0x20, 0x60, 0x20, 0x20, 0x70, 0xF0, 0x10, 0xF0, 0x80, 0xF0, 0xF0, 0x10, 0xF0, 0x10, 0xF0, 0x90, 0x90, 0xF0, 0x10, 0x10, 0xF0, 0x80, 0xF0, 0x10, 0xF0, 0xF0, 0x80, 0xF0, 0x90, 0xF0, 0xF0, 0x10, 0x20, 0x40, 0x40, 0xF0, 0x90, 0xF0, 0x90, 0xF0, 0xF0, 0x90, 0xF0, 0x10, 0xF0, 0xF0, 0x90, 0xF0, 0x90, 0x90, 0xE0, 0x90, 0xE0, 0x90, 0xE0, 0xF0, 0x80, 0x80, 0x80, 0xF0, 0xE0, 0x90, 0x90, 0x90, 0xE0, 0xF0, 0x80, 0xF0, 0x80, 0xF0, 0xF0, 0x80, 0xF0, 0x80, 0x80};

void chip8_init(struct chip8 *chip8)
{
    memset(chip8, 0, sizeof(struct chip8));
    memcpy(&chip8->memory.memory, chip8_charset, sizeof(chip8_charset));
}
void chip8_load(struct chip8 *chip8, char *buff, size_t size)
{
    memcpy(&chip8->memory.memory[CHIP8_LOADADDR], buff, size);
    chip8->registers.PC = CHIP8_LOADADDR;
}
static char waitforpress(struct chip8 *chip8)
{
    SDL_Event event;
    while (SDL_WaitEvent(&event))
    {
        if (event.type != SDL_KEYDOWN)
        {
            continue;
        }
        char key = event.key.keysym.sym;
        char vkey = chip8_keypressed(&chip8->keyboard, key);
        if (vkey != 1)
        {
            return vkey;
        }
    }
    return -1;
}
static void chip8_defualt_2(struct chip8 *chip8, unsigned short opcode)
{
    unsigned char x = (opcode >> 8) & 0x000f;
    unsigned char y = (opcode >> 4) & 0x000f;
    int rslt = 0;

    switch (opcode & 0x000f)
    {
    case 0x00:
        chip8->registers.V[x] = chip8->registers.V[y];
        break;
    case 0x01:
        chip8->registers.V[x] |= chip8->registers.V[y];
        break;
    case 0x02:
        chip8->registers.V[x] &= chip8->registers.V[y];
        break;
    case 0x03:
        chip8->registers.V[x] ^= chip8->registers.V[y];
        break;
    case 0x04:
        rslt = chip8->registers.V[x] + chip8->registers.V[y];
        chip8->registers.V[0xf] = 0;
        if (rslt > 0xff)
        {
            chip8->registers.V[0xf] = 1;
        }
        chip8->registers.V[x] = rslt;
        break;
    case 0x05:
        chip8->registers.V[0xf] = 0;
        if (chip8->registers.V[x] > chip8->registers.V[y])
        {
            chip8->registers.V[0xf] = 1;
        }
        chip8->registers.V[x] -= chip8->registers.V[y];
        break;
    case 0x06:
        chip8->registers.V[0xf] = chip8->registers.V[x] & 0x01;
        chip8->registers.V[x] /= 2;
        break;
    case 0x07:
        chip8->registers.V[0xf] = chip8->registers.V[y] > chip8->registers.V[x];
        chip8->registers.V[x] = chip8->registers.V[y] - chip8->registers.V[x];
        break;
    case 0x0E:
        chip8->registers.V[0xf] = chip8->registers.V[x] & 0b10000000;
        chip8->registers.V[x] *= 2;
        break;
    }
}

static void chip8_defualt_4(struct chip8 *chip8, unsigned short opcode)
{
    unsigned char x = (opcode >> 8) & 0x000f;
    switch (opcode & 0x00ff)
    {
    case 0x0007:
        chip8->registers.V[x] = chip8->registers.DT;
        break;
    case 0x000A:
    {
        char pressedkey = waitforpress(chip8);
        chip8->registers.V[x] = pressedkey;
    }
    break;
    case 0x0015:
        chip8->registers.DT = chip8->registers.V[x];
        break;
    case 0x0018:
        chip8->registers.ST = chip8->registers.V[x];
        break;
    case 0x001E:
        chip8->registers.I += chip8->registers.V[x];
        break;
    case 0x0029:
        chip8->registers.I = chip8->registers.V[x] * CHIP8_DEFUALT_HEIGHT;
        break;
    case 0x0033:
    {
        unsigned char hundreds = chip8->registers.V[x] / 100;
        unsigned char tens = chip8->registers.V[x] / 10 % 10;
        unsigned char unit = chip8->registers.V[x] % 10;
        chip8_memset(&chip8->memory, chip8->registers.I, hundreds);
        chip8_memset(&chip8->memory, chip8->registers.I + 1, tens);
        chip8_memset(&chip8->memory, chip8->registers.I + 2, unit);
    }
    break;
    case 0x0055:
    {
        for (int i = 0; i <= x; i++)
        {
            chip8_memset(&chip8->memory, chip8->registers.I + i, chip8->registers.V[i]);
        }
    }
    break;
    case 0x0065:
    {
        for (int i = 0; i <= x; i++)
        {
            chip8->registers.V[i] = chip8_memget(&chip8->memory, chip8->registers.I + i);
        }
    }
    break;
    }
}
static void chip8_defualt_1(struct chip8 *chip8, unsigned short opcode)
{
    unsigned short nnn = opcode & 0x0fff;
    unsigned char x = (opcode >> 8) & 0x000f;
    unsigned char y = (opcode >> 4) & 0x000f;
    unsigned char kk = opcode & 0x00ff;
    unsigned char n = opcode & 0x000f;
    switch (opcode & 0xf000)
    {
    case 0x1000:
        chip8->registers.PC = nnn;
        break;
    case 0x2000:
        chip8_push(chip8, chip8->registers.PC);
        chip8->registers.PC = nnn;
        break;
    case 0x3000:
        if (chip8->registers.V[x] == kk)
        {
            chip8->registers.PC += 2;
        }
        break;
    case 0x4000:
        if (chip8->registers.V[x] != kk)
        {
            chip8->registers.PC += 2;
        }
        break;
    case 0x5000:
        if (chip8->registers.V[x] == chip8->registers.V[y])
        {
            chip8->registers.PC += 2;
        }
        break;
    case 0x6000:
        chip8->registers.V[x] = kk;
        break;
    case 0x7000:
        chip8->registers.V[x] += kk;
        break;
    case 0x8000:
        chip8_defualt_2(chip8, opcode);
        break;
    case 0x9000:
        if (chip8->registers.V[x] != chip8->registers.V[y])
        {
            chip8->registers.PC += 2;
        }
        break;
    case 0xA000:
        chip8->registers.I = nnn;
        break;
    case 0xB000:
        chip8->registers.PC = nnn + chip8->registers.V[0];
        break;
    case 0xC000:
        srand(clock());
        chip8->registers.V[x] = (rand() % 255) & kk;
        break;
    case 0xD000:
    {
        const char *sprite = (const char *)&chip8->memory.memory[chip8->registers.I];
        chip8->registers.V[0xf] = chip8_screendraw(&chip8->screen, chip8->registers.V[x], chip8->registers.V[y], sprite, n);
    }
    case 0xE000:
    {
        switch (opcode & 0x00ff)
        {
        case 0x009E:
            if (keyispressed(&chip8->keyboard, chip8->registers.V[x]))
            {
                chip8->registers.PC += 2;
            }
            break;
        case 0x00A1:
            if (!keyispressed(&chip8->keyboard, chip8->registers.V[x]))
            {
                chip8->registers.PC += 2;
            }
            break;
        }
    }
    break;
    case 0xF000:
        chip8_defualt_4(chip8, opcode);
        break;
        
    }
}
void chip8_exec(struct chip8 *chip8, unsigned short opcode)
{
    switch (opcode)
    {
    case 0x00E0:
        screen_clear(&chip8->screen);
        break;
    case 0x00EE:
        chip8->registers.PC = chip8_pop(chip8);
        break;
    default:
        chip8_defualt_1(chip8, opcode);
    }
}