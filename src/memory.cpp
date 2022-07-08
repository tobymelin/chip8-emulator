#include <iostream>
#include <fstream>
#include <string>
#include "memory.h"

using namespace std;

Memory::Memory() {
	mem = vector<unsigned char>(MEMSIZE, 0);
	rom_loaded = false;
}


void Memory::load_rom(string f, uint16_t start_pos) {
	ifstream chip8file;

	// Load ROM from roms directory
	chip8file.open("roms/" + f, ios::binary);

	if (chip8file.is_open()) {
		streampos fileSize;

		chip8file.seekg(0, ios::end);
		fileSize = chip8file.tellg();
		chip8file.seekg(0, ios::beg);

		cout << "Loading rom " << f << ", size: " << fileSize << " bytes" << endl;
		chip8file.read((char*) &mem[0], fileSize);

		rom_loaded = true;
	}
	else {
		cout << "Could not open file " << f << endl;
	}
}


uint16_t Memory::fetch_op(uint16_t addr) {
	uint16_t op = mem[addr] << 8 | mem[addr+1];

	return op;
}

uint16_t Memory::read_byte(uint16_t addr) {
	return mem[addr];
}

void Memory::write_byte(uint16_t addr, uint8_t byte) {
	mem[addr] = byte;
}

