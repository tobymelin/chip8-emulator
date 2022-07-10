#ifndef EMU_CPU_H
#define EMU_CPU_H

#include "memory.h"
#include "io.h"

class CPU {
	private:
		uint16_t PC; // Program Counter (address register)
		uint16_t DT; // Timer
		uint16_t ST; // Sound timer
		uint16_t I; // Index register
		uint16_t VX[16]; // 16-bit variable register V0 to VF
		uint16_t stack[16]; // Stack
		uint16_t SP; // Stack pointer

		Memory *mem;
		IO *io;
 	public:
		CPU();

		void memory(Memory *mem_) { mem = mem_; };
		void io(IO *io_) { io = io_; };

		bool emulate();
};

#endif

