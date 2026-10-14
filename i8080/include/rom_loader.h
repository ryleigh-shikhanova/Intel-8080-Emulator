#ifndef ROM_LOADER_H
#define ROM_LOADER_H

#include <cstdint>
#include <filesystem>
#include <vector>

namespace rom {

// Reads an entire file as raw bytes. Throws std::runtime_error if the file
// cannot be opened or read.
std::vector<std::uint8_t> readFile(const std::filesystem::path& path);

}
#endif
