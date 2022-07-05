#include <iostream>
#include <fstream>
#include <string>
#include "memory.h"

using namespace std;

Memory::Memory() {
	//mem(MEMSIZE);
	mem = vector<unsigned char>(MEMSIZE);
	//mem = vector<unsigned char>(MEMSIZE);
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

		cout << fileSize << endl;
		cout << mem[0] << endl;
		chip8file.read((char*) &mem[0], fileSize);
	}
	else {
		cout << "Could not open file " + f << endl;
	}
}

int main() {
	Memory* test;

	test->load_rom("Pong [Paul Vervalin, 1990].ch8", 0);

	cout << MEMSIZE << endl;

	return 0;
}

