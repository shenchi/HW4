#include "NativeWindowGLFW.h"

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <cassert>

namespace hw4
{

	NativeWindowGLFW::NativeWindowGLFW(int width, int height, const char* title, bool fullscreen)
		:
		window(nullptr),
		width(width),
		height(height)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		window = glfwCreateWindow(width, height, title, fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

		assert(nullptr != window);

		glfwSetWindowUserPointer(window, this);

		glfwSetWindowSizeCallback(window, ResizeCallback);

		glfwMakeContextCurrent(window);
		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	}

	NativeWindowGLFW::~NativeWindowGLFW()
	{
		glfwDestroyWindow(window);
	}

	bool NativeWindowGLFW::UpdateEvent()
	{
		glfwPollEvents();

		return !glfwWindowShouldClose(window);
	}

	void NativeWindowGLFW::Present()
	{
		glfwSwapBuffers(window);
	}

	void NativeWindowGLFW::ResizeCallback(GLFWwindow * window, int width, int height)
	{
		auto win = reinterpret_cast<hw4::NativeWindowGLFW*>(glfwGetWindowUserPointer(window));
		if (nullptr == win)
			return;

		win->width = width;
		win->height = height;

		if (win->resize_callback)
			win->resize_callback(width, height);
	}

}
