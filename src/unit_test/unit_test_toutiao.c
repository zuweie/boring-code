/*
 * @Author: your name
 * @Date: 2021-01-15 09:14:00
 * @LastEditTime: 2021-07-29 15:00:36
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_dct.c
 */
#include <stdio.h>
#include <stdarg.h>
#include <CUnit/Basic.h>
#include "test_data.h"

typedef struct {
    char i;
    int j;
} ty_t;

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

static void dots_params(int nflag, ...)
{
    printf("\n test dot params:\n");
    va_list valist;
    va_start(valist, nflag);
    char i = va_arg(valist, char);
    int  j = va_arg(valist, int);
    size_t  k = va_arg(valist, size_t);
    float l = va_arg(valist, double);
    double m = va_arg(valist, double);
    ty_t   o = va_arg(valist, ty_t);
    printf("i %c \n", i);
    printf("j %d \n", j);
    printf("k %d \n", k);
    printf("l %lf \n", l);
    printf("m %lf\n", m);
    printf("ty.i: %d, ty.j: %d", o.i, o.j);
    va_end(valist);

}

static void test_dots_params(void)
{   
    ty_t t = {.i = 1, .j = 2};
    dots_params('c', 'd', 1, 3, 4.2, 3.5, t);
}

int do_toutiao_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test Toutiao Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    // if (NULL == CU_add_test(pSuite, "test toutiao", test_toutiao_test) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test toutiao", test_toutiao1_test) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test toutiao2", test_toutiao2_test) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }
    // if (NULL == CU_add_test(pSuite, "test toutiao3", test_toutiao3_test) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }
    
    // if (NULL == CU_add_test(pSuite, "test toutiao4", test_toutiao4_test) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    if (NULL == CU_add_test(pSuite, "test dots_params", test_dots_params) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}