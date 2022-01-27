#pragma once
#include <windows.h>
#include <winerror.h>
#include "Memory.h"
#include "Containers.h"
#include "IO.h"
#include "Debug.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#ifdef __cplusplus
extern "C" {
#endif
	int _fltused = 0; // it should be a single underscore since the double one is the mangled name
#ifdef __cplusplus
}
#endif

#pragma warning( disable : 4100 )
