#ifndef CHIP8_HPP
# define CHIP8_HPP

# include "datatypes.hpp"
# include <cstring>
# include <cstdio>
# include <cstdlib>
# include <ctime>
# include <unistd.h>

class Chip8 {
private:
    uint8_t registers[16];
    uint8_t memory[4096];
    uint16_t indexRegister;
    uint16_t programCounter;
    uint16_t stack[16];
    uint8_t stackPointer;
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint16_t opcode;

	void LoadFont();
	uint8_t Random();

	void SetTables();
	void Table0();
	void Table8();
	void TableE();
	void TableF();

	void OP_00E0();
	void OP_00EE();
	void OP_1nnn();
	void OP_2nnn();
	void OP_3xkk();
	void OP_4xkk();
	void OP_5xy0();
	void OP_6xkk();
	void OP_7xkk();
	void OP_8xy0();
	void OP_8xy1();
	void OP_8xy2();
	void OP_8xy3();
	void OP_8xy4();
	void OP_8xy5();
	void OP_8xy6();
	void OP_8xy7();
	void OP_8xyE();
	void OP_9xy0();
	void OP_Annn();
	void OP_Bnnn();
	void OP_Cxkk();
	void OP_Dxyn();
	void OP_Ex9E();
	void OP_ExA1();
	void OP_Fx07();
	void OP_Fx0A();
	void OP_Fx15();
	void OP_Fx18();
	void OP_Fx1E();
	void OP_Fx29();
	void OP_Fx33();
	void OP_Fx55();
	void OP_Fx65();
	void OP_NULL();

	typedef void (Chip8::*Chip8Func)();
	Chip8Func table[0xF + 1];
	Chip8Func table0[0xE + 1];
	Chip8Func table8[0xE + 1];
	Chip8Func tableE[0xE + 1];
	Chip8Func tableF[0x65 + 1];

public:
	uint8_t *keypad;
	uint32_t display[64 * 32];

	Chip8();
	~Chip8();
	void LoadRom(const char *fileName);
	void Cycle();

};
#endif //CHIP8_HPP