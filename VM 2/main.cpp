#include <iostream>
#include <fstream>
#include "cpu.h"

using namespace std;

int main(int argc, char* argv[]) {
	string funnyThing = "";
	if (argc < 2) {
		funnyThing = "D:\\source codes\\VM 2\\x64\\Debug\\test.rrom";
	}
	else {
		funnyThing = argv[1];
	}

	u32 i;
	vector<u32> prog;

	try {
		ifstream r(funnyThing, ios::binary);

		while (r.read((char*)&i, sizeof(i))) {
			prog.push_back(i);
		}
	}
	catch (exception e) {
		cout << "ERROR: " << e.what();
	}

	Cpu cpu;
	cpu.loadProgram(prog);
	cpu.run();
	return 0;
}