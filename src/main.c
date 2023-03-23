#include <stdio.h>
#include <Windows.h>
#include "chip8.h"
#include "SDL2/SDL.h"

const char chip8_keys[CHIP8_KEYNUM] = {SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6, SDLK_7, SDLK_8, SDLK_9, SDLK_a, SDLK_b, SDLK_c, SDLK_d, SDLK_e, SDLK_f};
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Provide the game rom");
        return -1;
    }
    const char *rom = argv[1];
    FILE* fp = fopen(rom, "rb");
    if (!fp)
    {
        printf("unable to open rom");
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char buff[size];
    int ch = fread(buff, size, 1, fp);
    if (ch != 1)
    {
        printf("unable to read rom");
        return -1;
    }
    struct chip8 chip8;
    chip8_init(&chip8);
    chip8_load(&chip8, buff, size);
    keymapset(&chip8.keyboard, chip8_keys);
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow(CHIP8_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, CHIP8_WIDTH, CHIP8_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);
    while (1)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
            {
                goto end;
            }
            break;
            case SDL_KEYDOWN:
            {
                char key = event.key.keysym.sym;
                int vkey = chip8_keypressed(&chip8.keyboard, key);
                if (vkey != -1)
                {
                    key_down(&chip8.keyboard, vkey);
                }
            }
            break;

            case SDL_KEYUP:
            {
                char key = event.key.keysym.sym;
                int vkey = chip8_keypressed(&chip8.keyboard, key);
                if (vkey != -1)
                {
                    key_up(&chip8.keyboard, vkey);
                }
            }
            break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        for (int x = 0; x < CHIP8_SCREEN_WIDTH; x++)
        {
            for (int y = 0; y < CHIP8_SCREEN_HEIGHT; y++)
            {
                if (chip8_screenisset(&chip8.screen, x, y))
                {
                    SDL_Rect rec;
                    rec.x = x * 10;
                    rec.y = y * 10;
                    rec.w = 10;
                    rec.h = 10;
                    SDL_RenderFillRect(renderer, &rec);
                }
            }
        }
        SDL_RenderPresent(renderer);

        if (chip8.registers.DT > 0)
        {
            Sleep(1);
            chip8.registers.DT -= 1;
        }
        if (chip8.registers.ST > 0)
        {
            Beep(15000, 100 * chip8.registers.ST);
            chip8.registers.ST = 0;
        }
        unsigned short opcodes = chip8_getword(&chip8.memory, chip8.registers.PC);
        chip8.registers.PC += 2;
        chip8_exec(&chip8, opcodes);
    }
end:
    SDL_DestroyWindow(window);
    return 0;
}
