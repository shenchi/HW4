#include "NativeWindowGL.h"

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <cassert>

namespace hw4
{

	NativeWindowGL::NativeWindowGL(int width, int height, const char* title, bool fullscreen)
		:
		window(nullptr)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(width, height, title, fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

		assert(nullptr != window);

		glfwMakeContextCurrent(window);
		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	}

	NativeWindowGL::~NativeWindowGL()
	{
		glfwDestroyWindow(window);
	}

	bool NativeWindowGL::UpdateEvent()
	{
		glfwPollEvents();

		return !glfwWindowShouldClose(window);
	}

	void NativeWindowGL::Present()
	{
		glfwSwapBuffers(window);
	}

}
