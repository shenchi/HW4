#pragma once

namespace hw4
{
	// not complete, just for demostration

	enum Button
	{
		MOUSE_LEFT_BUTTON = 0,
		MOUSE_RIGHT_BUTTON,
		MOUSE_MIDDLE_BUTTON,

		KEY_ESCAPE = 27,
		KEY_SPACE = 32,

		KEY_0 = 48,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,

		KEY_A = 65,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_I,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_M,
		KEY_N,
		KEY_O,
		KEY_P,
		KEY_Q,
		KEY_R,
		KEY_S,
		KEY_T,
		KEY_U,
		KEY_V,
		KEY_W,
		KEY_X,
		KEY_Y,
		KEY_Z,
	};

	class Input
	{
	private:
		static Input* _instance;
	public:
		inline static Input* instance() { return _instance; }

		Input();

		virtual ~Input();

	public:

		virtual bool IsButtonPressed(Button button) = 0;

		virtual float GetMousePositionX() = 0;

		virtual float GetMousePositionY() = 0;
	};
}
