#include <cstdint>
#include <exception>
#include <iostream>
#include <string>

#include "cpu.h"
#include "cpu_state.h"
#include "memory.h"
#include "rom_loader.h"

int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <rom file> [load address in hex, default 0]" << std::endl;
		return 1;
	}

	Memory mem;
	try {
		std::uint16_t loadAddress = 0;
		if (argc >= 3) {
			loadAddress = static_cast<std::uint16_t>(std::stoul(argv[2], nullptr, 16));
		}
		mem.load(loadAddress, rom::readFile(argv[1]));
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	CpuState state;
	Cpu8080 i8080(mem, state);
	std::cout << i8080.isFlagSet(Flag::Sign) << std::endl;
}
