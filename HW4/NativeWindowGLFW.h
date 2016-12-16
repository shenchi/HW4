#pragma once
#include "NativeWindow.h"

struct GLFWwindow;

namespace hw4
{
	class NativeWindowGLFW : public NativeWindow
	{
	public:
		NativeWindowGLFW(int width, int height, const char* title, bool fullscreen = false);
		~NativeWindowGLFW();

		virtual bool UpdateEvent();

		virtual void Present();

	private:
		friend class InputGLFW;
		GLFWwindow* window;
	};
}
