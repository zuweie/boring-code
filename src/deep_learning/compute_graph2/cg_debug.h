#ifndef __CG_DEBUG_H__
#define __CG_DEBUG_H__
#include <stdarg.h>


#if 1
#include <stdio.h>
#define CG_DEBUG(...) cg_print(__VA_ARGS__)
#else
#define CG_DEBUG(...)
#endif

void cg_print(const char* fmt, ...);

#endif