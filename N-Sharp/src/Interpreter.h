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

	void start(vector<string>& code);
	void executeLine(const string& line, vector<string> code);

private:
	vector<string> code;
	map<string, boost::any> globalVariables;
	map<string, boost::any> localVariables;
	map<string, pair<vector<string>, vector<string>>> functions;
	string currentLine;
	bool onFunction = false, insideCurlyBrace = false;
	int curIndex = 0;
};