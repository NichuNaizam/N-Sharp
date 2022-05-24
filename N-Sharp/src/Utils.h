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
vector<string> readFromFile(const string& path);

string parseUsingTag(const string& str);
string replace(const string& str, const string& toReplace, const string& replaceWith);
string ltrim(const string& str);
string rtrim(const string& str);
string trim(const string& str);

bool isNumber(const string& str);
bool startsWith(const string& str, const string& start);
bool endsWith(const string& str, const string& end);

int count(const string& str, const char& c);