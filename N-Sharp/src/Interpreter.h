#include "NS.h"

#pragma once
class Interpreter
{
public:
	boost::any runFunction(const string& str, vector<boost::any> arguments);
	
	boost::any getVariable(const string& name);
	void setVariable(const string& name, boost::any any, bool forceGlobal = false);

	bool isFunction(const string& str);
	bool isVariableDeclaration(const string& str);
	bool isMathExpression(const string& str);
	bool isUsingTag(const string& str);
	bool isFunctionDefinition(const string& str);
	bool isIfStatement(const string& str);
	bool isReturnStatement(const string& str);
	bool isGlobalStatement(const string& str);

	void start(vector<string>& code);
	boost::any executeLine(const string& line, vector<string> code, int* curIndex);

private:
	map<string, boost::any> globalVariables;
	map<string, boost::any> localVariables;
	map<string, pair<vector<string>, vector<string>>> functions;
	string currentLine;
	int scopeHeight = 0;
};