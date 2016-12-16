#pragma once

#include <functional>

namespace hw4
{
	class NativeWindow
	{
	public:
		virtual ~NativeWindow() {}

		virtual void SetTitle(const char* title) = 0;

		virtual bool UpdateEvent() = 0;

		virtual void Present() = 0;

		virtual int Width() const = 0;
		
		virtual int Height() const = 0;

		virtual void SetResizeCallback(std::function<void(int, int)> callback) = 0;
	};
}
