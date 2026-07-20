#pragma once
#include <string>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <cstdint>
#include <math.h>

//		FLAGS register order
// -------------------------------+
//	7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
//	S | Z | 0 | A | 0 | P | 1 | C |
//--------------------------------+

enum operation{ ADD, SUB, AND, OR, XOR};

struct reg {
	uint8_t data;
};

//For this implementation of the 8080 the following is assumed to be true:
//	1. Program memory begins at 0x00
//	2. The stack pointer (can) be placed in program memory, to do this is a fatal mistake. The stack pointer (should) be placed after program
//		memory. This particular implementation makes use of a varialbe, programEnd, which denotes the value in memory that contains the last
//		program instruction. As such, a logical statck pointer placement is anywhere in the range [programEnd + 1, 0xFF]
class c8080
{
public:
	//Registers
	reg A, B, C, D, E, H, L, FLAGS;

	//Memory
	uint8_t* mem = (uint8_t *)calloc(0xFFFF, sizeof(uint8_t));

	//Stack pointer
	uint16_t sp;

	//program counter
	uint16_t pc;

	//program end tells the emulator where the last program instruction is, this is set by loadProgram
	uint16_t programEnd; 

	//cycle counter
	uint16_t cycles = 0x00;

	bool interruptEnable = 0; //1 = true, 0 = false

	c8080() {
		sp = 0x00;
		pc = 0x00;
		A.data = 0x00;
		B.data = 0x00;
		C.data = 0x00;
		D.data = 0x00;
		E.data = 0x00;
		H.data = 0x00;
		L.data = 0x00;
		resetFlags();
		memset(mem, 0, sizeof(mem));
	}
	~c8080() {
		free(mem);
	}

	//i/o
	//formatted as High 8 bits are the port or device number, low 8 bits are data
	uint16_t output = 0x00; 
	uint16_t input = 0x00;

	bool out = false;
	bool in = false;

	int cycle();

	inline void mov(reg& f, reg& s);
	inline void mov(reg& f, uint8_t s);

	inline void add(reg& f, reg& s);
	inline void add(reg& f, uint8_t s);

	inline void adc(reg& f, reg& s);
	inline void adc(reg& f, uint8_t s);

	inline void sub(reg& f, reg& s);
	inline void sub(reg& f, uint8_t s);

	inline void sbb(reg& f, reg& s);
	inline void sbb(reg& f, uint8_t s);

	inline void ana(reg& f, reg& s);
	inline void ana(reg& f, uint8_t s);

	inline void xra(reg& f, reg& s);
	inline void xra(reg& f, uint8_t s);

	inline void ora(reg& f, reg& s);
	inline void ora(reg& f, uint8_t s);

	inline void cmp(reg& f, reg& s);
	inline void cmp(reg& f, uint8_t s);

	inline void inr(reg& f);
	inline void dcr(reg& f);

	inline void ret();
	inline void jmp();
	inline void call();
	inline void rst(uint8_t base);

	inline void nop();

	int getFlagStatus(int i);

	void loadProgram(uint16_t startAddr, std::string program);

	//flag set functions
	//All flags that are affected by carry support the inclusion of a carry bit by the uint8_t value c
	inline void setZeroFlag(uint8_t f);
	inline void setACFlag(uint8_t f, uint8_t s, uint8_t c = 0, operation op = ADD);
	inline void setSignFlag(uint8_t f);
	inline void setCarryFlag(uint8_t f, uint8_t s, uint8_t c = 0, operation op = ADD);
	inline void setParityFlag(uint8_t f);

	void resetFlags();
	void reset();

	uint16_t getM();
	int calculateParity(uint16_t f);
};

