/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2022-10-27 16:36:03
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2022-11-02 11:38:37
 * @FilePath: /boring-code/src/unit_test/unit_test_dp.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "container/cn.h"
#include "dp/lcs.h"
#include "unit_test.h"


static int  suite_success_init (void) 
{
    printf("\nlcs suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nlcs suite success clean\n");
}

static void test_lcs_test(void) 
{
    //  最长公共子序列算法，使用动态规划算法。
    //char* s1 = "1A2C3D4B56";
    //char* s2 = "B1D23A456A";
    //char* s1 = "1k2i6u37";
    //char* s2 = "k16u3i27";
    char* s1 = "135242687";
    char* s2 = "2148675";
    // lcs 1467 1487
    int len_s1 = strlen(s1);
    int len_s2 = strlen(s2);
    int* dp;
    int* path;
    lcs(s1, s2, &dp, &path);

    
    int (*pdp)[len_s2+1] = dp;
    int (*ppath)[len_s2+1] = path;

    printf("--- dp --- \n");
    printf("      ");
    for (int k=0;k<len_s1; k++) {
        printf(" %c ", (char)s1[k]);
    }
    printf("\n");


    for (int j=0;j<len_s2+1;++j) {
        if (j==0) 
            printf("   ");
        else
            printf(" %c ", s2[j-1]);
        for (int i=0; i<len_s1+1; ++i) {
            printf(" %d ", pdp[i][j]);
        }
        printf("\n");
    }
    // for (int i=0; i<len_s1+1; ++i) {
    //     //printf(" %c ", (char)s1[i-1]);
    //     for (int j=0; j<len_s2+1; ++j) {

    //         printf(" %d ", pdp[i][j]);
            

    //     }
    //     printf("\n");
    // }

    printf("\n\n--- path --- \n");
    // for (int i=0; i<len_s1+1; ++i) {
    //     for (int j=0; j<len_s2; ++j) {

    //         printf(" %d ", ppath[i][j]);

    //     }
    //     printf("\n");
    // }

    printf("      ");
    for (int k=0;k<len_s1; k++) {
        printf(" %c ", (char)s1[k]);
    }
    printf("\n");


    for (int j=0;j<len_s2+1;++j) {
        if (j==0) 
            printf("   ");
        else
            printf(" %c ", s2[j-1]);
        for (int i=0; i<len_s1+1; ++i) {
            printf(" %d ", ppath[i][j]);
        }
        printf("\n");
    }

    CN strings = lcs_strings(s1, s2, path);

    // print lcs:
    printf(" --- LCS strings ---\n");
    
    for (It first = CN_first(strings); !It_equal(first, CN_tail(strings)); It_next(first)) {
        printf(" %s,", It_str(first));
        
    }
    free(dp);
    free(path);
    CN_finalize(strings, NULL);

}

int do_dp_test (void) 
{

    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("The Tv Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test lcs ..\n", test_lcs_test) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

}