/*
 * @Author: your name
 * @Date: 2021-10-21 11:58:55
 * @LastEditTime: 2021-10-21 14:53:18
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/cn.c
 */
#include <stdlib.h>
#include <stdarg.h>

#include "cn.h"

static char* __CONTAINERS[CAPACITY_NUMBER] = {NULL};

CN CN_create(unsigned long build_code, ...)
{

}
CN CN_finalize(CN cn, int(*cleanup)(T*));