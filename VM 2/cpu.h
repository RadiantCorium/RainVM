#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <vector>

typedef uint32_t u32;

class Cpu {
	std::vector<u32> memory;
	u32 pc = 0;			// Program Counter
	u32 sp = 100;		// Stack Pointer
	u32 a = 0;			// A Register
	u32 b = 0;			// B Register
	u32 c = 0;			// C Register
	u32 r = 0;			// RESULT Register
	u32 ri = 0;			// RESETIGNORE Register (doesn't get reset with the RESET instruction)
	u32 ci = 0;			// CurrentInstruction Register
	u32 running = 1;	// Running Flag
	u32 si = 0;			// SILENT Flag

	void fetch();
	void decode();
	void execute();
	void moveToRegister(u32 registerNum, u32 data);
	u32 getFromRegister(u32 registerNum);

public:
	Cpu();
	void reset();
	void loadProgram(std::vector<u32> prog);
	void run();
};

#endif // !CPU_H
