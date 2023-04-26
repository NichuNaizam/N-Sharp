#include "Utils.h"
#include "Interpreter.h"
#include "Eval.h"

string anyAsString(boost::any& any, bool doubleQuotes)
{
	try {
		std::string value = boost::any_cast<string>(any);
		if (doubleQuotes) return "\"" + value + "\"";
		return value;
	}
	catch (exception) {
		try {
			return to_string(boost::any_cast<int>(any));
		}
		catch (exception) {
			try {
				return to_string(boost::any_cast<float>(any));
			}
			catch (exception) {
				try {
					return boost::any_cast<bool>(any) ? "true" : "false";
				}
				catch (exception) {
					try {
						return "(" + to_string(boost::any_cast<glm::vec2>(any).x) + ", " + to_string(boost::any_cast<glm::vec2>(any).y) + ")";
					}
					catch (exception) {
						try {
							vector<NSharpVariable> arr = boost::any_cast<vector<NSharpVariable>>(any);
							string output = "[";

							for (int i = 0; i < (int)arr.size(); i++) {
								output += anyAsString(arr[i].second, true);

								if (i != arr.size() - 1) {
									output += ", ";
								}
							}

							output += "]";
							return output;
						}
						catch (exception) {
							try {
								NSharpClass c = boost::any_cast<NSharpClass>(any);
								json j;
								j["functions"] = c.first;

								return j;
							}
							catch (exception) {
								logError("Unknown Data Type");
							}
						}
					}
				}
			}
		}
	}

	return NULL;
}

int anyAsInt(boost::any& any) {
	try {
		return stoi(boost::any_cast<string>(any));
	}
	catch (exception) {
		try {
			return boost::any_cast<int>(any);
		}
		catch (exception) {
			try {
				return (int)boost::any_cast<float>(any);
			}
			catch (exception) {
				try {
					return boost::any_cast<bool>(any);
				}
				catch (exception) {
					try {
						return (int)boost::any_cast<glm::vec2>(any).x;
					}
					catch (exception) {
						logError("Invalid data type encountered! (anyAsInt)");
					}
				}
			}
		}
	}

	return NULL;
}

float anyAsFloat(boost::any& any)
{
	try {
		return stof(boost::any_cast<string>(any));
	}
	catch (exception) {
		try {
			return (float)boost::any_cast<int>(any);
		}
		catch (exception) {
			try {
				return boost::any_cast<float>(any);
			}
			catch (exception) {
				try {
					return boost::any_cast<bool>(any);
				}
				catch (exception) {
					try {
						return boost::any_cast<glm::vec2>(any).x;
					}
					catch (exception) {
						logError("Invalid data type encountered! (anyAsFloat)");
					}
				}
			}
		}
	}

	return NULL;
}

bool anyAsBool(boost::any& any)
{
	try {
		return !boost::any_cast<string>(any).empty();
	}
	catch (exception) {
		try {
			return boost::any_cast<int>(any) == 1;
		}
		catch (exception) {
			try {
				return boost::any_cast<float>(any) == 1.0f;
			}
			catch (exception) {
				try {
					return boost::any_cast<bool>(any);
				}
				catch (exception) {
					try {
						return boost::any_cast<glm::vec2>(any).x > 0 || boost::any_cast<glm::vec2>(any).y > 0;
					}
					catch (exception) {
						logError("Invalid data type encountered! (anyAsBool)");
					}
				}
			}
		}
	}

	return NULL;
}

glm::vec2 anyAsVector2(boost::any& any)
{
	try {
		return boost::any_cast<glm::vec2>(any);
	}
	catch (exception) {
		return glm::vec2(0, 0);
	}
}

vector<NSharpVariable> anyAsArray(boost::any& any)
{
	try {
		return boost::any_cast<vector<NSharpVariable>>(any);
	}
	catch (exception) {
		return vector<NSharpVariable>();
	}
}

NSharpClass anyAsClass(boost::any& any)
{
	try {
		return boost::any_cast<NSharpClass>(any);
	}
	catch (exception) {
		logError("An Error occured (anyAsClass)");
		return NSharpClass();
	}
}

NSharpVariable getAnyFromParameter(const string& param, Interpreter& interpreter)
{
	if (interpreter.isStringConcatenation(param)) {
#if PRINT_LOGS
		logInfo("Guessing " + trim(param) + " as String Concatenation Expression");
#endif // PRINT_LOGS

		vector<string> params = splitOutsideDoubleQuotes(trim(param), '+');
		std::string newString = "";

		for (string s : params) {
			NSharpVariable v = getAnyFromParameter(s, interpreter);
			newString += anyAsString(v.second);
		}

		return createVariable("string", newString);
	}
	else if (interpreter.isFunction(param)) {
#if PRINT_LOGS
		logInfo("Guessing " + trim(param) + " as function");
#endif // PRINT_LOGS

		string functionName = parseFunctionCall(param);
		vector<NSharpVariable> params = getParameters(param, interpreter);

		return interpreter.runFunction(functionName, params);
	}
	else if (count(param, '"') >= 2) {
#if PRINT_LOGS
		logInfo("Guessing " + trim(param) + " as string");
#endif // PRINT_LOGS

		int start = (int)param.find_first_of('"') + 1;
		int end = (int)param.find_last_of('"') - 1;
		return createVariable("string", substring(param, start, end));
	}
	else if (interpreter.isMathExpression(param)) {
#if PRINT_LOGS
		logInfo("Guessing " + trim(param) + " as math expression");
#endif // PRINT_LOGS

		string expression = replace(param, " ", "");
		bool isInt = false;
		vector<char> ops = { '+', '-', '*', '/', '^' };
		vector<string> splitted = splitMultipleChars(param, ops);

		for (const string& s : splitted) {
			if (isNumber(trim(s)) || isFloat(trim(s))) continue;

			NSharpVariable var = getAnyFromParameter(trim(s), interpreter);
			string value = anyAsString(var.second);

			expression = replace(expression, trim(s), value);
		}

		float value = evaluateExpression(expression);

		return isInt ? createVariable("int", boost::any((int)value)) : createVariable("float", boost::any((float)value));
	}
	else if (param == "true" || param == "false") {
#if PRINT_LOGS
		logInfo("Guessing " + trim(param) + " as bool");
#endif // PRINT_LOGS

		bool state = false;

		if (param == "true") state = true;

		return createVariable("bool", state);
	}
	else if (isNumber(param)) {
#if PRINT_LOGS
		logInfo("Guessing " + trim(param) + " as int");
#endif // PRINT_LOGS

		return createVariable("int", stoi(trim(param)));
	}
	else if (isFloat(trim(param))) {
#if PRINT_LOGS
		logInfo("Guessing " + trim(param) + " as float");
#endif // PRINT_LOGS

		return createVariable("float", stof(trim(param)));
	}
	else {
#if PRINT_LOGS
		logInfo("Guessing " + trim(param) + " as variable");
#endif // PRINT_LOGS

		return interpreter.getVariable(trim(param));
	}
}

vector<NSharpVariable> getParameters(const string& str, Interpreter& interpreter) {
	vector<NSharpVariable> output;
	string s = removeParenthesis(trim(str));
	
	vector<string> arguments = splitArguments(s);
	for (const string& arg : arguments) {
		output.push_back(getAnyFromParameter(trim(arg), interpreter));
	}

	return output;
}

vector<string> split(const string& str, const char& c) {
	if (count(str, c) == 0) {
		return vector<string>{str};
	}

	string temp = "";
	vector<string> words;

	for (int i = 0; i < (int)str.size(); i++) {
		if (str[i] != c) {
			temp += str[i];
		}
		else {
			words.push_back(temp);
			temp = "";
		}
	}
	words.push_back(temp);

	return words;
}

vector<string> splitArguments(const string& str)
{
	vector<string> output;
	string s = trim(str);

	int parenthesis = 0;
	bool doubleQuotes = false;
	const char splitChar = ',';

	if (count(str, splitChar) == 0) {
		if (str.empty()) return vector<string>{};
		else return vector<string>{str};
	}

	string temp = "";
	for (int i = 0; i < (int)s.size(); i++) {
		if (s[i] == '(') {
			parenthesis++;
			temp += s[i];
			continue;
		}
		else if (s[i] == ')') {
			parenthesis--;
			temp += s[i];
			continue;
		}
		else if (s[i] == '"') {
			doubleQuotes = !doubleQuotes;
			temp += s[i];
			continue;
		}
		else if (parenthesis > 0 || doubleQuotes) {
			temp += s[i];
		}

		if (parenthesis > 0 || doubleQuotes) continue;

		if (str[i] != splitChar) {
			temp += s[i];
		}
		else {
			output.push_back(temp);
			temp.clear();
		}
	}

	output.push_back(temp);

	return output;
}

vector<string> splitMultipleChars(const string& str, vector<char> chars)
{
	bool hasChars = false;
	for (int i = 0; i < (int)chars.size(); i++) {
		if (count(str, chars[i]) != 0) {
			hasChars = true;
		}
	}

	if (!hasChars) return vector<string> {str};

	string temp = "";
	vector<string> words;

	for (int i = 0; i < (int)str.size(); i++) {
		bool found = false;

		for (int j = 0; j < (int)chars.size(); j++) {
			if (str[i] == chars[j]) {
				words.push_back(temp);
				temp = "";
				found = true;
				break;
			}
		}

		if (!found) {
			temp += str[i];
		}
	}
	words.push_back(temp);

	return words;
}

vector<string> splitOutsideDoubleQuotes(const string& str, const char& splitChar)
{
	vector<string> output;
	string s = trim(str);

	int parenthesis = 0;
	bool doubleQuotes = false;

	if (count(str, splitChar) == 0) {
		if (str.empty()) return vector<string>{};
		else return vector<string>{str};
	}

	string temp = "";
	for (int i = 0; i < (int)s.size(); i++) {
		if (s[i] == '"') {
			doubleQuotes = !doubleQuotes;
			temp += s[i];
			continue;
		}
		else if (doubleQuotes) {
			temp += s[i];
		}

		if (doubleQuotes) continue;

		if (str[i] != splitChar) {
			temp += s[i];
		}
		else {
			output.push_back(temp);
			temp.clear();
		}
	}

	output.push_back(temp);

	return output;
}

vector<string> parseCodeInsideBrackets(vector<string> code, string curLine, int* curIndex)
{
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

	if (started) {
		logScriptError("Syntax error: Missing closing bracket", curLine);
	}

	return statementCode;
}

vector<string> readFromFile(const string& path)
{
	vector<string> output;
	ifstream file;

	file.open(path);
	if (file.is_open()) {
		string line;

		while (!file.eof()) {
			getline(file, line);
			output.push_back(line);
		}

		file.close();
	}

	return output;
}

VariableType getVariableType(const string& str)
{
	vector<string> splitted = split(trim(str), ' ');
	string dataType = splitted[0];
	string varName = splitted[1];
	string value = trim(split(trim(str), '=')[1]);

	return VariableType(dataType, varName, value);
}

string parseUsingTag(const string& str)
{
	string s = trim(replace(replace(str, "using ", ""), ".", "/")) + ".ns";
	string path = "";

#ifdef _DEBUG
	path = startsWith(s, "NS/") ? "./internal_libraries/" + s : "./test/" + s;
#else
	path = startsWith(s, "NS/") ? getExePath() + "/internal_libraries/" + s : s;
#endif // _DEBUG

	return path;
}

string parseIfStatement(const string& str)
{
	int start = (int)str.find_first_of('(') + 1;
	int end = (int)str.find_last_of(')') - 1;
	return substring(str, start, end);
}

string parseFunctionCall(const string& str)
{
	return trim(split(str, '(')[0]);
}

string parseFunctionDefinition(const string& str)
{
	return replace(replace(split(str, '(')[0], "function ", ""), " ", "");
}

string parseWhileStatement(const string& str)
{
	return trim(replace(removeParenthesis(str), "while", ""));
}

string parseVariableDeclaration(const string& str)
{
	return trim(replace(split(str, '=')[0], "var ", ""));
}

string parseReturnStatement(const string& str)
{
	int start = (int)str.find_first_of(' ');
	int end = (int)str.size() - 1;
	return trim(substring(trim(str), start, end));
}

string removeParenthesis(const string& str)
{
	int start = (int)str.find_first_of('(') + 1;
	int end = (int)str.find_last_of(')') - 1;
	return substring(str, start, end);
}

string replace(const string& str, const string& toReplace, const string& replaceWith) {
	string output;
	string temp;

	int sameLetters = 0;
	int replaceIndex = 0;
	for (int i = 0; i < (int)str.size(); i++) {
		if (str[i] == toReplace[sameLetters]) {
			temp += str[i];

			if (sameLetters == 0)
				replaceIndex = i;

			sameLetters++;

			if (toReplace.size() == sameLetters) {
				output += replaceWith;
				sameLetters = 0;
				temp = "";
			}
		}
		else {
			output += temp + str[i];
			sameLetters = 0;
			temp = "";
		}
	}

	return output;
}

string substring(const string& str, int& start, int& end)
{
	string s = "";

	for (int i = 0; i < (int)str.size(); i++) {
		if (i >= start && i <= end) {
			s += str[i];
		}
	}

	return s;
}

string ltrim(const string& str)
{
	string s = str;
	s.erase(s.find_last_not_of(" \t\n\r\f\v") + 1);
	return s;
}

string rtrim(const string& str)
{
	string s = str;
	s.erase(0, s.find_first_not_of(" \t\n\r\f\v"));
	return s;
}

string trim(const string& str)
{
	return ltrim(rtrim(str));
}

string getExePath()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	string::size_type pos = string(buffer).find_last_of("\\/");
	return string(buffer).substr(0, pos);
}

bool isNumber(const string& str)
{
	bool isDigit = true;
	for (int i = 0; i < (int)str.size(); i++) {
		if (!isdigit(str[i])) isDigit = false;
	}

	return isDigit;
}

bool isFloat(const string& str)
{
	bool isFloat = true;
	for (int i = 0; i < (int)str.size(); i++) {
		if (!isdigit(str[i]) && str[i] != '.') isFloat = false;
	}

	return isFloat;
}

bool startsWith(const string& str, const string& start)
{
	for (int i = 0; i < (int)start.size(); i++) {
		if (start[i] != str[i]) return false;
	}

	return true;
}

bool endsWith(const string& str, const string& end)
{
	if (str.length() >= end.length()) {
		return (str.compare(str.length() - end.length(), end.length(), end) == 0);
	}
	else {
		return false;
	}
}

bool contains(const string& str, const string& content)
{
	int sameLetters = 0;
	for (int i = 0; i < (int)str.size(); i++) {
		if (str[i] == content[sameLetters]) {
			sameLetters++;

			if (sameLetters == (int)content.size()) {
				return true;
			}
		}
		else {
			sameLetters = 0;
		}
	}

	return false;
}

int count(const string& str, const char& c) {
	int count = 0;

	for (int i = 0; i < (int)str.size(); i++) {
		if (str[i] == c) {
			count++;
		}
	}

	return count;
}