#include "Window.h"

Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::initialize(const string& title, int width, int height)
{
	this->title = title;
	this->width = width;
	this->height = height;

	glfwInit();
	glfwDefaultWindowHints();
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (window == nullptr) {
		glfwTerminate();
		logError("Failed to init graphics");
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		logError("Failed to init graphics");
	}
}

void Window::update()
{
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::pollEvents()
{
	glfwPollEvents();
}

void Window::finalizeFrame()
{
	glfwSwapBuffers(window);
}

bool Window::windowShouldClose()
{
	return glfwWindowShouldClose(window);
}
