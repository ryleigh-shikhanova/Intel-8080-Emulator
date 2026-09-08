
#include <catch2/catch_test_macros.hpp>

#include "cpu.h"
#include "cpu_state.h"
#include "memory.h"

TEST_CASE("MOV B, C advances the PC")
{
	Memory memory;
  CpuState state;
  
  memory.write(0x0000, 0x41);
  state.pc = 0x0000;
  state.c = 0x01;

  REQUIRE(state.b == 0x00);
  REQUIRE(state.c == 0x01);
  
  Cpu8080 cpu{memory, state};
  auto result = cpu.step();

  REQUIRE(state.b  == 0x01);
  REQUIRE(state.c  == 0x01);
  REQUIRE(state.pc == 0x01);
  REQUIRE(result.opcode == 0x41);
}
