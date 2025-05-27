/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-25 10:33:49
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-05-27 11:36:22
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_debug.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AEß
 */
#include "cg_debug.h"
void cg_print(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args); 
}