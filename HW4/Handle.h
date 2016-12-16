#pragma once

#include "CommonTypes.h"
#define HANDLE_DECLARE(_name) struct _name\
{\
	uint32	id;\
	inline _name() : id(0xFFFFFFFF) {}\
	inline _name(uint32 id) : id(id) {}\
	inline operator bool() const { return id != 0xFFFFFFFF; }\
	inline operator size_t() const { return static_cast<size_t>(id); }\
};