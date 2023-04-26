#include "NS.h"

#pragma once
class Interpreter
{
public:
	map<string, NSharpFunction> functions;
	map<string, NSharpVariable> globalVariables;

	pair<string, boost::any> runFunction(const string& str, vector<NSharpVariable> arguments);
	
	pair<string, boost::any> getVariable(const string& name);
	void setVariable(const string& name, NSharpVariable data, bool forceGlobal = false);

	bool isStringConcatenation(const string& str);
	bool isFunction(const string& str);
	bool isVariableDeclaration(const string& str);
	bool isMathExpression(const string& str);
	bool isUsingTag(const string& str);
	bool isFunctionDefinition(const string& str);
	bool isIfStatement(const string& str);
	bool isReturnStatement(const string& str);
	bool isWhileStatement(const string& str);
	bool isArrayDefinition(const string& str);
	bool isVariable(const string& str);

	int searchLocalVariables(const string& name);
	string getLine();

	NSharpVariable start(vector<string>& code, bool isMain = false);
	NSharpVariable executeLine(const string& line, vector<string> code, int* curIndex);

private:
	map<int, map<string, NSharpVariable>> localVariables;
	otherFilesType otherFiles;
	string currentLine;
	string otherClassName = "";
	string otherVariableName = "";
	int scopeHeight = 0;
};