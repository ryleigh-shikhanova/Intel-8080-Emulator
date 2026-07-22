#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <array>

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

private:
	std::array<std::uint8_t, 0x10000> data_{};

};
#endif
