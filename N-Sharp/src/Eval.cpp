#include "Eval.h"
#include "Utils.h"

float order(const char& op)
{
	if (op == '+' || op == '-') return 1;
	if (op == '*' || op == '/') return 2;
	if (op == '^') return 3;

	return 0;
}

float applyOp(const float& a, const float& b, const char& c)
{
	switch (c) {
	case '+':
		return a + b;
		break;

	case '-':
		return a - b;
		break;

	case '*':
		return a * b;
		break;

	case '/':
		return a / b;
		break;

	case '^':
		return pow(a, b);
		break;
	}

	return 0;
}

float evaluateExpression(const string& exp)
{
	string tokens = replace(exp, " ", "");

	stack<float> values;
	stack<char> ops;
	bool negative = false;

	for (int i = 0; i < (int)tokens.size(); i++) {
		if (tokens[i] == '(') {
			ops.push(tokens[i]);
		}
		else if (isdigit(tokens[i])) {
			float val = 0;
			bool decimal = false;
			int decimalPlaces = 0;

			while (i < (int)tokens.length() && (isdigit(tokens[i]) || tokens[i] == '.')) {
				if (tokens[i] == '.') {
					decimal = true;
					i++;
					decimalPlaces++;
					continue;
				}

				if (decimal) {
					val = (val)+(tokens[i] - '0') / (float) pow(10.0f, decimalPlaces);
					decimalPlaces++;
				}
				else {
					val = (val * 10) + (tokens[i] - '0');
				}

				i++;
			}

			values.push(val);
			i--;
		}
		else if (tokens[i] == ')') {
			while (!ops.empty() && ops.top() != '(') {
				float val2 = values.top();
				values.pop();

				float val1 = values.top();
				values.pop();

				char op = ops.top();
				ops.pop();

				values.push(applyOp(val1, val2, op));
			}

			if (!ops.empty())
				ops.pop();
		}
		else {
			if (tokens[i] == '-' && (i == 0 || tokens[i - 1] == '*' || tokens[i - 1] == '/' || tokens[i - 1] == '+' || tokens[i - 1] == '-' || tokens[i - 1] == '^' || tokens[i - 1] == '(' || tokens[i - 1] == ')')) {
				negative = true;
				continue;
			}
			else {
				while (!ops.empty() && order(ops.top()) >= order(tokens[i])) {
					float val2 = values.top();
					values.pop();

					float val1 = values.top();
					values.pop();

					char op = ops.top();
					ops.pop();

					values.push(applyOp(val1, val2, op));
				}

				ops.push(tokens[i]);
			}
		}

		if (negative) {
			values.top() *= -1;
			negative = false;
		}
	}

	while (!ops.empty()) {
		float val2 = values.top();
		values.pop();

		float val1 = values.top();
		values.pop();

		char op = ops.top();
		ops.pop();

		values.push(applyOp(val1, val2, op));
	}

	return values.top();
}

bool containsInt(const string& expression, Interpreter& interpreter)
{
	bool isInt = false;
	string tokens = replace(expression, " ", "");
	logInfo(tokens);

	string temp = "";
	for (int i = 0; i < (int)tokens.size(); i++) {
		if (tokens[i] != '+' && tokens[i] != '-' && tokens[i] != '*' && tokens[i] != '/' && tokens[i] != '^' && tokens[i] != '(' && tokens[i] != ')') {
			if (temp.empty() && !isdigit(tokens[i])) temp += tokens[i];
			else if (!temp.empty()) temp += tokens[i];
		}
		else {
			if (!temp.empty()) {
				boost::any varValue = interpreter.getVariable(temp);
				isInt = getAnyType(varValue) == DataType::intType;
				i -= (int)temp.size();
				temp = "";
			}
		}
	}

	if (!temp.empty()) {
		boost::any varValue = interpreter.getVariable(temp);
		string value = anyAsString(varValue);
		isInt = getAnyType(varValue) == DataType::intType;
		temp.clear();
	}

	return isInt;
}

bool evaluateBoolExpression(const string& expression, Interpreter& interpreter)
{
	string tokens = replace(expression, " ", "");
	bool negative = startsWith(tokens, "!");
	if (negative) tokens = replace(tokens, "!", "");

	string filledTokens = "";

	string temp = "";
	for (int i = 0; i < (int)tokens.size(); i++) {
		if (tokens[i] != '<' && tokens[i] != '>' && tokens[i] != '=') {
			temp += tokens[i];
		}
		else {
			if (temp == "true" || temp == "false") {
				filledTokens += temp;
				temp.clear();
			}

			if (!temp.empty()) {
				boost::any varData = getAnyFromParameter(temp, interpreter);
				string data = anyAsString(varData);
				filledTokens += data;
				temp.clear();
			}

			filledTokens += tokens[i];
		}
	}

	if (!temp.empty()) {
		boost::any varData = getAnyFromParameter(temp, interpreter);
		string data = anyAsString(varData);
		filledTokens += data;
	}

	tokens = filledTokens;
	bool value = false;

	if (contains(tokens, "<=")) {
		vector<string> data = splitMultipleChars(tokens, { '<', '=' });
		if (stof(data[0]) <= stof(data[2])) value = true;
		else value = false;
	}
	else if (contains(tokens, ">=")) {
		vector<string> data = splitMultipleChars(tokens, { '>', '=' });
		if (stof(data[0]) >= stof(data[2])) value = true;
		else value = false;
	}
	else if (contains(tokens, "!=")) {
		vector<string> data = splitMultipleChars(tokens, { '!', '=' });
		if (data[0] != data[2]) value = true;
		else value = false;
	}
	else if (contains(tokens, "==")) {
		vector<string> data = split(tokens, '=');
		if (data[0] == data[2]) value = true;
		else value = false;
	}
	else if (contains(tokens, ">")) {
		vector<string> data = split(tokens, '>');
		if (stof(data[0]) > stof(data[1])) value = true;
		else value = false;
	}
	else if (contains(tokens, "<")) {
		vector<string> data = split(tokens, '<');
		if (stof(data[0]) < stof(data[1])) value = true;
		else value = false;
	}
	else if (tokens == "1" || tokens == "0") {
		if (tokens == "1") value = true;
		else if (tokens == "0") value = false;
	}
	else {
		logScriptError("Invalid statement error: " + expression, interpreter.getLineNo());
	}

	return negative ? !value : value;
}
