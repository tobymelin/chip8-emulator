#include <iostream>
#include "chip8.h"

using namespace std;

int main(int argc, char *argv[]) {
	CHIP8 *chip8 = new CHIP8();

 	if (argc > 1)
		chip8->load_rom(argv[1]);
	else
		chip8->load_rom("Pong [Paul Vervalin, 1990].ch8");

	chip8->boot();
}

