#include <iostream>

#include "cpu.h"
#include "cpu_state.h"
#include "memory.h"

int main() 
{
	Memory mem;
	Cpu8080 i8080(mem);
	std::cout << i8080.isFlagSet(Flag::Sign) << std::endl;	
}
