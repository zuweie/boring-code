/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2022-11-09 08:21:55
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2022-11-12 18:42:07
 * @FilePath: /boring-code/src/unit_test/unit_test_leetcode.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "container/cn.h"
#include "unit_test.h"
#include "leetcode/kmp.h"
#include "leetcode/regexp.h"
#include "leetcode/lcs.h"
#include "leetcode/longest_palindrome.h"

static int  suite_success_init (void) 
{
    printf("\nleetcode suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nleetcode suite success clean\n");
}

static void test_kmp_computer_prefix(void) 
{

    // char* word = "ababababca";
    // //char* word = "sdfasdfwer2342";
    // int len_word = strlen(word);
    // //int pi[len_word];
    // int* pi = compute_perfix(word);
    // for (int i=0; i<len_word; ++i) {
    //     printf("%d, ", pi[i]);
    // }
    // free(pi);
}

static void test_kmp(void) 
{
    char* seq = "kk-ababababca-b-ababababca-d";
    char* word = "abca-d";
    CN index = kmp_matcher(seq, word);
    printf("match pos: ");
    for (It first=CN_first(index); !It_equal(first, CN_tail(index)); It_next(first)) {
        printf("%d, ", It_int(first));
    }
    CN_finalize(index, NULL);
}

static void test_regexp(void)
{
    char* s = "aab";
    char* p = "c*a*b";

    int match = reg_exp_match(s, p);
    printf("\nmatch: %d \n", match);
}

static void test_lcs(void)
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

static void test_palinrome_str (void) {
    const char* s = "a1bcb1";
    char* p = longest_palidrome_str(s);

    if (p) {
        printf("\nps: %s, \n", p);
        free(p);
    } else {
        printf(" ps : empty");
    }
}

int do_leetcode_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("The string matcher", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    // if (NULL == CU_add_test(pSuite, "test string matcher ..\n", test_kmp_computer_prefix) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test string matcher ..\n", test_kmp) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test regexp ..\n", test_regexp) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test lcs ..\n", test_lcs) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    if (NULL == CU_add_test(pSuite, "test palindrome ..\n", test_palinrome_str) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}