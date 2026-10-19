#ifndef CPU_8080_H
#define CPU_8080_H

#include <cstdint>
#include <bit>
#include "cpu_state.h"
#include "memory.h"
#include "i8080_opcode_table.h"

// Used to group the executed opcode along with it's t-states
struct StepResult {
	std::uint16_t address{};
	std::uint8_t opcode{};
	std::uint8_t tStates{};
};

// The helper functions in this class accept the following registers:
// 	A, B, C, D, E, L, H, and M.
// 	M is the pseudo-register defined by the HL register pair
// 	These registers are mapped to an integer as defined by the 8080 hardware spec.
enum class Register : std::uint8_t {
	B = 0,
	C = 1,
	D = 2,
	E = 3,
	H = 4,
	L = 5,
	M = 6,
	A = 7
};

// The helper functions in this class accept the following register pairs:
// 	BC, DE, HL, and SP
//  PUSH/POP decode RP = 3 (0b11) as PSW, not SP. Use StackPair for those.
enum class RegisterPair : std::uint8_t {
	BC = 0,
	DE = 1,
	HL = 2,
	SP = 3
};

// For use with the stack instructions
enum class StackPair : std::uint8_t {
	BC  = 0,
	DE  = 1,
	HL  = 2,
	PSW = 3
};

enum class AluOp : std::uint8_t {
	ADD = 0,
	ADC = 1,
	SUB = 2,
	SBB = 3,
	ANA = 4,
	XRA = 5,
	ORA = 6,
	CMP = 7
};

class Cpu8080 {

//the cpu is given the state and memory. So, the memory should be created
public:
	explicit Cpu8080(Memory& memory, CpuState& state) 
		: memory_(memory), state_(state)
	{
	}
	
	[[nodiscard]]
  StepResult step();

	[[nodiscard]]
  const CpuState& state() const noexcept
	{
		return state_;
	}
	int run();
	void restoreState(const CpuState& state);
	void reset() noexcept;

	[[nodiscard]]
  bool isFlagSet(Flag flag) const noexcept;
	void setFlag(Flag flag, bool value) noexcept;

private:
	CpuState& state_;
	Memory& memory_;
	std::uint64_t tstates_{0};

	// instruction fetching
	[[nodiscard]] std::uint8_t fetchByte() noexcept;
	[[nodiscard]] std::uint16_t fetchWord() noexcept;

	//register access 
	[[nodiscard]] std::uint8_t readRegister(Register reg) const noexcept;
	void writeRegister(Register reg, std::uint8_t value) noexcept;

	[[nodiscard]] std::uint16_t readRegisterPair(RegisterPair pair) const noexcept;
	void writeRegisterPair(RegisterPair pair, std::uint16_t value) noexcept;

	// instruction decoding
	[[nodiscard]] constexpr Register decodeRegister(std::uint8_t opcode) noexcept
	{
		return static_cast<Register>(opcode & 0x07);
	}

	[[nodiscard]] constexpr RegisterPair decodeRegisterPair(std::uint8_t opcode) noexcept
	{
	return static_cast<RegisterPair>((opcode & 0x30) >> 4);
	}

	//flag helpers
	[[nodiscard]] static bool shouldSetZero(std::uint8_t value) noexcept;
	[[nodiscard]] static bool shouldSetSign(std::uint8_t value) noexcept;
	[[nodiscard]] static bool shouldSetParity(std::uint8_t value) noexcept;

	void updateSZPFlags(std::uint8_t value) noexcept;

	// Instruction handlers
	void executeNOP() const noexcept;
	void executeHLT() const noexcept;
	void executeINR(Register reg) noexcept;
	void executeINX(RegisterPair reg) noexcept;
	void executeMOV(Register dst, Register src) noexcept;
	void executeDCR(Register dst) noexcept;
	void executeDCX(RegisterPair dst) noexcept;
	void executeMVI(Register dst, uint8_t data) noexcept;
	void executeALU(AluOp op, Register reg, uint8_t val) noexcept;
};

#endif
