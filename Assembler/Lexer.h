#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <vector>

using namespace std;

typedef int32_t u32;
typedef std::vector<std::string> strings;
typedef std::vector<u32> instructions;

class Lexer {
	strings splitToLines(string s);
	strings splitToTokens(strings s);
	instructions convertToInstructions(strings s);

public:
	instructions lex(string s);
};

#endif