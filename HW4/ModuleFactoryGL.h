#pragma once

#include "ModuleFactory.h"

namespace hw4
{
	class ModuleFactoryGL : public ModuleFactory
	{
	public:
		ModuleFactoryGL();

		~ModuleFactoryGL();

		virtual std::shared_ptr<NativeWindow> CreateNativeWindow(int width, int height, const char* title, bool fullscreen = false);

		virtual std::shared_ptr<Renderer> CreateRenderer(std::weak_ptr<NativeWindow> window);
	};
}
