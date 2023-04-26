#pragma once

#ifdef _DEBUG
#define PRINT_LOGS true
#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
#else
#define PRINT_LOGS false
#endif // _DEBUG

#define VERSION "0.1.4-BETA"

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
#include <nlohmann/json.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#define otherFilesType map<string, pair<map<string, pair<vector<string>, vector<string>>>, map<string, pair<string, boost::any>>>>
#define NSharpNULL make_pair<string, boost::any>("NULL", NULL);
#define NSharpVariable pair<string, boost::any>
#define NSharpFunction pair<vector<string>, vector<string>>
#define NSharpClass pair<map<string, NSharpFunction>, map<string, NSharpVariable>>

using namespace std;
using namespace nlohmann;

NSharpVariable createVariable(string type, boost::any value);
NSharpVariable createClassObject(string className, map<string, NSharpVariable> variables, map<string, NSharpFunction> functions);

void setColor(int color, bool fromScript = false);
void logInfo(const string& message);
void logWarning(const string& warning);
void logError(const string& error);
void logScriptError(const string& error, string line);