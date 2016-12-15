#pragma once

namespace hw4
{
	class NativeWindow
	{
	public:
		virtual ~NativeWindow() {}

		virtual bool UpdateEvent() = 0;

		virtual void Present() = 0;
	};
}
