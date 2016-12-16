#pragma once

#include <memory>

namespace hw4
{
	class Renderer;
	class NativeWindow;
	class Input;

	class ModuleFactory
	{
	public:
		virtual ~ModuleFactory() {}

		virtual std::shared_ptr<NativeWindow> CreateNativeWindow(int width, int height, const char* title, bool fullscreen = false) = 0;

		virtual std::shared_ptr<Renderer> CreateRenderer(std::weak_ptr<NativeWindow> window) = 0;

		virtual std::shared_ptr<Input> CreateInput(std::weak_ptr<NativeWindow> window) = 0;
	};
}
