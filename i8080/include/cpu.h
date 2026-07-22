#ifndef CPU_8080_H
#define CPU_8080_H

#include <cstdint>
#include "cpu_state.h"

class Cpu8080 {

public:
	Cpu8080() {
	
	}
	~Cpu8080() {

	}

	bool isFlagSet(Flag flag) const;
	void setFlag(Flag flag, bool value);
private:
	std::uint8_t flags_ = 0x02;
};

#endif
