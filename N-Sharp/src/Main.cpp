#define STB_IMAGE_IMPLEMENTATION

#include "NS.h"
#include "Interpreter.h"

int main(int argc, char** argv) {
	// Arguments checks
	if (argc == 1) {
		Interpreter interpreter;
		vector<string> code;
		int curIndex = 0;
		cout << "---------------------------[N-Sharp - " << VERSION << "]-------------------------------" << endl;
		while (true) {
			cout << ">> ";
			string input;
			getline(cin, input);
			code.push_back(input);
			interpreter.executeLine(input, code, &curIndex);
			curIndex++;
		}
	}

	// Loading the script file from arguments
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
	interpreter.start(code, true);

	// Reset terminal color
	setColor(WHITE);
	
	return 0;
}

// Create a variable object (VARIABLE TYPE, VALUE)
NSharpVariable createVariable(string type, boost::any value)
{
	return make_pair(type, value);
}

// Create a class instance (CLASSNAME, map<VARIABLE NAME, NSHARPVARIABLE>, map<FUNCTION NAME, NSHARPFUNCTION>)
NSharpVariable createClassObject(string className, map<string, NSharpVariable> variables, map<string, NSharpFunction> functions)
{
	return createVariable(className, make_pair(functions, variables));
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

void logScriptError(const string& error, string line)
{
	setColor(RED);
	cout << "\n\n-------------------------------------ERROR-------------------------------------" << endl;
	cout << "Line: " << line << endl;
	cout << "Error: " << error << endl;
	setColor(WHITE);
	exit(-1);
}