#include <catch2/catch_test_macros.hpp>

#include "../include/cpu.h"
#include "../include/cpu_state.h"
#include "../include/memory.h"

TEST_CASE("Memory is initialized to zero")
{
	Memory memory;

	REQUIRE(memory.read(0x0000) == 0x00);
	REQUIRE(memory.read(0x8000) == 0x00);
	REQUIRE(memory.read(0xFFFF) == 0x00);
}
