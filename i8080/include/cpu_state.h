#ifndef CPU_STATE_H
#define CPU_STATE_H
/*
 * Mutable archietectural state of an Intel 8080 CPU.
 *
 * Memory and I/O devices are owned seperately from the CPU state.
 */
#include <cstdint>

struct CpuState {
	std::uint8_t a{};
	std::uint8_t b{};
	std::uint8_t c{};
	std::uint8_t d{};
	std::uint8_t e{};
	std::uint8_t h{};
	std::uint8_t l{};

	std::uint8_t flags{0x02};

	std::uint16_t pc{};
	std::uint16_t sp{};

	bool halted{};
};

#endif
