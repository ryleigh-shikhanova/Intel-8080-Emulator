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
