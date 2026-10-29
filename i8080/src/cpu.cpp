#include "cpu.h"
#include <iostream>
#include <stdexcept>

[[nodiscard]] StepResult Cpu8080::step() {
	const auto addr = state_.pc;
	const auto opcode = fetchByte();
	const OpcodeInfo& current_instruction = kOpcodeTable[opcode];

	StepResult step_result;
	step_result.address = addr;
	step_result.opcode = opcode;
	bool alternate_timing_taken = false;

	if ((opcode & 0xC7) == 0x00) {
		// 0x00 plus the undocumented NOP masks
		executeNOP();
	}
	else if ((opcode & 0xC7) == 0x04) {
		auto reg = decodeRegister(opcode >> 3);
		executeINR(reg);
	}
	else if ((opcode & 0xCF) == 0x03) {
		auto reg = decodeRegisterPair(opcode);
		executeINX(reg);
	}
	else if ((opcode & 0xC0) == 0x40 && opcode != 0x76) {
		auto dst = decodeRegister(opcode >> 3);
		auto src = decodeRegister(opcode);
		executeMOV(dst, src);
	}
	else if ((opcode & 0xC7) == 0x05) {
		auto dst = decodeRegister(opcode >> 3);
		executeDCR(dst);
	}
	else if ((opcode & 0xCF) == 0x0B) {
		auto dst = decodeRegisterPair(opcode); 
		executeDCX(dst);
	}
	else if ((opcode & 0xC7) == 0x06) {
		auto dst = decodeRegister(opcode >> 3);
		executeMVI(dst, fetchByte());
	}
	else if ((opcode & 0xCF) == 0x01) {
		auto dst = decodeRegisterPair(opcode);
		executeLXI(dst, fetchWord());
	}
	else if ((opcode & 0xCF) == 0x09) {
		//This mask represents the DAD instructions
		throw std::runtime_error("DAD not implemented");
	}
	else if ((opcode & 0xEF) == 0x02) {
		//This mask represents the STAX instructions (B and D only)
		throw std::runtime_error("STAX not implemented");
	}
	else if ((opcode & 0xEF) == 0x0A) {
		//This mask represents the LDAX instructions (B and D only)
		throw std::runtime_error("LDAX not implemented");
	}
	else if ((opcode & 0xC0) == 0x80) {
		auto op = static_cast<AluOp>((opcode >> 3) & 0x07);
		auto val = readRegister(decodeRegister(opcode));
		executeALU(op, val);
	}
	else if ((opcode & 0xC7) == 0xC6) {
		auto op = static_cast<AluOp>((opcode >> 3) & 0x07);
		auto val = fetchByte();
		executeALU(op, val);
	}
	else if ((opcode & 0xC7) == 0xC0) {
		//This mask represents the conditional return instructions (Rcc)
		//  bits 5-3 select the condition
		throw std::runtime_error("Rcc not implemented");
	}
	else if ((opcode & 0xC7) == 0xC2) {
		//This mask represents the conditional jump instructions (Jcc)
		//  bits 5-3 select the condition
		throw std::runtime_error("Jcc not implemented");
	}
	else if ((opcode & 0xC7) == 0xC4) {
		//This mask represents the conditional call instructions (Ccc)
		//  bits 5-3 select the condition
		throw std::runtime_error("Ccc not implemented");
	}
	else if ((opcode & 0xCF) == 0xC1) {
		//This mask represents the POP instructions (register pair 3 is PSW, not SP)
		throw std::runtime_error("POP not implemented");
	}
	else if ((opcode & 0xCF) == 0xC5) {
		//This mask represents the PUSH instructions (register pair 3 is PSW, not SP)
		throw std::runtime_error("PUSH not implemented");
	}
	else if ((opcode & 0xC7) == 0xC7) {
		//This mask represents the RST instructions
		//  bits 5-3 select the restart vector
		throw std::runtime_error("RST not implemented");
	}
	else {
		switch (opcode) {
			case 0x76:	//HLT
				executeHLT();
				break;
			case 0x07:	//RLC
			case 0x0F:	//RRC
			case 0x17:	//RAL
			case 0x1F:	//RAR
			case 0x22:	//SHLD
			case 0x27:	//DAA
			case 0x2A:	//LHLD
			case 0x2F:	//CMA
			case 0x32:	//STA
			case 0x37:	//STC
			case 0x3A:	//LDA
			case 0x3F:	//CMC
			case 0xC3:	//JMP
			case 0xCB:	//JMP*
			case 0xC9:	//RET
			case 0xD9:	//RET*
			case 0xCD:	//CALL
			case 0xDD:	//CALL*
			case 0xED:	//CALL*
			case 0xFD:	//CALL*
			case 0xD3:	//OUT
			case 0xDB:	//IN
			case 0xE3:	//XTHL
			case 0xE9:	//PCHL
			case 0xEB:	//XCHG
			case 0xF3:	//DI
			case 0xF9:	//SPHL
			case 0xFB:	//EI
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

//by the time that the cpu is run the memory should already have the SP, PC, and program memory set + loaded.
int Cpu8080::run() {
	StepResult step_result{};
	bool running = true;
	int exit_state = -1;
	while (!state_.halted) {
		step_result = step();
		if (step_result.opcode == 0x76) {
			exit_state = 0;
			running = false;
		}
	}
	return exit_state;
}

// Returns the value at the current PC in memory and advances the program counter by 1
[[nodiscard]]
std::uint8_t Cpu8080::fetchByte() noexcept {
	return memory_.read(state_.pc++);
}

[[nodiscard]]
std::uint16_t Cpu8080::fetchWord() noexcept {
	const auto lo = fetchByte();
	const auto hi = fetchByte();
	return static_cast<uint16_t>((hi << 8) | lo);
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

[[nodiscard]]
std::uint8_t Cpu8080::readRegister(Register reg) const noexcept {
	std::uint8_t value{};
	switch (reg) {
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

void Cpu8080::writeRegister(Register reg, std::uint8_t value) noexcept {
	switch (reg) {
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
std::uint16_t Cpu8080::readRegisterPair(RegisterPair pair) const noexcept {
	std::uint16_t value{};
	switch (pair) {
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

void Cpu8080::writeRegisterPair(RegisterPair pair, std::uint16_t value) noexcept {
	const auto high = static_cast<std::uint8_t>(value >> 8);
	const auto low = static_cast<std::uint8_t>(value & 0xFF);
	switch (pair) {
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
bool Cpu8080::shouldSetZero(std::uint8_t value) noexcept {
	return value == 0;
}

[[nodiscard]]
bool Cpu8080::shouldSetSign(std::uint8_t value) noexcept {
	return (value & 0x80) != 0;
}

[[nodiscard]]
bool Cpu8080::shouldSetParity(std::uint8_t value) noexcept {
	return (std::popcount(value) & 1) == 0;
}

void Cpu8080::updateSZPFlags(std::uint8_t value) noexcept {
	setFlag(Flag::Zero,   shouldSetZero(value));
	setFlag(Flag::Sign,   shouldSetSign(value));
	setFlag(Flag::Parity, shouldSetParity(value));
}

void Cpu8080::executeNOP() const noexcept {
	//when logging is implemented this function will simply
	//  write to the log
}

void Cpu8080::executeHLT() noexcept {
	state_.halted = true;
}

void Cpu8080::executeINR(Register reg) noexcept {
	auto value = readRegister(reg);
	setFlag(Flag::AuxCarry, (value & 0x0F) == 0x0F);
	value++;
	writeRegister(reg, value);
	updateSZPFlags(value);
}

void Cpu8080::executeINX(RegisterPair reg) noexcept {
	auto value = readRegisterPair(reg);
	value++;
	writeRegisterPair(reg, value);
}

void Cpu8080::executeMOV(Register dst, Register src) noexcept {
	auto value = readRegister(src);
	writeRegister(dst, value);
}

void Cpu8080::executeDCR(Register dst) noexcept {
	auto value = readRegister(dst);
	setFlag(Flag::AuxCarry, (value & 0x0F) != 0x00);
	value--;
	writeRegister(dst, value);
	updateSZPFlags(value);
}

void Cpu8080::executeDCX(RegisterPair dst) noexcept {
	auto value = readRegisterPair(dst);
	value--;
	writeRegisterPair(dst, value);
}

void Cpu8080::executeMVI(Register dst, std::uint8_t data) noexcept {
	writeRegister(dst, data);
}

void Cpu8080::executeLXI(RegisterPair dst, std::uint16_t val) noexcept {
	writeRegisterPair(dst , val);
}

void Cpu8080::executeALU(AluOp op, std::uint8_t val) noexcept {
	auto accum = readRegister(Register::A);
	switch (op) {
		case AluOp::ADD:
			accum = addByte(accum, val, false);
			break;
		case AluOp::ADC:
			accum = addByte(accum, val, isFlagSet(Flag::Carry));
			break;
		case AluOp::SUB:
			accum = subByte(accum, val, false);
			break;
		case AluOp::SBB:
			accum = subByte(accum, val, isFlagSet(Flag::Carry));
			break;
		case AluOp::ANA:
			accum &= val;
			break;
		case AluOp::XRA:
			accum ^= val;
			break;
		case AluOp::ORA:
			accum |= val;
			break;
		case AluOp::CMP:
			subByte(accum, val, false);
			break;
	}
	writeRegister(Register::A, accum);
}

std::uint8_t Cpu8080::addByte(std::uint8_t a, std::uint8_t value, bool carry) noexcept
{
	const unsigned s = a + value + carry;
	setFlag(Flag::Carry, s > 0xFF);
	//do aux carry flag
	const auto res = static_cast<std::uint8_t>(s);
	updateSZPFlags(res);
	return res;
}

std::uint8_t Cpu8080::subByte(std::uint8_t a, std::uint8_t value, bool borrow) noexcept
{
	const unsigned s = a - value - borrow;
	setFlag(Flag::Carry, s > 0xFF);		//since it's unsigned, it'll wrap if it goes neg
	//do aux carry flag
	const auto res = static_cast<std::uint8_t>(s);
	updateSZPFlags(res);
	return res;
}