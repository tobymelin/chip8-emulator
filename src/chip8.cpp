#include "chip8.h"
#include "memory.h"
#include "cpu.h"

CHIP8::CHIP8() {
	mem = new Memory();
	cpu = new CPU();
	io = new IO();

	// Initialise CPU connection to memory and IO
	cpu->memory(mem);
	cpu->io(io);
}

void CHIP8::load_rom(string f) {
	// Load ROM into first allowed memory slot (0x200 address)
	mem->load_rom(f, 0x200);
}

// Boot CHIP8 and run main loop
void CHIP8::boot() {
	while (true) {
		if (!cpu->emulate())
			break;

		if (!io->emulate())
			break;
	}
}

