#include <iostream>

#include "../include/cpu.h"
#include "../include/cpu_state.h"
#include "../include/memory.h"

int main() 
{
	Cpu8080 i8080;
	CpuState state;
	Memory mem;
	
	std::cout << i8080.isFlagSet(Flag::Carry) << std::endl;	
}
