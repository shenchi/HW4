#include "Renderer.h"

#include <cassert>

namespace hw4
{

	Renderer* Renderer::_instance = nullptr;

	Renderer::Renderer()
	{
		assert(nullptr == _instance);
		_instance = this;
	}

	Renderer::~Renderer()
	{
		_instance = nullptr;
	}

}
