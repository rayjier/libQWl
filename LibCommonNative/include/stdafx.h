#pragma once
#ifdef DLL_BUILD
	#define LIB_COMMON_API __declspec(dllexport)
#else
	#define LIB_COMMON_API __declspec(dllimport)
#endif
#include "Type.h"
#include "Common.h"