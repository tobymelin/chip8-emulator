#ifndef EMU_CHIP8_H
#define EMU_CHIP8_H

#include "memory.h"
#include "cpu.h"
#include "io.h"

class CHIP8 {
	private:
		Memory *mem;
		CPU *cpu;
		IO *io;

	public:
		CHIP8();

		void boot();
		void load_rom(string f);
};

#endif

