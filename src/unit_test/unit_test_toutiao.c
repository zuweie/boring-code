/*
 * @Author: your name
 * @Date: 2021-01-15 09:14:00
 * @LastEditTime: 2021-04-09 15:14:58
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_dct.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "test_data.h"

#define PRINTF_DOUBLES(x) printf("%lf ", (x));

static int  suite_success_init (void) 
{
    printf("\nToutiao suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nToutiao suite success clean\n");
}
static void test_toutiao4_test(void) 
{
    printf(" \n ");
    char *a;
    a = 100;
    printf("a 的值: %d \n", a);

    char **p = &a;
    *p = 200;
    printf(" a 的值: %d \n", a);

    char *c = "ccc";
    printf(" c 指向的字符串: %s \n", c);

    p = &c;
    *p = "bbb";
    printf(" c 所指向的字符串: %s \n", c);
}

static void test_toutiao3_test(void)
{
    printf("\n");
    int arr[5][4][3];
    int (*parr)[4][3];
    parr = arr;
    printf(" parr 地址：%p \n", parr);
    parr++;
    printf(" parr++ 地址：%p, 偏移量 %ld \n", parr, (long)parr-(long)arr);
}
static void test_toutiao2_test(void) 
{
    printf(" \n");
    int *p;
    p = 0;
    printf("\n p ++ 前的地址值: %p\n", p);
    p++;
    printf("\n p ++ 后的地址值: %p\n", p);

}

static void test_toutiao1_test(void) 
{
    char* p1;
    int* p2;
    float* p3;
    double* p4;
    void* p5;

    printf("\n");
    printf(" p1 指针的size: %d \n", sizeof(p1));
    printf(" p2 指针的size: %d \n", sizeof(p2));
    printf(" p3 指针的size: %d \n", sizeof(p3));
    printf(" p4 指针的size: %d \n", sizeof(p4));
    printf(" p5 指针的size: %d \n", sizeof(p5));
}

static void test_toutiao_test(void) 
{
    // 此处 a 的内存分布为：00000001 00000001
    int   a = 257;
    int  *b = &a;
    char *c = &a;

    printf("\n");
    printf(" b 指针的值: %p \n", b);
    printf(" c 指针的值: %p \n", c);
    printf(" *b 的值: %d \n", *b);
    printf(" *c 的值: %d \n", *c);
}

int do_toutiao_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test Toutiao Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test toutiao", test_toutiao_test) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test toutiao", test_toutiao1_test) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test toutiao2", test_toutiao2_test) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (NULL == CU_add_test(pSuite, "test toutiao3", test_toutiao3_test) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL == CU_add_test(pSuite, "test toutiao4", test_toutiao4_test) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}