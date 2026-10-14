#include "memory.h"

#include <algorithm>
#include <stdexcept>

void Memory::load(std::uint16_t address, std::span<const std::uint8_t> bytes) {
	if (bytes.size() > data_.size() - address) {
		throw std::out_of_range("Memory::load: block does not fit in memory");
	}
	std::ranges::copy(bytes, data_.begin() + address);
}
