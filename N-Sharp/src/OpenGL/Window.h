#include "NS.h"
#include "Utils.h"

#pragma once
class Window
{
public:
	string title;
	int width, height;
	
	~Window();

	static Window& instance() {
		static Window window;
		return window;
	}

	void initialize(const string& title, int width, int height);
	void clear(float r, float g, float b);
	void pollEvents();
	void finalizeFrame();

	bool windowShouldClose();

private:
	GLFWwindow* window;

	Window() = default;
};