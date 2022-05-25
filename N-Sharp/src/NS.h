#pragma once

#ifdef DEBUG
#define PRINT_LOGS true
#else
#define PRINT_LOGS false
#endif // DEBUG

#define BLACK			0
#define BLUE			1
#define GREEN			2
#define CYAN			3
#define RED				4
#define MAGENTA			5
#define BROWN			6
#define LIGHTGRAY		7
#define DARKGRAY		8
#define LIGHTBLUE		9
#define LIGHTGREEN		10
#define LIGHTCYAN		11
#define LIGHTRED		12
#define LIGHTMAGENTA	13
#define YELLOW			14
#define WHITE			15

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stack>
#include <fstream>
#include <regex>
#include <algorithm>
#include <limits>
#include <math.h>
#include <Windows.h>

#include <boost/any.hpp>

using namespace std;

void setColor(int color, bool fromScript = false);
void logInfo(const string& message);
void logWarning(const string& warning);
void logError(const string& error);
void logScriptError(const string& error, int lineNo);