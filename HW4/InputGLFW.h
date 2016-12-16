#pragma once

#include "Input.h"

struct GLFWwindow;

namespace hw4
{
	class NativeWindowGLFW;

	class InputGLFW : public Input
	{
	public:
		InputGLFW(NativeWindowGLFW* window);

		~InputGLFW();

	public:

		virtual bool IsButtonPressed(Button button);

		virtual float GetMousePositionX();

		virtual float GetMousePositionY();

	private:
		GLFWwindow* window;
	};
}
