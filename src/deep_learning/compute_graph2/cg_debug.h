/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-25 10:18:59
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-07 13:43:00
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_debug.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_DEBUG_H__
#define __CG_DEBUG_H__
#include <stdarg.h>


#if 0
#include <stdio.h>
#define CG_DEBUG(...) cg_print(__VA_ARGS__)

static inline void cg_print(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args); 
}
#else
#define CG_DEBUG(...)
#endif


#endif