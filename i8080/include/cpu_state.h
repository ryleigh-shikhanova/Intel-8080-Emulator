#ifndef CPU_STATE_H
#define CPU_STATE_H

/*
 * Mutable archietectural state of an Intel 8080 CPU.
 *
 * Memory and I/O devices are owned seperately from the CPU state.
 */
#include <cstdint>

// Masks that are used to check if a respective flag is turned on in the flags register
enum class Flag : std::uint8_t {
	Carry =    0x01,
	Parity =   0x04,
	AuxCarry = 0x10,
	Zero =     0x40,
	Sign =     0x80
};

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
