

#pragma once


typedef float float32_t;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifdef __linux__

#include "linux_compat.h"

#define PACK(__decl__)  __decl__ __attribute__((__packed__))

#endif



#ifdef _WIN32

#pragma warning(disable: 4996)
#pragma warning(disable: 4267)

#define PACK(__decl__) __pragma(pack (push,1)) __decl__ __pragma(pack(pop))
#endif
