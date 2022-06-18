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

		unsigned int addr1, addr2;
		
		for (int i = 0; i < fileSize; i += 2) {
			// cout << hex << fileData[i];
			printf("%.2X%.2X  -  ", fileData[i], fileData[i+1]);

			switch (fileData[i] & 0xF0) {
				case 0x10:
					printf("GOTO %d", (fileData[i] << 8) + fileData[i+1]);
					break;
				case 0x60:
					printf("V%.1X = %d", (fileData[i] & 0xF), fileData[i+1]);
					break;
				case 0x70:
					printf("V%.1X += %d", (fileData[i] & 0xF), fileData[i+1]);
					break;
				case 0x80:
					addr1 = fileData[i] & 0xF;
					addr2 = (fileData[i+1] & 0xF0) >> 4;

					switch (fileData[i+1] & 0xF) {
						case 0x0:
							printf("V%.1X = V%.1X", addr1, addr2);
							break;
						case 0x1:
							printf("V%.1X |= V%.1X", addr1, addr2);
							break;
					}
					break;
			}

			// if (fileData[i] >= 0x60 && fileData[i] <= 0x6F) {
			// 	printf("  -  V%.1X = %d", (fileData[i] - 0x60), fileData[i+1]);
			// }

			cout << endl;
		}
	}

	return 0;
}
