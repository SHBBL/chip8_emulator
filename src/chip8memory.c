#include "chip8memory.h"
#include <assert.h>
void check_mem(int index)
{
    assert(index >= 0 && index < CHIP8_MEMSIZE);
}
void chip8_memset(struct chip8_mem *mem, int index, unsigned char val)
{
    check_mem(index);
    mem->memory[index] = val;
}
unsigned char chip8_memget(struct chip8_mem *mem, int index)
{
    check_mem(index);
    return mem->memory[index];
}
unsigned short chip8_getword(struct chip8_mem *mem, int index)
{
    unsigned char byte1 = chip8_memget(mem, index);
    unsigned char byte2 = chip8_memget(mem, index + 1);
    return byte1 << 8 | byte2;
}