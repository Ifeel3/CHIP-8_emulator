#include "Chip8.hpp"

Chip8::Chip8() : indexRegister(0), programCounter(0x200), stackPointer(0), delayTimer(0), soundTimer(0), opcode(0) {
	memset(memory, 0, sizeof(uint8_t) * 4096);
	memset(registers, 0, sizeof(uint8_t) * 16);
	memset(stack, 0, sizeof(uint16_t) * 16);
	memset(display, 0, sizeof(uint32_t) * 64 * 32);
	keypad = new uint8_t[16];
	SetTables();
	srand(time(NULL));
	LoadFont();
}

void Chip8::Table0() {
	((*this).*(table0[opcode & 0x000F]))();
}

void Chip8::Table8() {
	((*this).*(table8[opcode & 0x000F]))();
}

void Chip8::TableE() {
	((*this).*(tableE[opcode & 0x000F]))();
}

void Chip8::TableF() {
	((*this).*(tableF[opcode & 0x00FF]))();
}

void Chip8::SetTables() {
	table[0x0] = &Chip8::Table0;
	table[0x1] = &Chip8::OP_1nnn;
	table[0x2] = &Chip8::OP_2nnn;
	table[0x3] = &Chip8::OP_3xkk;
	table[0x4] = &Chip8::OP_4xkk;
	table[0x5] = &Chip8::OP_5xy0;
	table[0x6] = &Chip8::OP_6xkk;
	table[0x7] = &Chip8::OP_7xkk;
	table[0x8] = &Chip8::Table8;
	table[0x9] = &Chip8::OP_9xy0;
	table[0xA] = &Chip8::OP_Annn;
	table[0xB] = &Chip8::OP_Bnnn;
	table[0xC] = &Chip8::OP_Cxkk;
	table[0xD] = &Chip8::OP_Dxyn;
	table[0xE] = &Chip8::TableE;
	table[0xF] = &Chip8::TableF;

	for (size_t i = 0; i <= 0xE; i++)
	{
		table0[i] = &Chip8::OP_NULL;
		table8[i] = &Chip8::OP_NULL;
		tableE[i] = &Chip8::OP_NULL;
	}

	table0[0x0] = &Chip8::OP_00E0;
	table0[0xE] = &Chip8::OP_00EE;

	table8[0x0] = &Chip8::OP_8xy0;
	table8[0x1] = &Chip8::OP_8xy1;
	table8[0x2] = &Chip8::OP_8xy2;
	table8[0x3] = &Chip8::OP_8xy3;
	table8[0x4] = &Chip8::OP_8xy4;
	table8[0x5] = &Chip8::OP_8xy5;
	table8[0x6] = &Chip8::OP_8xy6;
	table8[0x7] = &Chip8::OP_8xy7;
	table8[0xE] = &Chip8::OP_8xyE;

	tableE[0x1] = &Chip8::OP_ExA1;
	tableE[0xE] = &Chip8::OP_Ex9E;

	for (size_t i = 0; i <= 0x65; i++)
	{
		tableF[i] = &Chip8::OP_NULL;
	}

	tableF[0x07] = &Chip8::OP_Fx07;
	tableF[0x0A] = &Chip8::OP_Fx0A;
	tableF[0x15] = &Chip8::OP_Fx15;
	tableF[0x18] = &Chip8::OP_Fx18;
	tableF[0x1E] = &Chip8::OP_Fx1E;
	tableF[0x29] = &Chip8::OP_Fx29;
	tableF[0x33] = &Chip8::OP_Fx33;
	tableF[0x55] = &Chip8::OP_Fx55;
	tableF[0x65] = &Chip8::OP_Fx65;
}

Chip8::~Chip8() {delete [] keypad;}

void Chip8::LoadRom(const char *fileName) {
	FILE *rom = fopen(fileName, "rb");
	if (rom != NULL) {
		fread(memory + 0x200, sizeof(uint8_t), 3584, rom);
	}
	fclose(rom);
}

void Chip8::LoadFont() {
	uint8_t fontset[] = {
			0x60, 0x90, 0x90, 0x90, 0x60, // 0
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
	memcpy(memory + 0x50, fontset, 80);
}

uint8_t Chip8::Random() {
	return (uint8_t)(std::rand() % 254);
}

void Chip8::OP_00E0() {
	memset(display, 0, sizeof(display));
}

void Chip8::OP_00EE() {
	--stackPointer;
	programCounter = stack[stackPointer];
}

void Chip8::OP_1nnn() {
	programCounter = opcode & 0x0FFF;
}

void Chip8::OP_2nnn() {
	stack[stackPointer] = programCounter;
	++stackPointer;
	programCounter = opcode & 0x0FFF;
}

void Chip8::OP_3xkk() {
	if (registers[(opcode & 0x0F00) >> 8] == (opcode & 0xFF)) programCounter += 2;
}

void Chip8::OP_4xkk() {
	if (registers[(opcode & 0x0F00) >> 8] != (opcode & 0xFF)) programCounter += 2;
}

void Chip8::OP_5xy0() {
	if (registers[(opcode & 0x0F00) >> 8] == registers[(opcode & 0x00F0) >> 4]) programCounter += 2;
}

void Chip8::OP_6xkk() {
	registers[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
}

void Chip8::OP_7xkk() {
	registers[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
}

void Chip8::OP_8xy0() {
	registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x00F0) >> 4];
}

void Chip8::OP_8xy1() {
	registers[(opcode & 0x0F00) >> 8] |= registers[(opcode & 0x00F0) >> 4];
}

void Chip8::OP_8xy2() {
	registers[(opcode & 0x0F00) >> 8] &= registers[(opcode & 0x00F0) >> 4];
}

void Chip8::OP_8xy3() {
	registers[(opcode & 0x0F00) >> 8] ^= registers[(opcode & 0x00F0) >> 4];
}

void Chip8::OP_8xy4() {
	uint16_t sum = registers[(opcode & 0x0F00) >> 8] + registers[(opcode & 0x00F0) >> 4];
	if (sum > 255) registers[0xF] = 1;
	else registers[0xF] = 0;
	registers[(opcode & 0x0F00) >> 8] = sum & 0x00FF;
}

void Chip8::OP_8xy5() {
	if (registers[(opcode & 0x0F00) >> 8] > registers[(opcode & 0x00F0) >> 4]) registers[0xF] = 1;
	else registers[0xF] = 0;
	registers[(opcode & 0x0F00) >> 8] -= registers[(opcode & 0x00F0) >> 4];
}

void Chip8::OP_8xy6() {
	registers[0xF] = registers[(opcode & 0x0F00) >> 8] & 0b1;
	registers[(opcode & 0x0F00) >> 8] >>= 1;
}

void Chip8::OP_8xy7() {
	if (registers[(opcode & 0x00F0) >> 4] > registers[(opcode & 0x0F00) >> 8]) registers[0xF] = 1;
	else registers[0xF] = 0;
	registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x00F0) >> 4] - registers[(opcode & 0x0F00) >> 8];
}

void Chip8::OP_8xyE() {
	registers[0xF] = registers[(opcode & 0x0F00) >> 8] & 0b1;
	registers[(opcode & 0x0F00) >> 8] <<= 1;
}

void Chip8::OP_9xy0() {
	if (registers[(opcode & 0x0F00) >> 8] != registers[(opcode & 0x00F0) >> 4]) programCounter += 2;
}

void Chip8::OP_Annn() {
	indexRegister = opcode & 0x0FFF;
}

void Chip8::OP_Bnnn() {
	programCounter = registers[0] + (opcode & 0x0FFF);
}

void Chip8::OP_Cxkk() {
	registers[(opcode & 0x0F00) >> 8] = Random() & (opcode & 0x00FF);
}

void Chip8::OP_Dxyn() {
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;
	uint8_t height = opcode & 0x000F;

	// Wrap if going beyond screen boundaries
	uint8_t xPos = registers[Vx] % 64;
	uint8_t yPos = registers[Vy] % 32;

	registers[0xF] = 0;

	for (unsigned int row = 0; row < height; ++row)
	{
		uint8_t spriteByte = memory[indexRegister + row];

		for (unsigned int col = 0; col < 8; ++col)
		{
			uint8_t spritePixel = spriteByte & (0x80 >> col);
			uint32_t* screenPixel = &display[(yPos + row) * 64 + (xPos + col)];

			// Sprite pixel is on
			if (spritePixel)
			{
				// Screen pixel also on - collision
				if (*screenPixel == 0xFFFFFFFF)
				{
					registers[0xF] = 1;
				}

				// Effectively XOR with the sprite pixel
				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	}
}

void Chip8::OP_Ex9E() {
	if (keypad[registers[(opcode & 0x0F00) >> 8]]) programCounter += 2;
}

void Chip8::OP_ExA1() {
	if (!keypad[registers[(opcode & 0x0F00) >> 8]]) programCounter += 2;
}

void Chip8::OP_Fx07() {
	registers[(opcode & 0x0F00) >> 8] = delayTimer;
}

void Chip8::OP_Fx0A() {
	if (keypad[0x0]) registers[(opcode & 0x0F00) >> 8] = 0x0;
	else if (keypad[0x1]) registers[(opcode & 0x0F00) >> 8] = 0x1;
	else if (keypad[0x2]) registers[(opcode & 0x0F00) >> 8] = 0x2;
	else if (keypad[0x3]) registers[(opcode & 0x0F00) >> 8] = 0x3;
	else if (keypad[0x4]) registers[(opcode & 0x0F00) >> 8] = 0x4;
	else if (keypad[0x5]) registers[(opcode & 0x0F00) >> 8] = 0x5;
	else if (keypad[0x6]) registers[(opcode & 0x0F00) >> 8] = 0x6;
	else if (keypad[0x7]) registers[(opcode & 0x0F00) >> 8] = 0x7;
	else if (keypad[0x8]) registers[(opcode & 0x0F00) >> 8] = 0x8;
	else if (keypad[0x9]) registers[(opcode & 0x0F00) >> 8] = 0x9;
	else if (keypad[0xA]) registers[(opcode & 0x0F00) >> 8] = 0xA;
	else if (keypad[0xB]) registers[(opcode & 0x0F00) >> 8] = 0xB;
	else if (keypad[0xC]) registers[(opcode & 0x0F00) >> 8] = 0xC;
	else if (keypad[0xD]) registers[(opcode & 0x0F00) >> 8] = 0xD;
	else if (keypad[0xE]) registers[(opcode & 0x0F00) >> 8] = 0xE;
	else if (keypad[0xF]) registers[(opcode & 0x0F00) >> 8] = 0xF;
	else programCounter -= 2;
}

void Chip8::OP_Fx15() {
	delayTimer = registers[(opcode & 0x0F00) >> 8];
}

void Chip8::OP_Fx18() {
	soundTimer = registers[(opcode & 0x0F00) >> 8];
}

void Chip8::OP_Fx1E() {
	indexRegister += registers[(opcode & 0x0F00) >> 8];
}

void Chip8::OP_Fx29() {
	indexRegister = 0x50 + (5 * registers[(opcode & 0x0F00) >> 8]);
}

void Chip8::OP_Fx33() {
	uint8_t number = registers[(opcode & 0x0F00) >> 8];

	memory[indexRegister + 2] = number % 10;
	number /= 10;

	memory[indexRegister + 1] = number % 10;
	number /= 10;

	memory[indexRegister] = number % 10;
}

void Chip8::OP_Fx55() {
	for (uint8_t i = 0; i <= (uint8_t)((opcode & 0x0F00) >> 8); ++i) {
		memory[indexRegister + i] = registers[i];
	}
}

void Chip8::OP_Fx65() {
	for (uint8_t i = 0; i <= (uint8_t)((opcode & 0x0F00) >> 8); ++i) {
		registers[i] = memory[indexRegister + i];
	}
}

void Chip8::OP_NULL() {

}

void Chip8::Cycle()
{
	// Fetch
	opcode = (memory[programCounter] << 8) | memory[programCounter + 1];

	// Increment the PC before we execute anything
	programCounter += 2;

	// Decode and Execute
	((*this).*(table[(opcode & 0xF000) >> 12]))();

	// Decrement the delay timer if it's been set
	if (delayTimer > 0) {
		--delayTimer;
	}

	// Decrement the sound timer if it's been set
	if (soundTimer > 0) {
		--soundTimer;
	}
}