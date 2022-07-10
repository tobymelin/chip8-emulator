#include <iostream>
#include "chip8.h"

using namespace std;

int main() {
	CHIP8 *chip8 = new CHIP8();

	chip8->load_rom("Pong [Paul Vervalin, 1990].ch8");
	chip8->boot();
}

