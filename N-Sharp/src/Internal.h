#include "NS.h"
#include "Utils.h"
#include "Interpreter.h"
#include "OpenGL/OpenGL.h"

#pragma once

NSharpVariable NSFunction(string name, vector<NSharpVariable> params, Interpreter& interpreter) {
	try {
		/* System functions */
		if (name == "NS.System.PrintLine") {
			if ((int)params.size() == 0) throw exception();

			for (int i = 0; i < (int)params.size(); i++) {
				if (trim(params[i].first) == "Array") {
					NSharpClass c = anyAsClass(params[i].second);
					cout << anyAsString(c.second["arr"].second);
				}
				else {
					cout << anyAsString(params[i].second);
				}
			}

			cout << endl;
			return NSharpNULL;
		}
		else if (name == "NS.System.Print") {
			if ((int)params.size() == 0) throw exception();

			for (int i = 0; i < (int)params.size(); i++) {
				cout << anyAsString(params[i].second) << " ";
			}

			return NSharpNULL;
		}
		else if (name == "NS.System.ReadLine") {
			if ((int)params.size() > 0) throw exception();

			string input;
			getline(cin, input);
			return createVariable("string", input);
		}
		else if (name == "NS.System.SetTextColor") {
			if ((int)params.size() != 1) throw exception();

			string color = anyAsString(params[0].second);
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

			return NSharpNULL;
		}
		else if (name == "NS.System.Exit") {
			if (params.size() != 0) throw exception();

			glfwTerminate();
			exit(0);
		}
		else if (name == "NS.System.GetTime") {
			if (params.size() != 0) throw exception();

			return createVariable("float", (float)glfwGetTime());
		}

		/* Graphics functions */
		if (name == "NS.Graphics._Init_GPU_") {
			if (params.size() != 3) throw exception();

			int width = anyAsInt(params[0].second);
			int height = anyAsInt(params[1].second);
			string title = anyAsString(params[2].second);

			Window& window = Window::instance();
			window.initialize(title, width, height);

			initialize();

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			return NSharpNULL;
		}
		else if (name == "NS.Graphics._WindowShouldClose_") {
			if (params.size() != 0) throw exception();

			Window& window = Window::instance();
			return createVariable("bool", window.windowShouldClose());
		}
		else if (name == "NS.Graphics._Begin_") {
			if (params.size() != 0) throw exception();

			Window& window = Window::instance();
			window.pollEvents();

			return NSharpNULL;
		}
		else if (name == "NS.Graphics._Clear_") {
			if (params.size() != 3) throw exception();

			float r = anyAsFloat(params[0].second);
			float g = anyAsFloat(params[1].second);
			float b = anyAsFloat(params[2].second);
			
			Window& window = Window::instance();
			window.clear(r, g, b);

			return NSharpNULL;
		}
		else if (name == "NS.Graphics._LoadTexture_") {
			if (params.size() != 1) throw exception();

			string path = anyAsString(params[0].second);
			int id = generateTexture(path);
			if (id == -1) {
				logScriptError("Failed to load texture: " + path, interpreter.getLine());
			}

			return createVariable("int", id);
		}
		else if (name == "NS.Graphics._RenderTexture_") {
			if (params.size() != 4) throw exception();

			int id = anyAsInt(params[0].second);
			glm::vec2 pos = anyAsVector2(params[1].second);
			float rot = anyAsFloat(params[2].second);
			glm::vec2 scale = anyAsVector2(params[3].second);

			renderTexture(id, pos, rot, scale);
			return NSharpNULL;
		}
		else if (name == "NS.Graphics._End_") {
			if (params.size() != 0) throw exception();

			Window& window = Window::instance();
			window.finalizeFrame();
			return NSharpNULL;
		}

		/* Input functions */
		if (name == "NS.Input._IsKeyDown_") {
			if (params.size() != 1) throw exception();

			string keyName = anyAsString(params[0].second);
			return createVariable("bool", Input::isKeyDown(keyName));
		}
		else if (name == "NS.Input._IsKeyPressed_") {
			if (params.size() != 1) throw exception();

			string keyName = anyAsString(params[0].second);
			return createVariable("bool", Input::isKeyPressed(keyName));
		}
		else if (name == "NS.Input._IsMouseButtonDown_") {
			if (params.size() != 1) throw exception();

			int button = anyAsInt(params[0].second);
			return createVariable("bool", Input::isMouseButtonDown(button));
		}
		else if (name == "NS.Input._GetMousePosition_") {
			if (params.size() != 0) throw exception();

			return createVariable("vec2", Input::getMousePos());
		}
		else if (name == "NS.Input._GetMouseScroll_") {
			if (params.size() != 0) throw exception();

			return createVariable("vec2", Input::getMouseScroll());
		}

		/* Physics functions */
		if (name == "NS.Physics._DoesCollide_") {
			if (params.size() != 4) throw exception();

			glm::vec2 pos1 = anyAsVector2(params[0].second);
			glm::vec2 sca1 = anyAsVector2(params[1].second);
			glm::vec2 pos2 = anyAsVector2(params[2].second);
			glm::vec2 sca2 = anyAsVector2(params[3].second);

			return createVariable("bool", (
				pos1.x < pos2.x + sca2.x &&
				pos1.x + sca1.x > pos2.x &&
				pos1.y < pos2.y + sca2.y &&
				pos1.y + sca1.y > pos2.y
			));
		}

		/* Vector Functions */
		if (name == "NS.Vector._Vec2_") {
			if (params.size() != 2) throw exception();

			float x = anyAsFloat(params[0].second);
			float y = anyAsFloat(params[1].second);

			return createVariable("vec2", glm::vec2(x, y));
		}
		else if (name == "NS.Vector._GetX_") {
			if (params.size() != 1) throw exception();

			glm::vec2 v = anyAsVector2(params[0].second);
			return createVariable("float", v.x);
		}
		else if (name == "NS.Vector._GetY_") {
			if (params.size() != 1) throw exception();

			glm::vec2 v = anyAsVector2(params[0].second);
			return createVariable("float", v.y);
		}

		/* Array functions */
		if (name == "NS.Array._CreateArray_") {
			if (params.size() != 0) throw exception();

			vector<NSharpVariable> arr;

			return createVariable("array", arr);
		}
		else if (name == "NS.Array._Push_") {
			if (params.size() != 2) throw exception();

			vector<NSharpVariable> arr = anyAsArray(params[0].second);
			NSharpVariable value = params[1];

			arr.push_back(value);

			return createVariable("array", arr);
		}
	}
	catch (exception) {
		logScriptError("Function " + name + " does not take " + to_string(params.size()) + " arguments", interpreter.getLine());
	}

	logScriptError("Undefined N-Sharp Function: " + name, interpreter.getLine());
	return NSharpNULL;
}