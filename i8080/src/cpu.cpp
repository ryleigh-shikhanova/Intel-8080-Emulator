#include "cpu.h"
#include <iostream>

//this function is what advances the cpu and executes instructions
////it returns the state of the CPU after the instruction 
///it will increase the pc AFTER it executes the current instruction located at the PC
[[nodiscard]] StepResult step() {
	const OpcodeInfo& current_instruction = kOpcodeTable[memory_.read(state_.pc)];
	StepResult step_result;
	step_result.address = pc;
	step_result.opcode = current_instruction.hex;
	bool alternate_timing_taken = false;
	switch (current_instruction.hex) {
		case 0x00:
			executeNOP();
			state_.pc++;
			break;
		default:
			break;
	}
	auto t_states = alternate_timing_taken
		? current_instruction.t_states_taken
		: current_instruction.t_states;
	tStates_ += t_states;
	step_result.tStates = t_states;
	return step_result;
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

//by the time that the cpu is run the memory should already have the SP, PC, and program memory set + loaded.
void Cpu8080::run() {

	while(true){
		step();
	}	
}

void Cpu8080::executeNOP() const noexcept {
	//NOP does nothing
}
