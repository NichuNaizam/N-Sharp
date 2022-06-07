#include "NS.h"
#include "Window.h"

#pragma once
class Input
{
public:
	Input(const Input&) = delete;
	Input(Input&) = delete;
	Input(Input&&) = delete;

	static bool isKeyDown(string key);
	static bool isKeyPressed(string key);
	static bool isMouseButtonDown(int button);
	static glm::vec2 getMousePos();
	static glm::vec2 getMouseScroll();

private:
	static map<string, bool> Keys;
	static map<string, bool> KeysProcessed;
	static bool MouseButtons[GLFW_MOUSE_BUTTON_LAST];
	static glm::vec2 mousePos, scroll;

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorPosCallback(GLFWwindow* window, double x, double y);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	friend Window;
};