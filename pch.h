#pragma once
#include <windows.h>
#include <winerror.h>
#include "Debug.h"

#ifdef __cplusplus
extern "C" {
#endif
	int _fltused = 0; // it should be a single underscore since the double one is the mangled name
#ifdef __cplusplus
}
#endif
