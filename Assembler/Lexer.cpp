#include "Lexer.h"
#include <string>

instructions Lexer::lex(string s) {
	return convertToInstructions(splitToTokens(splitToLines(s)));
}

strings Lexer::splitToLines(string s) {
	size_t pos = 0;
	string token;
	string delimiter = "\n";
	strings tokens;
	while ((pos = s.find(delimiter)) != string::npos) {
		token = s.substr(0, pos);
		tokens.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	tokens.push_back(s);
	return tokens;
}

strings Lexer::splitToTokens(strings s) {
	strings tokens;
	for(string line : s) {
		size_t pos = 0;
		string token;
		string delimiter = " ";
		while ((pos = line.find(delimiter)) != string::npos) {
			token = line.substr(0, pos);
			tokens.push_back(token);
			line.erase(0, pos + delimiter.length());
		}
		tokens.push_back(line);
		tokens.push_back("NL");
	}
	return tokens;
}

instructions Lexer::convertToInstructions(strings s) {
	instructions instructions;
	for (string token : s) {
		// i need to use if/else if statements because you can't switch a string in CPP for some reason
		if (token == "HALT") {
			instructions.push_back(0x10000000);
		}
		else if (token == "RESET") {
			instructions.push_back(0x10000001);
		}
		else if (token == "NL") {
			// wasts space lol
			// instructions.push_back(0x10000002);
		}
		else if (token == "MOV") {
			instructions.push_back(0x10000003);
		}
		else if (token == "PUSH") {
			instructions.push_back(0x10000004);
		}
		else if (token == "POP") {
			instructions.push_back(0x10000005);
		}
		else if (token == "ADD") {
			instructions.push_back(0x10000006);
		}
		else if (token == "SUB") {
			instructions.push_back(0x10000007);
		}
		else if (token == "MUL") {
			instructions.push_back(0x10000008);
		}
		else if (token == "DIV") {
			instructions.push_back(0x10000009);
		}
		else if (token == "PRINT") {
			instructions.push_back(0x1000000A);
		}
		else if (token == "PRINTASASCII") {
			instructions.push_back(0x1000000B);
		}
		else if (token == "SLEEP") {
			instructions.push_back(0x1000000C);
		}
		else if (token == "REG_A") {
			instructions.push_back(0x20000001);
		}
		else if (token == "REG_B") {
			instructions.push_back(0x20000002);
		}
		else if (token == "REG_C") {
			instructions.push_back(0x20000003);
		}
		else if (token == "REG_R") {
			instructions.push_back(0x20000004);
		}
		else if (token == "REG_RI") {
			instructions.push_back(0x20000005);
		}
		else if (token == "MEMDUMP") {
			instructions.push_back(0x1000000D);
		}
		else if (token == "JUMP") {
			instructions.push_back(0x1000000E);
		}
		else if (token == "SILENT") {
			instructions.push_back(0x30000001);
		}
		else if (token == "UNSILENT") {
			instructions.push_back(0x40000001);
		}
		else if (token == "JUMPIFEQ") {
			instructions.push_back(0x1000000F);
		}
		else if (token == "JUMPIFNEQ") {
			instructions.push_back(0x10000010);
		}
		else if (token == "JUMPIFGT") {
			instructions.push_back(0x10000011);
		}
		else if (token == "JUMPIFLT") {
			instructions.push_back(0x10000012);
		}
		else if (token == "JUMPIFGOE") {
			instructions.push_back(0x10000013);
		}
		else if (token == "JUMPIFLOE") {
			instructions.push_back(0x10000014);
		}
		else {
			bool isNumber = false;
			for (u32 i = 0; i < token.length(); i++) {
				if (!isdigit(token[i])) {
					isNumber = false;
				}
				else {
					isNumber = true;
				}
			}
			if (!isNumber) {
				cout << "INVALID TOKEN FOUND! (" << token << ") PROGRAM MAY STILL WORK." << endl;
			}
			else
			{
				instructions.push_back(stoi(token));
			}
		}
	}
	return instructions;
}