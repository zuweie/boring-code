/*
 * @Author: your name
 * @Date: 2020-10-13 11:26:56
 * @LastEditTime: 2021-04-01 18:01:19
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/test_data.h
 */
#ifndef _TEST_DATA_H_
#define _TEST_DATA_H_

#include "container/Tv.h"
#include <math.h>
#define TEST_DATA_SIZE 100 
void init_test_data_int(void);
void init_test_data_float(void);
void init_test_data(void);

Tv getTSi(int i);
Tv getTSf(int i);
Tv getTSs(int i);
int geti(int i);
float getf(int i);
char* getstr(int i);
extern const int TEST_DATA_STR_SIZE;

#endif