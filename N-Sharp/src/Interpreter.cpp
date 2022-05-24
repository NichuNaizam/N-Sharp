#include "Interpreter.h"
#include "Utils.h"
#include "Internal.h"
#include "Eval.h"

boost::any Interpreter::runFunction(const string& str, vector<boost::any> arguments) {
	if (startsWith(trim(str), "NS.")) {
		return NSFunction(str, arguments, *this);
	}
	else {
		if (functions.find(trim(str)) != functions.end()) {
			pair<vector<string>, vector<string>> data = functions[trim(str)];
			vector<string> parameters = data.first;
			vector<string> functionCode = data.second;
			boost::any returnValue = NULL;

			if (arguments.size() < parameters.size() || arguments.size() > parameters.size()) {
				cout << "Function " << str << " does not take " << (int)arguments.size() << " arguments" << endl;
				exit(0);
			}

			if (scopeHeight == 0) localVariables.clear();
			if ((int)arguments.size() > 0) {
				for (int i = 0; i < (int)parameters.size(); i++) {
					localVariables[parameters[i]] = arguments[i];
				}
			}

			scopeHeight++;
			for (int i = 0; i < (int)functionCode.size(); i++) {
				string line = trim(functionCode[i]);

				currentLine = line;
				returnValue = executeLine(line, functionCode, &i);
			}
			scopeHeight--;
			localVariables.clear();

			return returnValue;
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
	if (scopeHeight > 0 && localVariables.find(name) != localVariables.end()) {
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
	if (globalVariables.find(name) != globalVariables.end()) {
		globalVariables[name] = any;
	}
	else if (scopeHeight > 0 && !forceGlobal) {
		localVariables[name] = any;
	}
	else {
		globalVariables[name] = any;
	}
}

bool Interpreter::isFunction(const string& str) {
	return count(str, '(') > 0 && count(str, ')') > 0 && !isNumber(trim(replace(replace(str, ")", ""), "(", ""))) && count(trim(str), '=') == 0;
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

bool Interpreter::isIfStatement(const string& str)
{
	return startsWith(trim(str), "if");
}

bool Interpreter::isReturnStatement(const string& str)
{
	return startsWith(trim(str), "return ");
}

bool Interpreter::isGlobalStatement(const string& str)
{
	return startsWith(trim(str), "global ");
}

void Interpreter::start(vector<string>& code) {
	int i = 0;

	for (i = 0; i < (int)code.size(); i++) {
		string line = trim(code[i]);

		currentLine = line;
		executeLine(line, code, &i);
	}
}

boost::any Interpreter::executeLine(const string& str, vector<string> code, int* curIndex) {
	if (startsWith(str, "//") || str.empty()) return NULL;

	if (isUsingTag(str)) {
		string path = parseUsingTag(str);
		vector<string> code = readFromFile(path);
		start(code);
	}
	else if (isGlobalStatement(str)) {
		string variableName = trim(replace(split(str, '=')[0], "global ", ""));
		boost::any data = getAnyFromParameter(trim(split(str, '=')[1]), *this);

		setVariable(variableName, data, true);
	}
	else if (isReturnStatement(str)) {
		string data = split(trim(str), ' ')[1];
		*curIndex = (int)code.size();

		return getAnyFromParameter(data, *this);
	}
	else if (isIfStatement(str)) {
		string parsed = parseIfStatement(str);
		vector<string> statementCode;
		bool started = false;
		int openings = 0;

		for (int i = *curIndex; i < (int)code.size(); i++) {
			if (count(code[i], '}') > 0) {
				if (openings > 0) {
					statementCode.push_back(code[i]);
					openings--;
					continue;
				}

				started = false;
				*curIndex = i + 1;
				break;
			}

			if (started) {
				statementCode.push_back(code[i]);
			}

			if (count(code[i], '{') > 0) {
				if (started) {
					openings++;
				}
				else {
					started = true;
				}
			}
		}

		if (evaluateBoolExpression(parsed, *this)) {
			scopeHeight++;
			start(statementCode);
			scopeHeight--;
		}
	}
	else if (isFunctionDefinition(str)) {
		if (scopeHeight > 0) {
			cout << "You cannot define functions inside of if statements or funtions" << endl;
			exit(0);
		}

		vector<string> functionCode;
		bool started = false;
		int openings = 0;

		for (int i = *curIndex; i < (int)code.size(); i++) {
			if (count(code[i], '}') > 0) {
				if (openings > 0) {
					functionCode.push_back(code[i]);
					openings--;
					continue;
				}

				started = false;
				*curIndex = i + 1;
				break;
			}

			if (started) {
				functionCode.push_back(code[i]);
			}

			if (count(code[i], '{') > 0) {
				if (started == true) {
					openings++;
				}
				else {
					started = true;
				}
			}
		}

		string s = trim(replace(split(str, '(')[1], ")", ""));
		vector<string> parameters = split(s, ',');
		
		if (parameters[0] == s) parameters.clear();

		for (int i = 0; i < (int)parameters.size(); i++) {
			parameters[i] = trim(parameters[i]);
		}

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
		string functionName = parseFunctionCall(str);
		vector<boost::any> params = getParameters(str, *this);

		runFunction(functionName, params);
	}
	else if (isMathExpression(str)) {
		boost::any answer = evaluateExpression(trim(split(str, '=')[1]), *this);
		string varName = trim(split(str, '=')[0]);

		setVariable(varName, answer);
	}
	else {
		cout << "Syntax error [" << *curIndex + 1 << "]: " << currentLine << endl;
		exit(0);
	}

	return NULL;
}