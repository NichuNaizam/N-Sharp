#include "Input.h"

map<string, bool> Input::Keys;
map<string, bool> Input::KeysProcessed;
bool Input::MouseButtons[GLFW_MOUSE_BUTTON_LAST];
glm::vec2 Input::mousePos;
glm::vec2 Input::scroll;

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	string keyName = glfwGetKeyName(key, scancode);
	Keys[keyName] = action == GLFW_PRESS || action == GLFW_REPEAT;
	KeysProcessed[keyName] = action == GLFW_PRESS || action == GLFW_REPEAT;
}

void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	MouseButtons[button] = action == GLFW_PRESS || action == GLFW_REPEAT;
}

void Input::cursorPosCallback(GLFWwindow* window, double x, double y)
{
	mousePos = { (float)x, (float)y };
}

void Input::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	scroll += glm::vec2((float)xoffset, (float)yoffset);
}

bool Input::isKeyDown(string key)
{
	return Keys[key];
}

bool Input::isKeyPressed(string key)
{
	bool state = KeysProcessed[key];
	KeysProcessed[key] = false;
	return state;
}

bool Input::isMouseButtonDown(int button)
{
	return MouseButtons[button];
}

glm::vec2 Input::getMousePos()
{
	return mousePos;
}

glm::vec2 Input::getMouseScroll()
{
	return scroll;
}
