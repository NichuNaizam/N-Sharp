#include "Window.h"
#include "Input.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

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

	// Set window icon
	int icoWidth, icoHeight, channels;
	string icoPath = "";

#ifdef DEBUG
	icoPath = "./res/N#.png";
#else
	icoPath = getExePath() + "/res/N#.png";
#endif

	unsigned char* pixels = stbi_load(icoPath.c_str(), &icoWidth, &icoHeight, &channels, 4);
	if (pixels == nullptr) {
		logWarning("Failed to set window icon");
	}
	else {
		GLFWimage ico;
		ico.width = icoWidth;
		ico.height = icoHeight;
		ico.pixels = pixels;
		glfwSetWindowIcon(window, 1, &ico);
	}

	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, Input::keyCallback);
	glfwSetMouseButtonCallback(window, Input::mouseButtonCallback);
	glfwSetCursorPosCallback(window, Input::cursorPosCallback);
	glfwSetScrollCallback(window, Input::scrollCallback);
}

void Window::clear(float r, float g, float b)
{
	glClearColor(r, g, b, 1);
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
