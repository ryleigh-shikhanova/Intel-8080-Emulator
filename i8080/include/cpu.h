#ifndef CPU_8080_H
#define CPU_8080_H

#include <cstdint>
#include "cpu_state.h"
#include "memory.h"
#include "i8080_opcode_table.h"

// Used to group the executed opcode along with it's t-states
struct StepResult {
	std::uint16_t address{};
	std::uint8_t opcode;
	std::uint8_t tStates;
};

// The helper functions in this class accept the following registers:
// 	A, B, C, D, E, L, and M.
// 	M is the pseudo-register defined by the HL register pair
enum Register {
	A,	B,
	C,	D,
	E,	H,
	L,	M,
};

// The helper functions in this class accept the following register pairs:
// 	BC, DC, HL, and SP
enum RegisterPair {
	BC,	DC,
	HL,	SP
};

class Cpu8080 {

//the cpu is given the state and memory. So, the memory should be created
public:
	explicit Cpu8080(Memory& memory, CpuState& state) 
		: memory_(memory), state_(state)
	{
	}
	~Cpu8080() {

	}
	

	[[nodiscard]] StepResult step();
	[[nodiscard]] const CpuState& state() const noexcept
	{
		return state_;
	}
	int run();
	void restoreState(const CpuState& state);
	void reset() noexcept;

	[[nodiscard]] bool isFlagSet(Flag flag) const;
	void setFlag(Flag flag, bool value);
private:
	CpuState& state_;
	Memory& memory_;
	std::uint64_t tstates_{0};
	void executeNOP() const noexcept;
	void executeHLT() const noexcept;
	void executeINR(Register reg) noexcept;
	void executeINX(RegisterPair reg) noexcept;

	std::uint8_t readRegister(Register reg) const noexcept;
	void writeRegister(Register reg, std::uint8_t value) noexcept;
};

#endif
