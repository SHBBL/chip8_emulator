#include "chip8.h"
#include "chip8stack.h"
#include <assert.h>
void check_stack(struct chip8 * chip8){
    assert(chip8->registers.SP < sizeof(chip8->stack.stack));
}
void chip8_push(struct chip8 *chip8, unsigned short val)
{
    chip8->registers.SP += 1;
    check_stack(chip8);
    chip8->stack.stack[chip8->registers.SP] = val;
}

unsigned short chip8_pop(struct chip8 *chip8)
{
    check_stack(chip8);
    unsigned short result = chip8->stack.stack[chip8->registers.SP];
    chip8->registers.SP -= 1;
    return result;
}