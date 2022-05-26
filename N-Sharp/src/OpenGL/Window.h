#include "NS.h"

#pragma once
class Window
{
public:
	~Window();

	static Window& instance() {
		static Window window;
		return window;
	}

	void initialize(const string& title, int width, int height);
	void update();
	void pollEvents();
	void finalizeFrame();

	bool windowShouldClose();

private:
	GLFWwindow* window;

	string title;
	int width, height;

	Window() = default;
};