#include "cpu.h"
#include <string>
#include <windows.h>
#include <sstream>
#include <format>
#include <fstream>
#include <filesystem>

using namespace std;

Cpu::Cpu() {
	memory.resize(1000000);
}
void Cpu::reset() {
	cout << "Performing CPU Reset" << endl;
	pc = -1;
	a = 0;
	b = 0;
	c = 0;
	r = 0;
	ci = 0;
	sp = 100;
}

void Cpu::loadProgram(std::vector<u32> prog) {
	memory.clear();
	memory.resize(1000000);
	for (int i = 0; i < prog.size(); i++) {
		memory[pc + 1 + i] = prog[i];
	}
}

void Cpu::run() {
	pc -= 1;
	while (running == 1) {
		fetch();
		decode();
		execute();
	}
	system("pause");
}

void Cpu::fetch() {
	pc++;
}

void Cpu::decode() {

}

void Cpu::execute() {
	char hex_string[20];
	u32 data;
	u32 data2;
	u32 data3;
	// cout << "PC: " << pc << ", 0x" << std::hex << memory[pc] << endl;
	switch (memory[pc]) {
	case 0x10000000: // HALT
		cout << "HALTING CPU!!" << endl;
		running = false;
		break;
	case 0x10000001: // RESET
		reset();
		break;
	case 0x10000002: // NL
		// basically useless, why did i add this?
		// just advance the PC
		// pc++;
		break;
	case 0x10000003: // MOV
		sprintf_s(hex_string, "%X", memory[pc + 1]);
		if (hex_string[0] == '2') {
			data = getFromRegister(stoi(hex_string));
		}
		else {
			data = memory[pc + 1];
		}
		if (si == 0) cout << "MOV " << data << " TO 0x" << hex << memory[pc + 2] << endl;
		moveToRegister(memory[pc + 2], data);
		pc += 2;
		break;
	case 0x10000004: // PUSH
		sprintf_s(hex_string, "%X", memory[pc + 1]);
		if (hex_string[0] == '2') {
			data = getFromRegister(stoi(hex_string));
		}
		else {
			data = memory[pc + 1];
		}
		if (si == 0) cout << "PUSH " << data << endl;
		memory[sp] = data;
		sp++;
		pc += 1;
		break;
	case 0x10000005: // POP
		sp--;
		if (si == 0) cout << "POP " << memory[sp] << endl;
		moveToRegister(0x20000004, memory[sp]);
		pc++;
		break;
	case 0x10000006: // ADD
		sprintf_s(hex_string, "%X", memory[pc + 1]);
		if (hex_string[0] == '2') {
			data = getFromRegister(stoi(hex_string));
		}
		else {
			data = memory[pc + 1];
		}
		sprintf_s(hex_string, "%X", memory[pc + 2]);
		if (hex_string[0] == '2') {
			data2 = getFromRegister(stoi(hex_string));
		}
		else {
			data2 = memory[pc + 2];
		}

		moveToRegister(0x20000004, data + data2);
		if (si == 0) cout << "ADD " << data << ", " << data2 << endl;
		pc += 2;
		break;
	case 0x10000007: // SUB
		sprintf_s(hex_string, "%X", memory[pc + 1]);
		if (hex_string[0] == '2') {
			data = getFromRegister(stoi(hex_string));
		}
		else {
			data = memory[pc + 1];
		}
		sprintf_s(hex_string, "%X", memory[pc + 2]);
		if (hex_string[0] == '2') {
			data2 = getFromRegister(stoi(hex_string));
		}
		else {
			data2 = memory[pc + 2];
		}

		moveToRegister(0x20000004, data - data2);
		if (si == 0) cout << "SUB " << data << ", " << data2 << endl;
		pc += 2;
		break;
	case 0x10000008: // MUL
		sprintf_s(hex_string, "%X", memory[pc + 1]);
		if (hex_string[0] == '2') {
			data = getFromRegister(stoi(hex_string));
		}
		else {
			data = memory[pc + 1];
		}
		sprintf_s(hex_string, "%X", memory[pc + 2]);
		if (hex_string[0] == '2') {
			data2 = getFromRegister(stoi(hex_string));
		}
		else {
			data2 = memory[pc + 2];
		}

		moveToRegister(0x20000004, data * data2);
		if (si == 0) cout << "MUL " << data << ", " << data2 << endl;
		pc += 2;
		break;
	case 0x10000009: // DIV
		sprintf_s(hex_string, "%X", memory[pc + 1]);
		if (hex_string[0] == '2') {
			data = getFromRegister(stoi(hex_string));
		}
		else {
			data = memory[pc + 1];
		}
		sprintf_s(hex_string, "%X", memory[pc + 2]);
		if (hex_string[0] == '2') {
			data2 = getFromRegister(stoi(hex_string));
		}
		else {
			data2 = memory[pc + 2];
		}

		moveToRegister(0x20000004, data / data2);
		if (si == 0) cout << "DIV " << data << ", " << data2 << endl;
		pc += 2;
		break;
	case 0x1000000A: // PRINT
		sprintf_s(hex_string, "%X", memory[pc + 1]);
		if (hex_string[0] == '2') {
			data = getFromRegister(stoi(hex_string));
		}
		else {
			data = memory[pc + 1];
		}
		cout << std::hex << (int)data;
		pc += 1;
		break;
	case 0x1000000B: // PRINTASASCII
		sprintf_s(hex_string, "%X", memory[pc + 1]);
		if (hex_string[0] == '2') {
			data = getFromRegister(stoi(hex_string));
		}
		else {
			data = memory[pc + 1];
		}
		cout << (char)data;
		pc += 1;
		break;
	case 0x1000000C: // SLEEP
		if (si == 0) cout << "SLEEP FOR " << memory[pc + 1] << "MS" << endl;
		Sleep(memory[pc + 1]);
		pc += 1;
		break;
	case 0x1000000D: // DUMP
		cout << "ARE YOU SURE YOU WANT TO PERFORM A MEMORY DUMP? (THIS WILL TAKE A WHILE!) [Y/N] ";
		char choice;
		cin >> choice;
		if (choice == 'y' || choice == 'Y') {
			string memDump;
			char buffer[50];
			char buffer2[50];
			cout << "PROCESSING..." << endl << endl;
			for (u32 i = 0; i < memory.size(); i++) {
				cout << "\b\r";
				cout << (float)i / 10000 << "/" << memory.size() / 10000 << "%";
				memDump += sprintf_s(buffer, "ADRESS %d: %d\n", i, memory[i]);
			}
			ofstream fw("C:\\tmp\\MEMDUMP.txt", std::ofstream::out);
			fw << memDump;
			fw.close();
			cout << endl << "MEMDUMP WRITTEN TO C:\\TMP\\MEMDUMP.TXT" << endl;
		}
		else {
			cout << "MEMORY DUMP SKIPPED." << endl;
		}
		break;
	case 0x1000000E: // JUMP
		sprintf_s(hex_string, "%X", memory[pc + 1]);
		if (hex_string[0] == '2') {
			data = getFromRegister(stoi(hex_string));
		}
		else {
			data = memory[pc + 1];
		}
		pc = data;
		break;
	case 0x30000001: // SILENT
		si = 1;
		break;
	case 0x40000001: // UNSILENT
		si = 0;
		break;
	case 0x1000000F: // JUMPIFEQUAL
		sprintf_s(hex_string, "%X", memory[pc + 1]);
		if (hex_string[0] == '2') {
			data = getFromRegister(stoi(hex_string));
		}
		else {
			data = memory[pc + 1];
		}
		sprintf_s(hex_string, "%X", memory[pc + 2]);
		if (hex_string[0] == '2') {
			data2 = getFromRegister(stoi(hex_string));
		}
		else {
			data2 = memory[pc + 2];
		}
		sprintf_s(hex_string, "%X", memory[pc + 3]);
		if (hex_string[0] == '2') {
			data3 = getFromRegister(stoi(hex_string));
		}
		else {
			data3 = memory[pc + 3];
		}
		if (data == data2) pc = data3;
		break;
	case 0x10000010: // JUMPIFNOTEQUAL
		sprintf_s(hex_string, "%X", memory[pc + 1]);
		if (hex_string[0] == '2') {
			data = getFromRegister(stoi(hex_string));
		}
		else {
			data = memory[pc + 1];
		}
		sprintf_s(hex_string, "%X", memory[pc + 2]);
		if (hex_string[0] == '2') {
			data2 = getFromRegister(stoi(hex_string));
		}
		else {
			data2 = memory[pc + 2];
		}
		sprintf_s(hex_string, "%X", memory[pc + 3]);
		if (hex_string[0] == '2') {
			data3 = getFromRegister(stoi(hex_string));
		}
		else {
			data3 = memory[pc + 3];
		}
		if (data != data2) pc = data3;
		break;
	case 0x10000011: // JUMPIFGREATERTHAN
		sprintf_s(hex_string, "%X", memory[pc + 1]);
		if (hex_string[0] == '2') {
			data = getFromRegister(stoi(hex_string));
		}
		else {
			data = memory[pc + 1];
		}
		sprintf_s(hex_string, "%X", memory[pc + 2]);
		if (hex_string[0] == '2') {
			data2 = getFromRegister(stoi(hex_string));
		}
		else {
			data2 = memory[pc + 2];
		}
		sprintf_s(hex_string, "%X", memory[pc + 3]);
		if (hex_string[0] == '2') {
			data3 = getFromRegister(stoi(hex_string));
		}
		else {
			data3 = memory[pc + 3];
		}
		if (data > data2) pc = data3;
		break;
	case 0x10000012: // JUMPIFLESSTHAN
		sprintf_s(hex_string, "%X", memory[pc + 1]);
		if (hex_string[0] == '2') {
			data = getFromRegister(stoi(hex_string));
		}
		else {
			data = memory[pc + 1];
		}
		sprintf_s(hex_string, "%X", memory[pc + 2]);
		if (hex_string[0] == '2') {
			data2 = getFromRegister(stoi(hex_string));
		}
		else {
			data2 = memory[pc + 2];
		}
		sprintf_s(hex_string, "%X", memory[pc + 3]);
		if (hex_string[0] == '2') {
			data3 = getFromRegister(stoi(hex_string));
		}
		else {
			data3 = memory[pc + 3];
		}
		if (data < data2) pc = data3;
		break;
	case 0x10000013: // JUMPIFGREATEROREQUAL
		sprintf_s(hex_string, "%X", memory[pc + 1]);
		if (hex_string[0] == '2') {
			data = getFromRegister(stoi(hex_string));
		}
		else {
			data = memory[pc + 1];
		}
		sprintf_s(hex_string, "%X", memory[pc + 2]);
		if (hex_string[0] == '2') {
			data2 = getFromRegister(stoi(hex_string));
		}
		else {
			data2 = memory[pc + 2];
		}
		sprintf_s(hex_string, "%X", memory[pc + 3]);
		if (hex_string[0] == '2') {
			data3 = getFromRegister(stoi(hex_string));
		}
		else {
			data3 = memory[pc + 3];
		}
		if (data >= data2) pc = data3;
		break;
	case 0x10000014: // JUMPIFLESSOREQUAL
		sprintf_s(hex_string, "%X", memory[pc + 1]);
		if (hex_string[0] == '2') {
			data = getFromRegister(stoi(hex_string));
		}
		else {
			data = memory[pc + 1];
		}
		sprintf_s(hex_string, "%X", memory[pc + 2]);
		if (hex_string[0] == '2') {
			data2 = getFromRegister(stoi(hex_string));
		}
		else {
			data2 = memory[pc + 2];
		}
		sprintf_s(hex_string, "%X", memory[pc + 3]);
		if (hex_string[0] == '2') {
			data3 = getFromRegister(stoi(hex_string));
		}
		else {
			data3 = memory[pc + 3];
		}
		if (data <= data2) pc = data3;
		break;
	}
}

void Cpu::moveToRegister(u32 registerNum, u32 data) {
	switch (registerNum) {
	case 0x20000001: // A
		a = data;
		break;
	case 0x20000002: // B
		b = data;
		break;
	case 0x20000003: // C
		c = data;
		break;
	case 0x20000004: // R
		r = data;
		break;
	case 0x20000005: // RI
		ri = data;
		break;
	}
}

u32 Cpu::getFromRegister(u32 registerNum) {
	switch (registerNum) {
	case 20000001: // A
		return a;
		break;
	case 20000002: // B
		return b;
		break;
	case 20000003: // C
		return c;
		break;
	case 20000004: // R
		return r;
		break;
	case 20000005: // RI
		return ri;
		break;
	default:
		return 0;
	}
}