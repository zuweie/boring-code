/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2022-11-09 08:21:55
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2022-12-03 17:17:29
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
#include "leetcode/domino_tromino_tiles.h"
#include "leetcode/permutation_combination.h"
#include "leetcode/891.h"
#include "leetcode/813.h"
#include "leetcode/37.h"

static int  suite_success_init (void) 
{
    printf("\nleetcode suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nleetcode suite success clean\n");
}

static void print_b(void* pointer, size_t size) 
{
    unsigned long data = *((unsigned long*) pointer);
    int length = size*8;
    int counter = 0;
    while (length-- > 0) {
        printf("%lu", (data>>length)&0x1);
        counter++;
        if (counter % 8 == 0) printf(" ");
    }

    
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

static void test_domino_tromino_tiles (void) {
    long n = domino_tromino_tilings(6);
    printf("domino_tromino_numbers: %ld \n", n);
}

static void test_permutation_combination (void) {
    float input[9] = {1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f};
    int row, col;
    float* ret = permutation_combination(input, 9, 1, &row, &col);
    float (*mat)[col] = ret;
    printf("\n");
    for (int j=0; j<col; ++j) {
        for (int i=0; i<row; ++i) {
            printf("%d, ", (int)mat[i][j]);
        }
        printf("\n");
    }
    free(ret);
}

static void test_permutation_combination2 (void) {
    float input[10] = {1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f};
    int row, col; 
    float** ret = permutation_combination2(input, 10, 4, &row, &col);
    float (*mat)[col] = ret;
    printf("\n");
    for (int i=0; i<row; ++i) {
        for (int j=0; j<col; ++j) {
            printf("%d, ", (int)mat[i][j]);
        }
        printf("\n");
    }
    free(ret);
}

static void test_891 (void) {
    //int input[3] = {2, 1, 3};
    //int input[] ={5,69,89,92,31,16,25,45,63,40,16,56,24,40,75,82,40,12,50,62,92,44,67,38,92,22,91,24,26,21,100,42,23,56,64,43,95,76,84,79,89,4,16,94,16,77,92,9,30,13};
    int input[] = {5,69,89,92,31,16,25,45,63,40};
    long total = sum_sub_seq_widths(input, sizeof(input) / sizeof(int));
    printf("total: %ld", total);
}

static void test_891_2 (void) 
{
    int input[] ={5,69,89,92,31,16,25,45,63,40,16,56,24,40,75,82,40,12,50,62,92,44,67,38,92,22,91,24,26,21,100,42,23,56,64,43,95,76,84,79,89,4,16,94,16,77,92,9,30,13};
    long total = sum_sub_seq_widths2(input, sizeof(input) / sizeof(int));
    printf("total: %ld", total);
    
}

static void test_891_3 (void) {

    int input[] ={5,69,89,92,31,16,25,45,63,40,16,56,24,40,75,82,40,12,50,62,92,44,67,38,92,22,91,24,26,21,100,42,23,56,64,43,95,76,84,79,89,4,16,94,16,77,92,9,30,13};

    //int input[] = {5,69,89,92,31,16,25,45,63,40};
    long total = sum_sub_seq_widths3(input, sizeof(input) / sizeof(int));
    printf("total: %ld ", total);
}

static void test_813 (void) 
{
    // int inputs[] = {4663,3020,7789,1627,9668,1356,4207,1133,8765,4649,205,6455,8864,3554,3916,5925,3995,4540,3487,5444,8259,8802,6777,7306,989,4958,2921,8155,4922,2469,6923,776,9777,1796,708,786,3158,7369,8715,2136,2510,3739,6411,7996,6211,8282,4805,236,1489,7698};
    // int k = 27;

    // int inputs[] = {9,1,2,3,9};
    // int k = 3;

    int inputs[] = {1,2,3,4,5,6,7};
    int k = 4;

    double res = largestSumOfAverages(inputs, sizeof(inputs)/ sizeof(int), k);
    
    printf("res:%lf ", res);
}

static void test_37 (void) 
{
char board[9][9] = {{"5","3",".",".","7",".",".",".","."}, \
                    {"6",".",".","1","9","5",".",".","."}, \
                    {".","9","8",".",".",".",".","6","."}, \
                    {"8",".",".",".","6",".",".",".","3"}, \
                    {"4",".",".","8",".","3",".",".","1"}, \
                    {"7",".",".",".","2",".",".",".","6"}, \
                    {".","6",".",".",".",".","2","8","."}, \
                    {".",".",".","4","1","9",".",".","5"}, \
                    {".",".",".",".","8",".",".","7","9"}};


    int out = calculate_valid_numbers(board, 0, 2);
    print_b(&out, sizeof(out));
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

    // if (NULL == CU_add_test(pSuite, "test palindrome ..\n", test_palinrome_str) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test domino_tromino_tiles ..\n", test_domino_tromino_tiles) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test permutation_combination ..\n", test_permutation_combination) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test permutation_combination2 ..\n", test_permutation_combination2) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test test_891 ..\n", test_891) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test test_891_3..\n", test_891_3) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test test_891_2 ..\n", test_891_2) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test test_813 ..\n", test_813) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    if (NULL == CU_add_test(pSuite, "test test_37 ..\n", test_37) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}