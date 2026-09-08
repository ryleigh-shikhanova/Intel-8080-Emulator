#include "cpu.h"
#include <iostream>
#include <stdexcept>

[[nodiscard]] StepResult Cpu8080::step() {
	const OpcodeInfo& current_instruction = kOpcodeTable[memory_.read(state_.pc)];
	auto opcode = current_instruction.hex;
	StepResult step_result;
	step_result.address = state_.pc;
	step_result.opcode = opcode;
	bool alternate_timing_taken = false;

	//At this point we will add a function to perform moves-avoiding the switch
	//Likewise, here we will add a function to perform arithmetic operations-avoiding the switch
	if ((opcode & 0xC7) == 0x04){
		//This mask represents the INR instructions
		auto reg = decodeRegister((opcode >> 3) & 0x07);
		executeINR(reg);
		state_.pc++;
	}
	else{
		switch (opcode) {
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
				executeHLT();
        state_.pc++;
				break;
			default:
				throw std::runtime_error("Opcode not implemented");
		}
	}
	auto t_states = alternate_timing_taken
		? current_instruction.t_states_taken
		: current_instruction.t_states;
	this->tstates_ += t_states;
	step_result.tStates = t_states;
	return step_result;
}

[[nodiscard]]
bool Cpu8080::isFlagSet(Flag flag) const noexcept {
	const auto mask = static_cast<std::uint8_t>(flag);
	return (state_.flags & mask) != 0;
}

void Cpu8080::setFlag(Flag flag, bool value) noexcept {
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
	//when logging is implemented this function will simply
  //  write to the log
}

void Cpu8080::executeHLT() const noexcept {
  state_.halted = true;
}

void Cpu8080::executeINR(Register reg) noexcept {
	auto value = readRegister(reg);
    setFlag(Flag::AuxCarry, (value & 0x0F) == 0x0F);
	value++;
	writeRegister(reg, value);
	updateSZPFlags(value);
}

[[nodiscard]]
std::uint8_t Cpu8080::readRegister(Register reg) const noexcept{
	std::uint8_t value{};
	switch(reg){
		case Register::A:
			value = state_.a;
			break;
		case Register::B:
			value = state_.b;
			break;
		case Register::C:
			value = state_.c;
			break;
		case Register::D:
			value = state_.d;
			break;
		case Register::E:
			value = state_.e;
			break;
		case Register::H:
			value = state_.h;
			break;
		case Register::L:
			value = state_.l;
			break;
    	case Register::M:
      		value = memory_.read(readRegisterPair(RegisterPair::HL));
      		break;
	}	
	return value;
}

void Cpu8080::writeRegister(Register reg, std::uint8_t value) noexcept{
	switch(reg){
		case Register::A:
			state_.a = value;
			break;
		case Register::B:
			state_.b = value;
			break;
		case Register::C:
			state_.c = value;
			break;
		case Register::D:
			state_.d = value;
			break;
		case Register::E:
			state_.e = value;
			break;
		case Register::H:
			state_.h = value;
			break;
		case Register::L:
			state_.l = value;
			break;
    	case Register::M:
      		memory_.write(readRegisterPair(RegisterPair::HL), value);
      		break;
	}	
}

[[nodiscard]]
std::uint16_t Cpu8080::readRegisterPair(RegisterPair pair) const noexcept{
	std::uint16_t value{};
	switch(pair){
		case RegisterPair::BC:
			value = (static_cast<std::uint16_t>(state_.b) << 8) 
				      | state_.c;
			break;
		case RegisterPair::DE:
			value = (static_cast<std::uint16_t>(state_.d) << 8) 
				      | state_.e;
			break;
		case RegisterPair::HL:
			value = (static_cast<std::uint16_t>(state_.h) << 8) 
				      | state_.l;
			break;
		case RegisterPair::SP:
      		value = state_.sp; 
			break;
	}
	return value;
}

void Cpu8080::writeRegisterPair(RegisterPair pair, std::uint16_t value) noexcept{
	const auto high = static_cast<std::uint8_t>(value >> 8);
  	const auto low =  static_cast<std::uint8_t>(value & 0xFF);
  	switch(pair){
		case RegisterPair::BC:
      		state_.b = high;
      		state_.c = low;
			break;
		case RegisterPair::DE:
      		state_.d = high;
      		state_.e = low;
			break;
		case RegisterPair::HL:
      		state_.h = high;
      		state_.l = low;
			break;
		case RegisterPair::SP:
      		state_.sp = value;
			break;
	}
}

[[nodiscard]]
    bool Cpu8080::shouldSetZero(std::uint8_t value) noexcept{
      return value == 0;
    }

  [[nodiscard]]
    bool Cpu8080::shouldSetSign(std::uint8_t value) noexcept{
      return (value & 0x80) != 0;
    }

  [[nodiscard]]
    bool Cpu8080::shouldSetParity(std::uint8_t value) noexcept{
      return (std::popcount(value) & 1) == 0;
    }
  
  void Cpu8080::updateSZPFlags(std::uint8_t value) noexcept{
    setFlag(Flag::Zero,   shouldSetZero(value));
    setFlag(Flag::Sign,   shouldSetSign(value));
    setFlag(Flag::Parity, shouldSetParity(value));
  }
