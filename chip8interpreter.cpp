#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
	ifstream chip8file;

	// Load file from input or default ROM if no additional arguments provided
	if (argc > 1)
		chip8file.open(argv[1], ios::binary);
	else
		chip8file.open("roms/Pong [Paul Vervalin, 1990].ch8", ios::binary);

	if (!chip8file.is_open()) {
		cout << "Could not open ROM.";
		return 1;
	}

	// Not exactly sized to CHIP-8 register sizes
	// short = 2 bytes
	// int = 4 bytes
	// 
	// CHIP-8 spec:
	// VX = 16 8-bit registers
	// I = 12-bit address register (can be 16-bit, but will mainly use leftmost 12 bits)
	// DT = 8-bit delay timer
	// ST = 8-bit sound timer;
	// PC = 16-bit program counter (currently executing address)
	// SP = 8-bit stack pointer (points to topmost level of the stack)
	unsigned short int VX[16], DT, ST;
	unsigned short int PC = 0;
	unsigned short int stack[16];
	unsigned short int *SP = &stack[0];
	unsigned int I, temp;

	streampos fileSize;

	chip8file.seekg(0, ios::end);
	fileSize = chip8file.tellg();
	chip8file.seekg(0, ios::beg);

	vector<unsigned char> fileData(fileSize);
	chip8file.read((char*) &fileData[0], fileSize);

	unsigned int addr1, addr2, addrCombined;

	for (int i = 0; i < fileSize; i += 2) {
		addrCombined = (fileData[i] << 8) + fileData[i+1];
		printf("%X   %X  -  ", i + 0x200, addrCombined);

		switch (fileData[i] & 0xF0) {
			case 0x00:
				if (addrCombined == 0x00E0) {
					printf("CLEAR SCREEN");
					// TODO: Implement once graphics is implemented
				}
				else if (addrCombined == 0x00EE) {
					printf("return;");
					PC = *SP;
					SP = SP > &stack[0] ? SP - 16 : &stack[0];
				}
				else {
					printf("CALL ROUTINE AT ADDRESS %.3X", addrCombined & 0xFFF);
					PC = addrCombined & 0xFFF;
				}
				break;
			case 0x10:
				printf("GOTO %X", addrCombined & 0xFFF);
				PC = addrCombined & 0xFFF;
				break;
			case 0x20:
				printf("*(0x%X)()", addrCombined & 0xFFF);
				SP = SP + 16;
				*SP = PC;
				PC = addrCombined & 0xFFF;
				break;
			case 0x30:
				printf("Skip next if V%X == %X", (fileData[i] & 0xF), fileData[i+1]);
				if (VX[fileData[i] & 0xF] == fileData[i+1])
					PC += 16;
				break;
			case 0x40:
				printf("Skip next if V%X != %X", (fileData[i] & 0xF), fileData[i+1]);
				if (VX[fileData[i] & 0xF] != fileData[i+1])
					PC += 16;
				break;
			case 0x50:
				printf("Skip next if V%X == V%X", (fileData[i] & 0xF), (fileData[i+1] & 0xF0));
				if (VX[fileData[i] & 0xF] == VX[(fileData[i+1] & 0xF0) >> 4])
					PC += 16;
				break;
			case 0x60:
				printf("V%.1X = %X", (fileData[i] & 0xF), fileData[i+1]);
				VX[fileData[i] & 0xF] = fileData[i+1];
				break;
			case 0x70:
				printf("V%.1X += %X", (fileData[i] & 0xF), fileData[i+1]);
				VX[fileData[i] & 0xF] += fileData[i+1];
				break;
			case 0x80:
				addr1 = fileData[i] & 0xF;
				addr2 = (fileData[i+1] & 0xF0) >> 4;

				// Last digit in 0x8 hex code determines operator
				switch (fileData[i+1] & 0xF) {
					case 0x0:
						printf("V%.1X = V%.1X", addr1, addr2);
						VX[addr1] = VX[addr2];
						break;
					case 0x1:
						printf("V%.1X |= V%.1X", addr1, addr2);
						VX[addr1] = VX[addr1] | VX[addr2];
						break;
					case 0x2:
						printf("V%.1X &= V%.1X", addr1, addr2);
						VX[addr1] = VX[addr1] & VX[addr2];
						break;
					case 0x3:
						printf("V%.1X ^= V%.1X", addr1, addr2);
						VX[addr1] = VX[addr1] ^ VX[addr2];
						break;
					case 0x4:
						printf("V%.1X += V%.1X", addr1, addr2);
						temp = VX[addr1] + VX[addr2];

						// Set VF to 1 if there is a carry, else 0
						if (temp > 0xFF)
							VX[0xF] = 1;
						else
							VX[0xF] = 0;

						VX[addr1] = temp & 0xFF;

						break;
					case 0x5:
						printf("V%.1X -= V%.1X", addr1, addr2);

						VX[addr1] = (VX[addr1] - VX[addr2]) % 0x100;

						// Set VF to 0 if there is a borrow, else 1
						// TODO: Correct? Contradicting information on whether this should be >= or >
						// Current interpretation: Borrow can only happen if VX[addr2] > VX[addr1]
						// ie. strictly greater than.
						if (VX[addr1] >= VX[addr2])
							VX[0xF] = 1;
						else
							VX[0xF] = 0;

						break;
					case 0x6: // Undocumented opcode, VX = VY >> 1 in CHIP-8, VX = VX >> 1 in CHIP-48/SCHIP
						printf("VF = V%.1X & 0xF, V%.1X >> 1", addr2, addr2);

						VX[0xF] = VX[addr2] & 0b1;
						VX[addr1] = VX[addr2] >> 1;

						break;
					case 0x7:
						printf("V%.1X = V%.1X - V%.1X", addr1, addr1, addr2);

						VX[addr1] = (VX[addr2] - VX[addr1]) % 0x100;

						// Set VF to 0 if there is a borrow, else 1
						if (VX[addr2] >= VX[addr1])
							VX[0xF] = 1;
						else
							VX[0xF] = 0;

						break;
					case 0xE: // Undocumented opcode, VX = VY >> 1 in CHIP-8, VX = VX >> 1 in CHIP-48/SCHIP
						printf("VF = V%.1X & 0xF0, V%.1X << 1", addr2, addr2);

						VX[0xF] = VX[addr2] & 0b10000000;
						VX[addr1] = VX[addr2] << 1;

						break;
				}
				break;
			case 0x90:
				printf("Skip next if V%X != V%X", fileData[i] & 0xF, (fileData[i+1] & 0xF0) >> 4);

				if (VX[fileData[i] & 0xF] != (fileData[i+1] >> 4))
					PC += 16;

				break;
			case 0xA0:
				printf("I = %X", ((fileData[i] & 0xF) << 8) + fileData[i+1]);
				I = addrCombined & 0xFFF;
				break;
			case 0xB0:
				printf("PC = V0 + %X", ((fileData[i] & 0xF) << 8) + fileData[i+1]);
				PC = VX[0] + addrCombined & 0xFFF;
				break;
			case 0xC0:
				printf("V%X = rand() & %X", fileData[i] & 0xF, fileData[i+1]);
				VX[fileData[i] & 0xF] = rand() % 256;
				break;
			case 0xD0:
				printf("draw(V%X, V%X, %X)", fileData[i] & 0xF, (fileData[i+1] & 0xF0) >> 4, fileData[i+1] & 0xF);
				break;
			case 0xE0:
				if (fileData[i+1] == 0x9E) {
					printf("Skip next if key at V%X is pressed", fileData[i] & 0xF);
				}
				else if (fileData[i+1] == 0xA1) {
					printf("Skip next if key at V%X is NOT pressed", fileData[i] & 0xF);
				}
				break;
			case 0xF0:
				switch (fileData[i+1]) {
					case 0x07:
						printf("Set V%X = DT value", fileData[i] & 0xF);
						break;
					case 0x0A:
						printf("Wait for key press, store in V%X", fileData[i] & 0xF);
						break;
					case 0x15:
						printf("Set DT = V%X", fileData[i] & 0xF);
						DT = fileData[i] & 0xF;
						break;
					case 0x18:
						printf("Set ST = V%X", fileData[i] & 0xF);
						ST = fileData[i] & 0xF;
						break;
					case 0x1E:
						printf("Set I = I + V%X", fileData[i] & 0xF);
						break;
					case 0x29:
						printf("Set I = address of sprite stored in V%X", fileData[i] & 0xF);
						break;
					case 0x33:
						printf("Store BCD value of V%X in I, I+1, I+2", fileData[i] & 0xF);
						break;
					case 0x55:
						printf("Store V0 to V%X in I", fileData[i] & 0xF);
						break;
					case 0x65:
						printf("Fill V0 to V%X with data from I", fileData[i] & 0xF);
						break;
				}
				break;
			}

		cout << endl;
	}

	return 0;
}
