#include "chip8screen.h"
#include <memory.h>
#include <assert.h>

static void check(int x, int y)
{
    assert(x >= 0 && x < CHIP8_SCREEN_WIDTH && y >= 0 && y < CHIP8_SCREEN_HEIGHT);
}
void chip8_screenset(struct chip8_screen *screen, int x, int y)
{
    check(x, y);
    screen->pixels[y][x] = true;
}
bool chip8_screenisset(struct chip8_screen *screen, int x, int y)
{
    check(x, y);
    return screen->pixels[y][x];
}
bool chip8_screendraw(struct chip8_screen *screen, int x, int y, const char *sprite, int n)
{
    bool pixel_isset = false;
    for (int ly = 0; ly < n; ly++)
    {
        char c = sprite[ly];
        for (int lx = 0; lx < 8; lx++)
        {
            if ((c & (0b10000000 >> lx)) == 0)
            {
                continue;
            }
            if (screen->pixels[(ly + y) % CHIP8_SCREEN_HEIGHT][(lx + x) % CHIP8_SCREEN_WIDTH])
            {
                pixel_isset = true;
            }
            screen->pixels[(ly + y) % CHIP8_SCREEN_HEIGHT][(lx + x) % CHIP8_SCREEN_WIDTH] ^= true;
        }
    }
    return pixel_isset;
}
void screen_clear(struct chip8_screen *screen)
{
    memset(screen->pixels, 0, sizeof(screen->pixels));
}