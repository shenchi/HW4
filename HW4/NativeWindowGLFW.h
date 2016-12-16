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

		virtual void SetTitle(const char* title);

		virtual bool UpdateEvent();

		virtual void Present();

		virtual int Width() const { return width; }

		virtual int Height() const { return height; }

		inline virtual void SetResizeCallback(std::function<void(int, int)> callback) { resize_callback = callback; }

	private:
		static void ResizeCallback(GLFWwindow* window, int width, int height);

	private:
		friend class InputGLFW;
		GLFWwindow* window;

		int width;
		int height;

		std::function<void(int, int)> resize_callback;
	};
}
