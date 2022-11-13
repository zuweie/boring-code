/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2022-11-07 15:39:55
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2022-11-13 22:54:13
 * @FilePath: /boring-code/src/string_match/regexp.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <string.h>
#include "regexp.h"

int reg_exp_match(const char* s, const char* p) 
{
    int len_s = strlen(s);
    int len_p = strlen(p);

    //int dp[len_s+1][len_p+1];
    int dp[len_s+1][len_p+1];
    memset(dp, 0, sizeof(dp));

    dp[0][0] = 1;

    // 这里的 i 的要重 0 开始，0为空字符，a* 与 空字符匹配是为 ture 的。所有 i 必须要把空字符算上。
    for (int i=0; i<=len_s; ++i) {
        for (int j=1; j<=len_p; ++j) {
            
            // if (p[j-1] == '*') {
            //     dp[i][j-1] = dp[i-2][j-1];
            //     if (p[j-2] == '.' || p[j-2] == s[i]) {
            //         dp[i][j-1] = dp[i][j-1] || dp[i][j-3];
            //     }
            // } else if ( p[j-1] == '.' || p[j-1] == s[i]){
            //     dp[i][j] = dp[i-1][j-1];
            // }

            if (p[j-1] == '*') {
                
                dp[i][j] = dp[i][j-2];

                if (i > 0 && (p[j-2] == '.' || p[j-2] == s[i-1])) {
                    dp[i][j] = dp[i][j] || dp[i-1][j];
                }

            } else if (i > 0 && (p[j-1] == '.' || p[j-1] == s[i-1])) {
                dp[i][j] = dp[i-1][j-1];
            }

        }
    }

    // printf("\n");
    // for (int i=0; i<len_s+1; ++i) 
    // {
    //     for (int j=0; j<len_p+1; ++j) {
    //         printf("%d, ", dp[i][j]);
    //     }
    //     printf("\n");
    // }

    return dp[len_s][len_p];
}

