#include "../include/cpu.h"
#include <iostream>

bool Cpu8080::isFlagSet(Flag flag) const {
	const auto mask = static_cast<std::uint8_t>(flag);
	return (flags_ & mask) != 0;
}

void Cpu8080::setFlag(Flag flag, bool value) {
	const auto mask = static_cast<std::uint8_t>(flag);
	
	if (value) {
		flags_ != mask;
	} else {
		flags_ &= static_cast<std::uint8_t>(-mask);
	}
}
