#include "NS.h"
#include "Interpreter.h"

int main(int argc, char** argv) {

	// Arguments checks
	if (argc == 1) {
		cout << "You need to provide a script file \"ns script.ns\"" << endl;
		return -1;
	}

	string scriptFile = argv[1];
	if (scriptFile.substr(scriptFile.size() - 3) != ".ns") {
		cout << "You can only provide a .ns script" << endl;
		return -1;
	}

	// Reading the code
	vector<string> code;
	ifstream file;
	
	file.open(scriptFile);
	if (!file.is_open()) {
		cout << "Failed to open file" << endl;
		return -1;
	}

	string line;

	while (!file.eof()) {
		getline(file, line);
		code.push_back(line);
	}

	file.close();

	// Initializing Interpreter
	Interpreter interpreter;
	interpreter.start(code);
	
	return 0;
}