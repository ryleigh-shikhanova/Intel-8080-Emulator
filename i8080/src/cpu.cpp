#include "cpu.h"
#include <iostream>
#include <stdexcept>

[[nodiscard]] StepResult Cpu8080::step() {
	const OpcodeInfo& current_instruction = kOpcodeTable[memory_.read(state_.pc)];
	StepResult step_result;
	step_result.address = state_.pc;
	step_result.opcode = current_instruction.hex;
	bool alternate_timing_taken = false;
	//At this point we will add a function to perform moves-avoiding the switch
	//Likewise, here we will add a function to perform arithmetic operations-avoiding the switch
	switch (current_instruction.hex) {
		case 0x00:	//NOP
			executeNOP();
			state_.pc++;
			break;
		case 0x10:	//NOP*
			executeNOP();
			state_.pc++;
			break;
		case 0x20:	//NOP*
			executeNOP();
			state_.pc++;
			break;
		case 0x30:	//NOP*
			executeNOP();
			state_.pc++;
			break;
		case 0x08:	//NOP*
			executeNOP();
			state_.pc++;
			break;
		case 0x18:	//NOP*
			executeNOP();
			state_.pc++;
			break;
		case 0x28:	//NOP*
			executeNOP();
			state_.pc++;
			break;
		case 0x38:	//NOP*
			executeNOP();
			state_.pc++;
			break;
		case 0x76:	//HLT
			//implement HLT
			break;
		default:
			throw std::runtime_error("OPCODE not in lookup table");
			break;
	}
	auto t_states = alternate_timing_taken
		? current_instruction.t_states_taken
		: current_instruction.t_states;
	this->tstates_ += t_states;
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
int Cpu8080::run() {
	StepResult step_result{};
	bool running = true;
	int exit_state = -1;
	while(running){
		step_result = step();
		if (step_result.opcode == 0x76){
			exit_state = 0;
			running = false;
		}
	}	
	return exit_state;
}

void Cpu8080::executeNOP() const noexcept {
	//NOP does nothing
}

void Cpu8080::executeHLT() const noexcept {
	//
}
