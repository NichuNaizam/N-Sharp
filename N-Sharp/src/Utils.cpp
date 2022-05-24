#include "Utils.h"
#include "Interpreter.h"
#include "Eval.h"

DataType getAnyType(boost::any& any)
{
	try {
		boost::any_cast<string>(any);
		return DataType::STRING;
	}
	catch (exception) {
		try {
			boost::any_cast<int>(any);
			return DataType::INTEGER;
		}
		catch (exception) {
			try {
				boost::any_cast<float>(any);
				return DataType::FLOAT;
			}
			catch (exception) {
				try {
					boost::any_cast<bool>(any);
					return DataType::BOOLEAN;
				}
				catch (exception) {
					cout << "Invalid data type encountered!" << endl;
					exit(-1);
				}
			}
		}
	}
}

string anyAsString(boost::any& any)
{
	try {
		return boost::any_cast<string>(any);
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
					return to_string(boost::any_cast<bool>(any));
				}
				catch (exception) {
					cout << "Invalid data type encountered!" << endl;
					exit(-1);
				}
			}
		}
	}
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
					cout << "Invalid data type encountered!" << endl;
					exit(-1);
				}
			}
		}
	}
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
					cout << "Invalid data type encountered!" << endl;
					exit(-1);
				}
			}
		}
	}
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
					cout << "Invalid data type encountered!" << endl;
					exit(-1);
				}
			}
		}
	}
}

boost::any getAnyFromParameter(const string& input, Interpreter& interpreter)
{
	string param = input;

	if (count(param, '"') >= 2) {
		int start = (int)param.find_first_of('"') + 1;
		int end = (int)param.find_last_of('"') - 1;
		return substring(input, start, end);
	}
	else if (interpreter.isFunction(param)) {
		string functionName = parseFunctionCall(param);
		vector<boost::any> params = getParameters(param, interpreter);

		return interpreter.runFunction(functionName, params);
	}
	else if (interpreter.isMathExpression(param)) {
		return evaluateExpression(param, interpreter);
	}
	else if (param == "true" || param == "false") {
		bool state = false;

		if (param == "true") state = true;

		return state;
	}
	else if (param.substr(param.size() - 1) == "f") {
		float f = stof(replace(param, "f", ""));
		return f;
	}
	else if (isNumber(param)) {
		return stoi(param);
	}
	else {
		return interpreter.getVariable(trim(param));
	}
}

vector<boost::any> getParameters(const string& str, Interpreter& interpreter) {
	vector<boost::any> output;
	string s = removeParenthesis(trim(str));
	
	vector<string> arguments = splitArguments(s);
	for (const string& arg : arguments) {
		output.push_back(getAnyFromParameter(arg, interpreter));
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

	return vector<string>(output);
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

string parseUsingTag(const string& str)
{
	return trim(replace(replace(str, "using ", ""), ".", "/")) + ".ns";
}

string parseIfStatement(const string& str)
{
	return replace(replace(replace(str, " ", ""), "if(", ""), ")", "");
}

string parseFunctionCall(const string& str)
{
	return trim(split(str, '(')[0]);
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

bool isNumber(const string& str)
{
	bool isDigit = true;
	for (int i = 0; i < (int)str.size(); i++) {
		if (!isdigit(str[i])) isDigit = false;
	}

	return isDigit;
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