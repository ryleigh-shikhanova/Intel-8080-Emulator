#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <array>
#include <span>

class Memory {
public:
	std::uint8_t read(std::uint16_t address) const noexcept {
		return data_[address];
	}
	void write(std::uint16_t address, std::uint8_t value) noexcept {
		data_[address] = value;
	}

	void clear() noexcept {
		data_.fill(0);
	}

	// Copies bytes into memory starting at address. Throws std::out_of_range
	// if the block would run past the end of the 64K address space.
	void load(std::uint16_t address, std::span<const std::uint8_t> bytes);

private:
	std::array<std::uint8_t, 0x10000> data_{};

};
#endif
