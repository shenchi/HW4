#include "Input.h"

#include <cassert>

namespace hw4
{
	Input* Input::_instance = nullptr;

	Input::Input()
	{
		assert(_instance == nullptr);
		_instance = this;
	}

	Input::~Input()
	{
		_instance = nullptr;
	}

}
