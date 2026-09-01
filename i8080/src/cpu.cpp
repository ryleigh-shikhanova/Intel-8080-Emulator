#include "cpu.h"
#include <iostream>

//this function is what advances the cpu and executes instructions
////it returns the state of the CPU after the instruction 
[[nodiscard]] StepResult step() {
	return state();
}

[[nodiscard]] bool Cpu8080::isFlagSet(Flag flag) const {
	const auto mask = static_cast<std::uint8_t>(flag);
	return (state_.flags & mask) != 0;
}

void Cpu8080::setFlag(Flag flag, bool value) {
	const auto mask = static_cast<std::uint8_t>(flag);
	
	if (value) {
		state_.flags |= mask;
	} else {
		state_.flags &= static_cast<std::uint8_t>(~mask);
	}
}

void Cpu8080::executeNOP() const noexcept {
	//NOP does nothing
}
