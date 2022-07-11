#include <iostream>
#include "cpu.h"

using namespace std;

CPU::CPU() {
	// Set PC to the starting address of 
	// the loaded rom.
	PC = 0x200;
}

bool CPU::emulate() {
 	bool retval = true;

	uint16_t op = mem->fetch_op(PC);
	PC += 2;
	uint16_t op_byte1 = (op & 0xFF00) >> 8;
	uint16_t op_byte2 = op & 0xFF;

	uint16_t addr1, addr2, temp;

	//cout << hex << op << endl << op_byte1 << endl << op_byte2 << endl;

	switch (op & 0xF000) {
		case 0x0:
			if (op == 0x00E0) {
				printf("CLEAR SCREEN");
				io->clear_screen();
			}
			else if (op == 0x00EE) {
				printf("return;");
				PC = stack[SP--];
			}
			else {
				// Ignore all other 0x0nnn calls - not used according to
				// some references.
				//
				//printf("CALL ROUTINE AT ADDRESS %.3X", op & 0xFFF);
				//stack[SP++] = PC;
				//PC = op & 0xFFF;
			}
			break;
		case 0x1000:
			printf("GOTO %X", op & 0xFFF);
			PC = op & 0xFFF;
			break;
		case 0x2000:
			printf("*(0x%X)()", op & 0xFFF);
			//SP += 1;
			stack[SP++] = PC;
			PC = op & 0xFFF;
			break;
		case 0x3000:
			printf("Skip next if V%X == %X", (op_byte1 & 0xF), op_byte2);
			if (VX[op_byte1 & 0xF] == op_byte2)
				PC += 2;
			break;
		case 0x4000:
			printf("Skip next if V%X != %X", (op_byte1 & 0xF), op_byte2);
			if (VX[op_byte1 & 0xF] != op_byte2)
				PC += 2;
			break;
		case 0x5000:
			printf("Skip next if V%X == V%X", (op_byte1 & 0xF), (op_byte2 & 0xF0));
			if (VX[op_byte1 & 0xF] == VX[(op_byte2 & 0xF0) >> 4])
				PC += 2;
			break;
		case 0x6000:
			printf("V%.1X = %X", (op_byte1 & 0xF), op_byte2);
			VX[op_byte1 & 0xF] = op_byte2;
			break;
		case 0x7000:
			printf("V%.1X += %X", (op_byte1 & 0xF), op_byte2);
			VX[op_byte1 & 0xF] = (VX[op_byte1 & 0xF] + op_byte2) % 0x100;
			break;
		case 0x8000:
			addr1 = op_byte1 & 0xF;
			addr2 = (op_byte2 & 0xF0) >> 4;

			// Last digit in 0x8 hex code determines operator
			switch (op & 0xF) {
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

					VX[addr1] = temp % 0x100;

					break;
				case 0x5:
					printf("V%.1X -= V%.1X", addr1, addr2);

					// Set VF to 0 if there is a borrow, else 1
					// TODO: Correct? Contradicting information on whether this should be >= or >
					// Current interpretation: Borrow can only happen if VX[addr2] > VX[addr1]
					// ie. strictly greater than.
					if (VX[addr1] > VX[addr2])
						VX[0xF] = 1;
					else
						VX[0xF] = 0;

					VX[addr1] = (VX[addr1] - VX[addr2]) % 0x100;

					break;
				case 0x6: // Undocumented opcode, VX = VY >> 1 in CHIP-8, VX = VX >> 1 in CHIP-48/SCHIP
					printf("VF = V%.1X & 0xF, V%.1X >> 1", addr2, addr2);

					// TODO: Correct? Contradicting information
					VX[0xF] = VX[addr2] & 0b1;
					VX[addr1] = VX[addr2] >> 1;

					break;
				case 0x7:
					printf("V%.1X = V%.1X - V%.1X", addr1, addr1, addr2);

					// Set VF to 0 if there is a borrow, else 1
					if (VX[addr2] > VX[addr1])
						VX[0xF] = 1;
					else
						VX[0xF] = 0;

					VX[addr1] = (VX[addr2] - VX[addr1]) % 0x100;

					break;
				case 0xE: // Undocumented opcode, VX = VY >> 1 in CHIP-8, VX = VX >> 1 in CHIP-48/SCHIP
					printf("VF = V%.1X & 0xF0, V%.1X << 1", addr2, addr2);

					// TODO: Correct? Contradicting information
					VX[0xF] = VX[addr2] & 0b10000000;
					VX[addr1] = VX[addr2] << 1;

					break;
			}
			break;
		case 0x9000:
			printf("Skip next if V%X != V%X", op_byte1 & 0xF, (op_byte2 & 0xF0) >> 4);

			if (VX[op_byte1 & 0xF] != VX[op_byte2 >> 4])
				PC += 2;

			break;
		case 0xA000:
			printf("I = %X", ((op_byte1 & 0xF) << 8) + op_byte2);
			I = op & 0xFFF;
			break;
		case 0xB000:
			printf("PC = V0 + %X", ((op_byte1 & 0xF) << 8) + op_byte2);
			PC = VX[0] + op & 0xFFF;
			break;
		case 0xC000:
			printf("V%X = rand() & %X", op_byte1 & 0xF, op_byte2);
			VX[op_byte1 & 0xF] = (rand() % 0x100) & op_byte2;
			break;
		case 0xD000:
			printf("draw(V%X, V%X, %X)", op_byte1 & 0xF, (op_byte2 & 0xF0) >> 4, op_byte2 & 0xF);
			drw(VX[op_byte1 & 0xF], VX[(op_byte2 & 0xF0) >> 4], op_byte2 & 0xF);
			break;
		case 0xE000:
			if (op_byte2 == 0x9E) {
				// TODO: Implement
				printf("Skip next if key at V%X is pressed", op_byte1 & 0xF);
			}
			else if (op_byte2 == 0xA1) {
				// TODO: Implement
				printf("Skip next if key at V%X is NOT pressed", op_byte1 & 0xF);
			}
			break;
		case 0xF000:
			switch (op_byte2) {
				case 0x07:
					printf("Set V%X = DT value", op_byte1 & 0xF);
					VX[op_byte1 & 0xF] = DT;
					break;
				case 0x0A:
					printf("Wait for key press, store in V%X", op_byte1 & 0xF);
					break;
				case 0x15:
					printf("Set DT = V%X", op_byte1 & 0xF);
					DT = op_byte1 & 0xF;
					break;
				case 0x18:
					printf("Set ST = V%X", op_byte1 & 0xF);
					ST = op_byte1 & 0xF;
					break;
				case 0x1E:
					printf("Set I = I + V%X", op_byte1 & 0xF);
					I = I + VX[op_byte1 & 0xF];
					break;
				case 0x29:
					printf("Set I = address of sprite stored in V%X", op_byte1 & 0xF);
					I = VX[op_byte1 & 0xF] * 0x05;
					break;
				case 0x33:
					printf("Store BCD value of V%X in I, I+1, I+2", op_byte1 & 0xF);

 	 	 	 	 	addr1 = op_byte1 & 0xF;
					temp = (VX[temp] - VX[temp]/100 * 100) / 10;

					mem->write_byte(I, VX[temp] / 100);
					mem->write_byte(I+1, temp);
					mem->write_byte(I+2, VX[temp] - VX[temp]/100*100 - temp * 10);

					break;
				case 0x55:
					printf("Store V0 to V%X starting at mem[I]", op_byte1 & 0xF);

					for (int register_idx = 0; register_idx <= (op_byte1 & 0xF); register_idx++) {
						mem->write_byte(I + register_idx, VX[register_idx]);
					}
					break;
				case 0x65:
					printf("Fill V0 to V%X with data starting from mem[I]", op_byte1 & 0xF);

					for (int register_idx = 0; register_idx <= (op_byte1 & 0xF); register_idx++) {
						VX[register_idx] = mem->read_byte(I + register_idx);
					}
					break;
			}
			break;
		}
	cout << endl;

	return retval;
}


void CPU::drw(uint16_t vx, uint16_t vy, uint16_t N) {
	uint16_t start_x = vx % VID_WIDTH;
	uint16_t start_y = vy % VID_HEIGHT;

	VX[0xF] = 0;

	uint8_t sprite_row;
	bool pixel;

	for (int i = 0; i < N; i++) {
		sprite_row = mem->read_byte(I + i);

		if (start_y + N >= VID_HEIGHT)
			break;

		for (int col = 0; col < 8; col++) {
			if (start_x + col >= VID_WIDTH)
				break;

			// Calculate pixel on/off for each bit
			pixel = (sprite_row & (0x01 << (7 - col))) >> (7 - col);

			// Set pixel on/off and update VF if pixel is already set
 	 	 	VX[0xF] = VX[0xF] | io->change_pixel(start_x + col, start_y + i, pixel);
		}
	}
}

