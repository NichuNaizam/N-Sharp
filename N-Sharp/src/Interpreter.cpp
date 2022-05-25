#include "Interpreter.h"
#include "Utils.h"
#include "Internal.h"
#include "Eval.h"

boost::any Interpreter::runFunction(const string& str, vector<boost::any> arguments) {
	if (startsWith(trim(str), "NS.")) {
		boost::any returnValue = NSFunction(str, arguments, *this);

#if PRINT_LOGS
		logInfo("NS Function " + trim(str) + " has been executed, return value: " + anyAsString(returnValue));
#endif // PRINT_LOGS

		return returnValue;
	}
	else {
		if (functions.find(trim(str)) != functions.end()) {
			pair<vector<string>, vector<string>> data = functions[trim(str)];
			vector<string> parameters = data.first;
			vector<string> functionCode = data.second;
			boost::any returnValue = NULL;

			if (arguments.size() < parameters.size() || arguments.size() > parameters.size()) {
				logInfo(anyAsString(arguments[0]));
				logScriptError("Function " + str + " does not take " + to_string(arguments.size()) + " arguments", getLineNo() - 1);
			}

			if ((int)arguments.size() > 0) {
				for (int i = 0; i < (int)parameters.size(); i++) {
					localVariables[scopeHeight + 1][parameters[i]] = arguments[i];
				}
			}

			int functionIndex = functionLineNo[trim(str)];
			returnValue = start(functionCode, functionIndex);
			localVariables.clear();

#if PRINT_LOGS
			logInfo("Function " + trim(str) + " has been executed, return value: " + anyAsString(returnValue));
#endif // PRINT_LOGS

			return returnValue;
		}
		else {
			logScriptError("Undefined function error: ", getLineNo());
		}
	}

	return NULL;
}

boost::any Interpreter::getVariable(const string& name)
{
	int variableScope = searchLocalVariables(name);
	if (scopeHeight > 0 && variableScope != -1) {
		return localVariables[variableScope][name];
	}

	if (globalVariables.find(name) == globalVariables.end()) {
		logScriptError("Variable not defined error: " + name, getLineNo());
	}

	return globalVariables.at(name);
}

void Interpreter::setVariable(const string& name, boost::any any, bool forceGlobal)
{
	if (globalVariables.find(name) != globalVariables.end()) {
		globalVariables[name] = any;
	}
	else if (scopeHeight > 0 && !forceGlobal) {
		localVariables[scopeHeight][name] = any;
	}
	else {
		globalVariables[name] = any;
	}

#if PRINT_LOGS
	logInfo("Variable " + name + " set to " + anyAsString(any));
#endif // PRINT_LOGS
}

int Interpreter::searchLocalVariables(const string& name)
{
	if (scopeHeight > 0) {
		for (int i = 1; i <= scopeHeight; i++) {
			if (localVariables[i].find(name) != localVariables[i].end()) {
				return i;
			}
		}
	}

	return -1;
}

bool Interpreter::isFunction(const string& str) {
	return count(str, '(') > 0 && count(str, ')') > 0 && !isNumber(trim(replace(replace(str, ")", ""), "(", ""))) && count(trim(str), '=') == 0;
}

bool Interpreter::isVariableDeclaration(const string& str)
{
	string predictedVarName = trim(split(replace(str, " ", ""), '=')[0]);
	return startsWith(trim(str), "var ") || (localVariables[scopeHeight].find(predictedVarName) != localVariables[scopeHeight].end() || globalVariables.find(predictedVarName) != globalVariables.end());
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

bool Interpreter::isWhileStatement(const string& str)
{
	return startsWith(trim(str), "while");
}

int Interpreter::getLineNo()
{
	return currentIndex;
}

boost::any Interpreter::start(vector<string>& code, int statementIndex, bool isMain) {
	currentIndex = isMain ? 0 : statementIndex + 2;
	boost::any toReturn = NULL;

	if (!isMain) {
		scopeHeight++;
	}
	
	for (int i = 0; i < (int)code.size(); i++) {
		currentIndex++;
		string line = trim(code[i]);
		toReturn = executeLine(line, code, &i);
	}

	if (!isMain) {
		localVariables[scopeHeight].clear();
		scopeHeight--;
	}

	return toReturn;
}

boost::any Interpreter::executeLine(const string& str, vector<string> code, int* curIndex) {
	if (startsWith(str, "//") || str.empty()) return NULL;

#if PRINT_LOGS
	logInfo("Executing line: " + trim(str));
#endif // PRINT_LOGS


	if (isUsingTag(str)) {
		string path = parseUsingTag(str);
		vector<string> code = readFromFile(path);

#if PRINT_LOGS
		logInfo("Executing using tag, path: " + trim(path));
#endif // PRINT_LOGS

		start(code, 0, true);

#if PRINT_LOGS
		logInfo("Completed executing using tag, path: " + trim(path));
#endif // PRINT_LOGS
	}
	else if (isWhileStatement(str)) {
		string statement = parseWhileStatement(str);
		vector<string> statementCode = parseCodeInsideBrackets(code, curIndex);

#if PRINT_LOGS
		logInfo("Executing while loop, statement: " + trim(statement));
#endif // PRINT_LOGS

		while (evaluateBoolExpression(statement, *this)) {
			start(statementCode, getLineNo());
		}
	}
	else if (isGlobalStatement(str)) {
		string variableName = trim(replace(split(str, '=')[0], "global ", ""));
		boost::any data = getAnyFromParameter(trim(split(str, '=')[1]), *this);

		if (startsWith(trim(str), "global ") && globalVariables.find(variableName) != globalVariables.end()) {
			logScriptError("Global variable " + variableName + " is already defined", getLineNo());
		}

#if PRINT_LOGS
		logInfo("Setting global variable " + trim(variableName));
#endif // PRINT_LOGS

		setVariable(variableName, data, true);
	}
	else if (isReturnStatement(str)) {
		string data = parseReturnStatement(str);
		*curIndex = (int)code.size();

#if PRINT_LOGS
		logInfo("Executing return statement, value: " + trim(data));
#endif // PRINT_LOGS

		return getAnyFromParameter(data, *this);
	}
	else if (isIfStatement(str)) {
		string parsed = parseIfStatement(str);
		vector<string> statementCode = parseCodeInsideBrackets(code, curIndex);

#if PRINT_LOGS
		logInfo("Executing if statement, statement: " + trim(parsed));
#endif // PRINT_LOGS

		if (evaluateBoolExpression(parsed, *this)) {
			start(statementCode, getLineNo());
		}
	}
	else if (isFunctionDefinition(str)) {
		if (scopeHeight != 0) {
			logScriptError("You cannot define functions inside of if statements or funtions", getLineNo());
		}

		vector<string> functionCode = parseCodeInsideBrackets(code, curIndex);

		string functionName = parseFunctionDefinition(str);
		string s = trim(replace(split(str, '(')[1], ")", ""));
		vector<string> parameters = splitOutsideDoubleQuotes(s, ',');

		for (int i = 0; i < (int)parameters.size(); i++) {
			parameters[i] = trim(parameters[i]);
		}

#if PRINT_LOGS
		logInfo("Declaring function: " + trim(functionName));
#endif // PRINT_LOGS

		functions[functionName] = make_pair(parameters, functionCode);
		functionLineNo[functionName] = getLineNo();
	}
	else if (isVariableDeclaration(str)) {
		string variableName = parseVariableDeclaration(str);
		boost::any data = getAnyFromParameter(trim(split(str, '=')[1]), *this);

		if (startsWith(trim(str), "var ") && (globalVariables.find(variableName) != globalVariables.end() || searchLocalVariables(variableName) != -1)) {
			logScriptError("Variable " + variableName + " is already defined", getLineNo());
		}

#if PRINT_LOGS
		logInfo("Setting variable: " + trim(variableName));
#endif // PRINT_LOGS

		setVariable(variableName, data);
	}
	else if (isFunction(str)) {
		string functionName = parseFunctionCall(str);
		vector<boost::any> params = getParameters(str, *this);

#if PRINT_LOGS
		logInfo("Executing function: " + trim(functionName));
#endif // PRINT_LOGS

		runFunction(functionName, params);
	}
	else {
		logScriptError("Syntax error", *curIndex);
	}

	return NULL;
}