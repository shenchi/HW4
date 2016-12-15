#pragma once
#include "NativeWindow.h"

struct GLFWwindow;

namespace hw4
{
	class NativeWindowGL : public NativeWindow
	{
	public:
		NativeWindowGL(int width, int height, const char* title, bool fullscreen = false);
		~NativeWindowGL();

		virtual bool UpdateEvent();

		virtual void Present();

	private:
		GLFWwindow* window;
	};
}
