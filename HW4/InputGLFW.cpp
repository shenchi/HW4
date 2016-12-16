#include "InputGLFW.h"

#include <GLFW\glfw3.h>

#include "NativeWindowGLFW.h"

namespace hw4
{

	InputGLFW::InputGLFW(NativeWindowGLFW* window)
		:
		window(window->window)
	{
		glfwSetInputMode(this->window, GLFW_STICKY_KEYS, 1);
		glfwSetInputMode(this->window, GLFW_STICKY_MOUSE_BUTTONS, 1);

		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	InputGLFW::~InputGLFW()
	{
	}

	bool InputGLFW::IsButtonPressed(Button button)
	{
		if (button <= MOUSE_MIDDLE_BUTTON)
			return glfwGetMouseButton(window, static_cast<int>(button)) == GLFW_PRESS;
		if (button == KEY_ESCAPE)
			return glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
		return glfwGetKey(window, static_cast<int>(button)) == GLFW_PRESS;
	}

	float InputGLFW::GetMousePositionX()
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return static_cast<float>(x);
	}

	float InputGLFW::GetMousePositionY()
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return static_cast<float>(y);
	}

}
