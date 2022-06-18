#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
	ifstream chip8file;
	chip8file.open("roms/Pong [Paul Vervalin, 1990].ch8", ios::binary);

	streampos fileSize;

	if (chip8file.is_open()) {
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
					}
					else if (addrCombined == 0x00EE) {
						printf("return;");
					}
					else {
						printf("CALL ROUTINE AT ADDRESS %.3X", addrCombined & 0xFFF);
					}
					break;
				case 0x10:
					printf("GOTO %X", ((fileData[i] & 0xF) << 8) + fileData[i+1]);
					break;
				case 0x20:
					printf("*(0x%X)()", addrCombined & 0xFFF);
					break;
				case 0x30:
					printf("Skip next if V%X == %X", (fileData[i] & 0xF), fileData[i+1]);
					break;
				case 0x40:
					printf("Skip next if V%X != %X", (fileData[i] & 0xF), fileData[i+1]);
					break;
				case 0x50:
					printf("Skip next if V%X = V%X", (fileData[i] & 0xF), (fileData[i+1] & 0xF0));
					break;
				case 0x60:
					printf("V%.1X = %X", (fileData[i] & 0xF), fileData[i+1]);
					break;
				case 0x70:
					printf("V%.1X += %X", (fileData[i] & 0xF), fileData[i+1]);
					break;
				case 0x80:
					addr1 = fileData[i] & 0xF;
					addr2 = (fileData[i+1] & 0xF0) >> 4;

					// Last digit in 0x8 hex code determines operator
					switch (fileData[i+1] & 0xF) {
						case 0x0:
							printf("V%.1X = V%.1X", addr1, addr2);
							break;
						case 0x1:
							printf("V%.1X |= V%.1X", addr1, addr2);
							break;
						case 0x2:
							printf("V%.1X &= V%.1X", addr1, addr2);
							break;
						case 0x3:
							printf("V%.1X ^= V%.1X", addr1, addr2);
							break;
						case 0x4:
							printf("V%.1X += V%.1X", addr1, addr2);
							// Set VF to 1 if there is a carry, else 0
							break;
						case 0x5:
							printf("V%.1X -= V%.1X", addr1, addr2);
							// Set VF to 0 if there is a borrow, else 1
							break;
						case 0x6:
							printf("VF = V%.1X & 0xF, V%.1X >> 1", addr1, addr1);
							break;
						case 0x7:
							printf("V%.1X = V%.1X - V%.1X", addr1, addr1, addr2);
							// Set VF to 0 if there is a borrow, else 1
							break;
						case 0xE:
							printf("VF = V%.1X & 0xF0, V%.1X << 1", addr1, addr1);
							break;
					}
					break;
				case 0x90:
					printf("Skip next if V%X != V%X", addr1 & 0xF, (addr2 & 0xF0) >> 4)
					break;
			}

			cout << endl;
		}
	}

	return 0;
}
