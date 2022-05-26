#include "NS.h"
#include "Utils.h"
#include "Interpreter.h"
#include "OpenGL/OpenGL.h"

boost::any NSFunction(string name, vector<boost::any> params, Interpreter& interpreter) {
	try {
		/* System functions */
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
		else if (name == "NS.System.SetTextColor") {
			if ((int)params.size() != 1) throw exception();

			string color = anyAsString(params[0]);
			transform(color.begin(), color.end(), color.begin(), ::tolower);

			if (color == "black") setColor(BLACK, true);
			else if (color == "blue") setColor(BLUE, true);
			else if (color == "green") setColor(GREEN, true);
			else if (color == "cyan") setColor(CYAN, true);
			else if (color == "red") setColor(RED, true);
			else if (color == "magenta") setColor(MAGENTA, true);
			else if (color == "brown") setColor(BROWN, true);
			else if (color == "lightgray") setColor(LIGHTGRAY, true);
			else if (color == "darkgray") setColor(DARKGRAY, true);
			else if (color == "lightblue") setColor(LIGHTBLUE, true);
			else if (color == "lightgreen") setColor(LIGHTGREEN, true);
			else if (color == "lightcyan") setColor(LIGHTCYAN, true);
			else if (color == "lightmagenta") setColor(LIGHTMAGENTA, true);
			else if (color == "yellow") setColor(YELLOW, true);
			else if (color == "white") setColor(WHITE, true);
			else setColor(WHITE, true);

			return NULL;
		}
		else if (name == "NS.System.GetTime") {
			if (params.size() != 0) throw exception();

			return (float)glfwGetTime();
		}

		/* Graphics functions */
		if (name == "NS.Graphics.Init") {
			if (params.size() != 3) throw exception();

			int width = anyAsInt(params[0]);
			int height = anyAsInt(params[1]);
			string title = anyAsString(params[2]);

			Window& window = Window::instance();
			window.initialize(title, width, height);
			return NULL;
		}
		else if (name == "NS.Graphics.WindowShouldClose") {
			if (params.size() != 0) throw exception();

			Window& window = Window::instance();
			return window.windowShouldClose();
		}
		else if (name == "NS.Graphics.Begin") {
			if (params.size() != 0) throw exception();

			Window& window = Window::instance();
			window.pollEvents();
			window.update();
			return NULL;
		}
		else if (name == "NS.Graphics.End") {
			if (params.size() != 0) throw exception();

			Window& window = Window::instance();
			window.finalizeFrame();
			return NULL;
		}
	}
	catch (exception) {
		logScriptError("Function " + name + " does not take " + to_string(params.size()) + " arguments", interpreter.getLineNo());
		exit(0);
	}

	logScriptError("Undefined N-Sharp Function: " + name, interpreter.getLineNo());
	return NULL;
}