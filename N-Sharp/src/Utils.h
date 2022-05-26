#include "NS.h"
#include "Interpreter.h"
#include "DataTypes.h"

DataType getAnyType(boost::any& any);

string anyAsString(boost::any& any);
int anyAsInt(boost::any& any);
float anyAsFloat(boost::any& any);
bool anyAsBool(boost::any& any);

boost::any getAnyFromParameter(const string& param, Interpreter& interpreter);

vector<boost::any> getParameters(const string& str, Interpreter& interpreter);
vector<string> split(const string& str, const char& c);
vector<string> splitArguments(const string& str);
vector<string> splitMultipleChars(const string& str, vector<char> chars);
vector<string> splitOutsideDoubleQuotes(const string& str, const char& splitChar);
vector<string> parseCodeInsideBrackets(vector<string> code, int* curIndex);
vector<string> readFromFile(const string& path);

string parseUsingTag(const string& str);
string parseIfStatement(const string& str);
string parseFunctionCall(const string& str);
string parseFunctionDefinition(const string& str);
string parseWhileStatement(const string& str);
string parseVariableDeclaration(const string& str);
string parseReturnStatement(const string& str);
string removeParenthesis(const string& str);
string replace(const string& str, const string& toReplace, const string& replaceWith);
string substring(const string& str, int& start, int& end);
string ltrim(const string& str);
string rtrim(const string& str);
string trim(const string& str);
string getExePath();

bool isNumber(const string& str);
bool isFloat(const string& str);
bool startsWith(const string& str, const string& start);
bool endsWith(const string& str, const string& end);
bool contains(const string& str, const string& content);

int count(const string& str, const char& c);