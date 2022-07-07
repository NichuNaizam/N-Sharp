#include "InternalClasses.h"

vector<string> getArrayClass() {
	return vector<string> {
		"var arr = NS.Array._CreateArray_()\n",
		"\n",
		"function push(value)\n",
		"{\n",
		"	arr = NS.Array._Push_(arr, value)\n",
		"}\n",
	};
}