/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2022-11-10 23:04:18
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2022-11-12 18:41:55
 * @FilePath: /boring-code/src/leetcode/longest_palindrome.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <string.h>
#include "longest_palindrome.h"

/**
 * @brief 获取最长回文，使用动态规划做法。
 * 
 * @param s 
 * @return char* 
 */
char* longest_palidrome_str(const char* s) 
{
    int n = strlen(s);
    int dp[n][n];

    memset(dp, 0, sizeof(dp));
    int max = 0;
    int max_i;
    for (int c=0; c<=n+1; c++) {
        for (int i=0; i<n-c; i++) {

            int j = i + c;
            
            if (s[i] == s[j]) {
                if (c <=1 ) {
                    dp[i][j] = 1;
                } else {
                    dp[i][j] = dp[i+1][j-1];
                }

                if (dp[i][j]) {
                    max = c + 1;
                    max_i = i;
                }
            }

        }
    }
    if (max > 1) {
        char* palidrome_str = malloc(max+1);
        memcpy(palidrome_str, &s[max_i], max);
        palidrome_str[max] = '\0';
        return palidrome_str;
    }
    return NULL;
}