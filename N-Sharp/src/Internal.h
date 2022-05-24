#include "NS.h"
#include "Utils.h"
#include "Interpreter.h"

boost::any NSFunction(string name, vector<boost::any> params, Interpreter& interpreter) {
	try {
		if (name == "NS.System.PrintLine") {
			if ((int)params.size() == 0) throw exception();

			for (int i = 0; i < (int)params.size(); i++) {
				cout << anyAsString(params[i]);
			}

			cout << endl;
			return NULL;
		}
		else if (name == "NS.System.Print") {
			if ((int)params.size() == 0) throw exception();

			for (int i = 0; i < (int)params.size(); i++) {
				cout << anyAsString(params[i]) << " ";
			}

			return NULL;
		}
		else if (name == "NS.System.ReadLine") {
			if ((int)params.size() > 0) throw exception();

			string input;
			getline(cin, input);
			return input;
		}
	}
	catch (exception) {
		cout << "Function " << name << " does not take " << params.size() << " arguments" << endl;
		exit(0);
	}

	std::cout << "Undefined NS function error: " << name << endl;
	exit(0);
	return NULL;
}