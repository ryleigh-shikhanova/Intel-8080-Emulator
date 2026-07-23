#ifndef CPU_8080_H
#define CPU_8080_H

#include <cstdint>
#include "cpu_state.h"

// Used to group the executed opcode along with it's t-states
struct StepResult {
	std::uint16_t address{}
	std::uint8_t opcode;
	std::uint8_t tStates;
};

class Cpu8080 {

public:
	Cpu8080() {
	
	}
	~Cpu8080() {

	}
	
	[[nodiscard]] StepResult step();
	[[nodiscard]] const CpuState& state() const noexcept
	{
		return state_;
	}
	[[nodiscard]] bool isFlagSet(Flag flag) const;
	void setFlag(Flag flag, bool value);
private:
	std::uint8_t flags_ = 0x02;
	CpuState state_{};

	void executeNOP() const noexcept;

};

#endif
