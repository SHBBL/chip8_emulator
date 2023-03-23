#include "chip8keyboard.h"

int chip8_keypressed(struct chip8_keyboard *keyboad, char key)
{
    for (int i = 0; i < CHIP8_KEYNUM; i++)
    {
        if (keyboad->keyboardmap[i] == key)
        {
            return i;
        }
    }
    return -1;
}
bool keyispressed(struct chip8_keyboard *keyboad, int key)
{
    return keyboad->keyboard[key];
}
void key_up(struct chip8_keyboard *keyboad, int key)
{
    keyboad->keyboard[key] = false;
}
void key_down(struct chip8_keyboard *keyboad, int key)
{
    keyboad->keyboard[key] = true;
}
void keymapset(struct chip8_keyboard *keyboad, const char *kmap)
{
    keyboad->keyboardmap = kmap;
}