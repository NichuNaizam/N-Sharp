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
	interpreter.start(code, 0, true);

	// Reset terminal color
	setColor(WHITE);
	
	return 0;
}

int scriptColor = WHITE;
void setColor(int color, bool fromScript) {
	if (fromScript) scriptColor = color;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color == -1 ? scriptColor : color);
}

void logInfo(const string& info) {
	setColor(WHITE);
	cout << "[";
	setColor(GREEN);
	cout << "INFO";
	setColor(WHITE);
	cout << "]: ";
	setColor(GREEN);
	cout << info << endl;
	setColor(-1);
}

void logWarning(const string& warning) {
	setColor(WHITE);
	cout << "[";
	setColor(YELLOW);
	cout << "WARNING";
	setColor(WHITE);
	cout << "]: ";
	setColor(YELLOW);
	cout << warning << endl;
	setColor(-1);
}

void logError(const string& error) {
	setColor(WHITE);
	cout << "[";
	setColor(RED);
	cout << "ERROR";
	setColor(WHITE);
	cout << "]: ";
	setColor(RED);
	cout << error << endl;
	setColor(WHITE);
	exit(-1);
}

void logScriptError(const string& error, int lineNo)
{
	setColor(WHITE);
	cout << "[";
	setColor(RED);
	cout << "ERROR";
	setColor(WHITE);
	cout << "][";
	setColor(GREEN);
	cout << lineNo;
	setColor(WHITE);
	cout << "]: ";
	setColor(RED);
	cout << error << endl;
	setColor(WHITE);
	exit(-1);
}