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

class Cpu8080 {

//the cpu is given the state and memory. So, the memory should be created
public:
	explicit Cpu8080(Memory& memory, CpuState& state) 
		: memory_(memory), state_(state)
	{
		tstates_ = 0;
	}
	~Cpu8080() {

	}
	

	[[nodiscard]] StepResult step();
	[[nodiscard]] const CpuState& state() const noexcept
	{
		return state_;
	}
	void run();
	void restoreState(const CpuState& state);
	void reset() noexcept;

	[[nodiscard]] bool isFlagSet(Flag flag) const;
	void setFlag(Flag flag, bool value);
private:
	CpuState& state_;
	Memory& memory_;
	tstates_;
	void executeNOP() const noexcept;

};

#endif
