#ifndef EMU_MEMORY_H
#define EMU_MEMORY_H

#include <vector>
#include <string>

using namespace std;

#define MEMSIZE 4096


class Memory {
	private:
		vector<unsigned char> mem;
		bool rom_loaded;

	public:
		Memory();
		void load_rom(string f, uint16_t start_pos);
		uint16_t fetch_op(uint16_t addr);
		uint16_t read_byte(uint16_t addr);

		void write_byte(uint16_t addr, uint8_t byte);
};

#endif

