#include "cpu.h"
#include <iostream>

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
