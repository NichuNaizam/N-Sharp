#include "NS.h"
#include "Utils.h"

boost::any NSFunction(string name, vector<boost::any> params) {
	if (name == "NS.System.PrintLine") {
		for (int i = 0; i < (int)params.size(); i++) {
			cout << anyAsString(params[i]);
		}

		cout << endl;
		return NULL;
	}
	else if (name == "NS.System.Print") {
		for (int i = 0; i < (int)params.size(); i++) {
			cout << anyAsString(params[i]);
		}

		return NULL;
	} 
	else if (name == "NS.System.ReadLine") {
		string input;
		cin >> input;
		return input;
	}

	std::cout << "Undefined function error: " << name << endl;
	exit(0);
	return NULL;
}