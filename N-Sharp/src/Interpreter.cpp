#include "Interpreter.h"
#include "Utils.h"
#include "Internal.h"
#include "Eval.h"

boost::any Interpreter::runFunction(const string& str, vector<boost::any> arguments) {
	if (startsWith(trim(str), "NS.")) {
		return NSFunction(str, arguments);
	}
	else {
		if (functions.find(trim(str)) != functions.end()) {
			pair<vector<string>, vector<string>> data = functions[trim(str)];
			vector<string> parameters = data.first;
			vector<string> functionCode = data.second;

			if (arguments.size() < parameters.size() || arguments.size() > parameters.size()) {
				cout << "function " << str << " does not take " << (int)arguments.size() << " arguments" << endl;
				exit(0);
			}

			localVariables.clear();
			if ((int)arguments.size() > 0) {
				for (int i = 0; i < (int)arguments.size(); i++) {
					localVariables[parameters[i]] = arguments[i];
				}
			}

			onFunction = true;
			start(functionCode);
			onFunction = false;
			localVariables.clear();
		}
		else {
			cout << "Undefined function error: " << str << endl;
			exit(0);
		}
	}

	return NULL;
}

boost::any Interpreter::getVariable(const string& name)
{
	if (onFunction && localVariables.find(name) != localVariables.end()) {
		return localVariables.at(name);
	}

	if (globalVariables.find(name) == globalVariables.end()) {
		std::cout << "Variable not defined error: " << name << endl;
		exit(0);
	}

	return globalVariables.at(name);
}

void Interpreter::setVariable(const string& name, boost::any any, bool forceGlobal)
{
	if (onFunction && !forceGlobal) {
		localVariables[name] = any;
	}
	else {
		globalVariables[name] = any;
	}
}

bool Interpreter::isFunction(const string& str) {
	return count(str, '(') > 0 && count(str, ')') > 0 && !isNumber(trim(replace(replace(str, ")", ""), "(", "")));
}

bool Interpreter::isVariableDeclaration(const string& str)
{
	return startsWith(trim(str), "set ");
}

bool Interpreter::isMathExpression(const string& str)
{
	return count(str, '\"') == 0 && count(str, '=') == 0 && (count(str, '+') != 0 || count(str, '-') != 0 || count(str, '*') != 0 || count(str, '/') != 0 || count(str, '^') != 0);
}

bool Interpreter::isUsingTag(const string& str)
{
	return startsWith(trim(str), "using ");
}

bool Interpreter::isFunctionDefinition(const string& str)
{
	return startsWith(trim(str), "function ") && count(str, '(') > 0 && count(str, ')') > 0;
}

void Interpreter::start(vector<string>& code) {
	if (code.empty()) this->code = code;

	for (const string& line : code) {
		currentLine = line;
		executeLine(line, code);

		curIndex++;
	}
}

void Interpreter::executeLine(const string& str, vector<string> code) {
	if (startsWith(str, "//") || str.empty()) return;

	if (startsWith(str, "{")) {
		insideCurlyBrace = true;
		return;
	}
	else if (startsWith(str, "}")) {
		insideCurlyBrace = false;
		return;
	}

	if (insideCurlyBrace) return;

	if (isUsingTag(str)) {
		string path = parseUsingTag(str);
		vector<string> code = readFromFile(path);
		start(code);
	}
	else if (isFunctionDefinition(str)) {
		vector<string> functionCode;
		bool started = false;

		for (int i = curIndex; i < (int)code.size(); i++) {
			if (count(code[i], '}') > 0) {
				started = false;
				break;
			}

			if (started) {
				functionCode.push_back(code[i]);
			}

			if (count(code[i], '{') > 0) {
				started = true;
			}
		}

		string s = trim(replace(split(str, '(')[1], ")", ""));
		vector<string> parameters = split(s, ',');

		functions[split(replace(str, "function ", ""), '(')[0]] = make_pair(parameters, functionCode);

		if (started) {
			cout << "Syntax error: Missing closing bracket" << endl;
			exit(0);
		}
	}
	else if (isVariableDeclaration(str)) {
		string variableName = trim(replace(split(str, '=')[0], "set ", ""));
		boost::any data = getAnyFromParameter(trim(split(str, '=')[1]), *this);

		setVariable(variableName, data);
	}
	else if (isFunction(str)) {
		string functionName = trim(split(str, '(')[0]);
		vector<boost::any> params = getParameters(str, *this);

		runFunction(functionName, params);
	}
	else if (isMathExpression(str)) {
		boost::any answer = evaluateExpression(trim(split(str, '=')[1]), *this);
		string varName = trim(split(str, '=')[0]);

		setVariable(varName, answer);
	}
	else {
		cout << "Syntax error: " << currentLine << endl;
		exit(0);
	}
}