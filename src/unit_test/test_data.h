/*
 * @Author: your name
 * @Date: 2020-10-13 11:26:56
 * @LastEditTime: 2025-06-09 12:09:36
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/test_data.h
 */
#ifndef _TEST_DATA_H_
#define _TEST_DATA_H_

#include "container/Typ.h"
#include <math.h>
#include "vtype/vfloat_type.h"
#define TEST_DATA_SIZE 100 

void init_test_data_int(void);
void init_test_data_float(void);
void init_test_data(void);

int tsd_get_int(int i);
vfloat_t tsd_get_float(int i);
char* tsd_get_str(int i);
extern const int TEST_DATA_STR_SIZE;

#endif