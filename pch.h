#pragma once
#include <windows.h>
#include <winerror.h>
#include "Memory.h"
#include "IO.h"
#include "Debug.h"
#include "Utils.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#ifdef __cplusplus
extern "C" {
#endif
	int _fltused = 0;
#ifdef __cplusplus
}
#endif

#pragma warning( disable : 4100 )
