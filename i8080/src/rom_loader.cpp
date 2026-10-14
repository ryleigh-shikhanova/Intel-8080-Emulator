#include "rom_loader.h"

#include <fstream>
#include <iterator>
#include <stdexcept>

namespace rom {

std::vector<std::uint8_t> readFile(const std::filesystem::path& path) {
	std::ifstream file(path, std::ios::binary);
	if (!file) {
		throw std::runtime_error("Could not open file: " + path.string());
	}

	std::vector<std::uint8_t> bytes(std::istreambuf_iterator<char>(file), {});
	if (file.bad()) {
		throw std::runtime_error("Error while reading file: " + path.string());
	}
	return bytes;
}

}
