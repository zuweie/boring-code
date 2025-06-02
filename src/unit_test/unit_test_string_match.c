/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2022-10-27 16:36:03
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-02 16:27:30
 * @FilePath: /boring-code/src/unit_test/unit_test_dp.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "container/cn.h"
#include "unit_test.h"
#include "string_match/kmp.h"
#include "string_match/regexp.h"


static int  suite_success_init (void) 
{
    printf("\nstring matcher suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nstring matcher suite success clean\n");
}

static void test_string_matcher_computer_prefix_test(void) 
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

static void test_string_matcher_kmp(void) 
{
    char* seq = "kk-ababababca-b-ababababca-d";
    char* word = "abca-d";
    CN index = kmp_matcher(seq, word);
    printf("match pos: ");
    for (Iter first=CN_first(index); !It_equal(first, CN_tail(index)); first = It_next(first)) {
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

int do_string_matcher_test (void) 
{

    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("The string matcher", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test string matcher ..\n", test_string_matcher_computer_prefix_test) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // if (NULL == CU_add_test(pSuite, "test string matcher ..\n", test_string_matcher_kmp) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }


    if (NULL == CU_add_test(pSuite, "test regexp ..\n", test_regexp) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}