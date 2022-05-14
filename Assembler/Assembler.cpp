#include <iostream>
#include <fstream>
#include <string>
#include "Lexer.h"

// typedef uint32_t u32;

using namespace std;

int main(int argc, char* argv[]) {
	// check for input errors
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " <txt-filename>" << endl;
		exit(1);
	}

	ifstream infile;
	infile.open(argv[1]);
	if (!infile.is_open()) {
		cout << "Error: could not open [" << argv[1] << "]" << endl;
		exit(1);
	}
	string line;
	string contents;
	while (getline(infile, line)) {
		contents += line + "\n";
	}
	infile.close();

	// parse file
	Lexer lexer;
	instructions lexemes = lexer.lex(contents);

	ofstream ofile;
	string finalS = argv[1];
	finalS = finalS.substr(0, finalS.size() - 4);
	finalS += ".rrom";
	ofile.open(finalS, ios::binary);
	for (u32 i = 0; i < lexemes.size(); i++) {
		ofile.write(reinterpret_cast<char*>(&lexemes[i]), sizeof(32));
	}
	ofile.close();
	cout << "Successfully compiled." << endl;
	system("pause");
	return 0;
}