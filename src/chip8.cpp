#include "chip8.h"
#include "memory.h"
#include "cpu.h"

CHIP8::CHIP8() {
	mem = new Memory();
	cpu = new CPU();

	// Initialise CPU connection to memory
	cpu->memory(mem);
}

void CHIP8::load_rom(string f) {
	// Load ROM into first allowed memory slot (0x200 address)
	mem->load_rom(f, 0x200);
}

void CHIP8::emulate() {
	cpu->emulate();
}


int main() {
	CHIP8* emu = new CHIP8;
	emu->load_rom("Pong [Paul Vervalin, 1990].ch8");

	while (true) {
		emu->emulate();
	}

	return 0;
}

