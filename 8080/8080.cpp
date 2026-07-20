#include <iostream>
#include <string>

#include "CPU/c8080.h"
//#include "Debugger\DebuggerC8080.h"

//#include "IO\c8080commandLine.h"
//#include "IO\c8080ioHandler.h"

#define showPerformance false

c8080 i8080;
std::string program = "C30D001AD30113FE00C8C30300112900CD0300DB004F114100CD030079D3000DFE00CA2800C31C0076456E746572206E756D626572206F66206C6F6F70733A2000546865206F75747075742069733A2000";

bool startCPU = false;
bool debugMode = false;
bool stepMode = false;

int main()
{
//	DebuggerC8080  debugger;
//	c8080ioHandler handler;

	i8080.loadProgram(0x0000, program);

	while (i8080.cycle() != -1) {
		//do stuff
		std::cout << i8080.pc << std::endl;
	}
}
