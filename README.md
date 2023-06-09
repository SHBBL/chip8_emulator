# chip8_emulator

## ⚠️Doesn't support super_chip48 instructions⚠️

## Chip8:
Chip-8 is a simple, interpreted, programming language which was first used on some do-it-yourself computer systems in the late 1970s and early 1980s. The COSMAC VIP, DREAM 6800, and ETI 660 computers are a few examples. These computers typically were designed to use a television as a display, had between 1 and 4K of RAM, and used a 16-key hexadecimal keypad for input. The interpreter took up only 512 bytes of memory, and programs, which were entered into the computer in hexadecimal, were even smaller.

Chip-48 inspired a whole new crop of Chip-8 interpreters for various platforms, including MS-DOS, Windows 3.1, Amiga, HP48, MSX, Adam, and ColecoVision. I became involved with Chip-8 after stumbling upon Paul Robson's interpreter on the World Wide Web. Shortly after that, I began writing my own Chip-8 interpreter.

This document is a compilation of all the different sources of information I used while programming my interpreter.



## Keypad:
 ```
 Keypad and keyboard               
 +-+-+-+-+               
 |1|2|3|C|               
 +-+-+-+-+               
 |4|5|6|D|                
 +-+-+-+-+          
 |7|8|9|E|             
 +-+-+-+-+       
 |A|0|B|F|                
 +-+-+-+-+   
```
## Installation:
```
git clone https://github.com/SHBBL/chip8_emulator.git
cd chip8_emulator
mingw32-make && cd bin
```
## Usage:
``
main.exe <rom game from /chip8_emulator/roms>
``

## Screenshots:
![alt text](https://github.com/SHBBL/chip8_emulator/blob/main/blob/invaders.png)
![alt text](https://github.com/SHBBL/chip8_emulator/blob/main/blob/invaders2.png)

## dependencies:
* C/C++ Gcc (Mingw)
* SDL2
* Mingw32-make for Make Manual Compilation



