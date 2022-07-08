#include <iostream>
#include <fstream>
#include <string>
#include "memory.h"

using namespace std;

Memory::Memory() {
	mem = vector<unsigned char>(MEMSIZE, 0);
	rom_loaded = false;


	// Memory from address 0x0 and up contains
	// font sprites for all the hex characters 0-F
	const unsigned char sprites[] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	for (int i = 0; i < sizeof(sprites); i++) {
		mem[i] = sprites[i];
	}
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
		chip8file.read((char*) &mem[start_pos], fileSize);

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

