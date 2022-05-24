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

boost::any getAnyFromParameter(const string& param, Interpreter& interpreter)
{
	if (param.empty()) return string();

	if (count(param, '"') == 2) {
		return replace(param, "\"", "");
	}
	else if (interpreter.isFunction(param)) {
		string functionName = trim(split(param, '(')[0]);
		vector<boost::any> params = getParameters(param, interpreter);

		return interpreter.runFunction(functionName, params);
	}
	else if (interpreter.isMathExpression(param)) {
		return evaluateExpression(param, interpreter);
	}
	else if (isNumber(param)) {
		return stoi(param);
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
	else {
		return interpreter.getVariable(trim(param));
	}
}

vector<boost::any> getParameters(const string& str, Interpreter& interpreter) {
	vector<boost::any> output;
	string s = trim(replace(split(str, '(')[1], ")", ""));
	vector<string> arguments = split(s, ',');

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

int count(const string& str, const char& c) {
	int count = 0;

	for (int i = 0; i < (int)str.size(); i++) {
		if (str[i] == c) {
			count++;
		}
	}

	return count;
}