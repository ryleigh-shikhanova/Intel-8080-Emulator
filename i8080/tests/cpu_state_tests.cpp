#include <catch2/catch_test_macros.hpp>

#include "cpu.h"
#include "cpu_state.h"
#include "memory.h"

TEST_CASE("Memory is initialized to zero")
{
	Memory memory;

	REQUIRE(memory.read(0x0000) == 0x00);
	REQUIRE(memory.read(0x8000) == 0x00);
	REQUIRE(memory.read(0xFFFF) == 0x00);
}

TEST_CASE("CpuState has the correct initial values")
{
	CpuState state;

	REQUIRE(state.a == 0x00);
	REQUIRE(state.b == 0x00);
	REQUIRE(state.pc == 0x00);
	REQUIRE(state.sp == 0x00);
	REQUIRE(state.flags == 0x02);
	REQUIRE_FALSE(state.halted);
}

TEST_CASE("CPU construction preserves supplied state and memory")
{
	Memory memory;
	CpuState state;
	
	memory.write(0x1234, 0xAB);
	state.pc = 0x1234;

	Cpu8080 cpu{memory, state};

	REQUIRE(memory.read(0x1234) == 0xAB);
	REQUIRE(state.pc == 0x1234);
}

TEST_CASE("CPU flags can be set and checked correctly")
{
	Memory memory;
	CpuState state;
	
	Cpu8080 cpu{memory, state};

	cpu.setFlag(Flag::Carry, true);
	REQUIRE(cpu.isFlagSet(Flag::Carry));

	cpu.setFlag(Flag::Parity, true);
	REQUIRE(cpu.isFlagSet(Flag::Parity));

	cpu.setFlag(Flag::AuxCarry, true);
	REQUIRE(cpu.isFlagSet(Flag::AuxCarry));

	cpu.setFlag(Flag::Zero, true);
	REQUIRE(cpu.isFlagSet(Flag::Zero));

	cpu.setFlag(Flag::Sign, true);
	REQUIRE(cpu.isFlagSet(Flag::Sign));
}

TEST_CASE("CPU flags can be disabled correctly")
{
	Memory memory;
	CpuState state;

	Cpu8080 cpu{memory, state};

	//First all flags will be enabled, then the flags register will be checked as they are disabled
	cpu.setFlag(Flag::Carry, true);
	cpu.setFlag(Flag::Parity, true);
	cpu.setFlag(Flag::AuxCarry, true);
	cpu.setFlag(Flag::Zero, true);
	cpu.setFlag(Flag::Sign, true);

	cpu.setFlag(Flag::Carry, false);
	REQUIRE(!cpu.isFlagSet(Flag::Carry));

	cpu.setFlag(Flag::Parity, false);
	REQUIRE(!cpu.isFlagSet(Flag::Parity));

	cpu.setFlag(Flag::AuxCarry, false);
	REQUIRE(!cpu.isFlagSet(Flag::AuxCarry));

	cpu.setFlag(Flag::Zero, false);
	REQUIRE(!cpu.isFlagSet(Flag::Zero));

	cpu.setFlag(Flag::Sign, false);
	REQUIRE(!cpu.isFlagSet(Flag::Sign));
}

TEST_CASE("HLT halts the cpu and returns a 0 exit code")
{
	Memory memory;
	CpuState state;

	memory.write(0x0000, 0x76); //writes the halt instruction
	state.pc = 0x00;

	Cpu8080 cpu{memory, state};

	auto exit_code = cpu.run();
	REQUIRE(exit_code == 0);
}

TEST_CASE("NOP advances the program counter")
{
	Memory memory;
	CpuState state;

	memory.write(0x0000, 0x00);
	state.pc = 0x00;

	Cpu8080 cpu{memory, state};

	auto result = cpu.step();

	REQUIRE(state.pc == 0x0001);
	REQUIRE(result.opcode == 0x00);
	REQUIRE(result.tStates == 4);
}
