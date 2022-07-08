#ifndef EMU_CHIP8_H
#define EMU_CHIP8_H

#include "memory.h"
#include "cpu.h"

class CHIP8 {
	private:
		Memory *mem;
		CPU *cpu;
	public:
		CHIP8();

		void emulate();
		void load_rom(string f);
};

#endif

