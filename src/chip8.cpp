#include "chip8.h"
#include "memory.h"
#include "cpu.h"

CHIP8::CHIP8() {
	mem = new Memory();
	cpu = new CPU();

	cpu->memory(mem);
}

void CHIP8::load_rom(string f) {
	// Load ROM into first allowed memory slot (0x200 address)
	mem->load_rom(f, 0x200);
}


int main() {
	CHIP8* emu = new CHIP8;
	return 0;
}

