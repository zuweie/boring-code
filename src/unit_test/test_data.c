/*
 * @Author: your name
 * @Date: 2020-10-13 11:34:16
 * @LastEditTime: 2021-11-05 13:07:42
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/test_data.c
 */
#include <stdlib.h>
#include "test_data.h"

int test_data_int[TEST_DATA_SIZE];
vfloat_t test_data_float[TEST_DATA_SIZE];
char* test_data_string[] = {
        "abc",
        "efg",
        "hijk",
        "lmn",
        "joe",
        "wei",
        "cl1024",
        "tunimadeb",
        "fuck",
        "nice",
        "113224",
        "334422",
        "8989128",
        "jjjjda",
        "uuuu",
        "098765",
        "020",
        "898",
        "llllll",
        "A",
        "AC",
        "DC",
        "ZLP",
        "kKk"
        "abc",
        "efg",
        "hijk",
        "lmn",
        "joe",
        "wei",
        "cl1024",
        "tunimadeb",
        "fuck",
        "nice",
        "113224",
        "334422",
        "8989128",
        "jjjjda",
        "uuuu",
        "098765",
        "020",
        "898",
        "llllll",
        "A",
        "AC",
        "DC",
        "ZLP",
        "kKk"
        "abc",
        "efg",
        "hijk",
        "lmn",
        "joe",
        "wei",
        "cl1024",
        "tunimadeb",
        "fuck",
        "nice",
        "113224",
        "334422",
        "8989128",
        "jjjjda",
        "uuuu",
        "098765",
        "020",
        "898",
        "llllll",
        "A",
        "AC",
        "DC",
        "ZLP",
        "kKk"
        "abc",
        "efg",
        "hijk",
        "lmn",
        "joe",
        "wei",
        "cl1024",
        "tunimadeb",
        "fuck",
        "nice",
        "113224",
        "334422",
        "8989128",
        "jjjjda",
        "uuuu",
        "098765",
        "020",
        "898",
        "llllll",
        "A",
        "AC",
        "DC",
        "ZLP",
        "kKk"
        "abc",
        "efg",
        "hijk",
        "lmn",
        "joe",
        "wei",
        "cl1024",
        "tunimadeb",
        "fuck",
        "nice",
        "113224",
        "334422",
        "8989128",
        "jjjjda",
        "uuuu",
        "098765",
        "020",
        "898",
        "llllll",
        "A",
        "AC",
        "DC",
        "ZLP",
        "kKk"
};
const int TEST_DATA_STR_SIZE = sizeof(test_data_string) / sizeof(char*);

void init_test_data_int(void)
{
    for(int i=0; i<TEST_DATA_SIZE; ++i) {
        test_data_int[i] = rand();
    }
}
void init_test_data_float(void)
{
    for (int i=0; i<TEST_DATA_SIZE; ++i) {
        int r1 = rand();
        int r2 = rand();
        test_data_float[i] = r2 ? (vfloat_t) r1 / (vfloat_t) r2 : 0.f;
    }
}

void init_test_data(void) 
{
    init_test_data_int();
    init_test_data_float();
}

int tsd_get_int(int i) {
    return test_data_int[i%TEST_DATA_SIZE];
}

vfloat_t tsd_get_float(int i)
{
    return test_data_float[i%TEST_DATA_SIZE];
}

char* tsd_get_str(int i) 
{
    return test_data_string[i%TEST_DATA_SIZE];
}
