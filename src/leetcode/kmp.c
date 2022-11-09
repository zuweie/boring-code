/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2022-11-03 14:57:34
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2022-11-07 10:10:41
 * @FilePath: /boring-code/src/string_match/kmp.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <string.h>
#include "kmp.h"

static int compute_perfix(const char* wor, int pi[]);

/**
 * @brief 核心思想就是找道 pattern 中前后对称的前缀与后缀，当匹配失败时，直接从相同的后缀开始匹配。省去重头再来的烦恼.
 * 
 * @param sequence 
 * @param word 
 * @return CN 
 */

CN kmp_matcher(const char* sequence, const char* word) 
{
    int n = strlen(sequence);
    int m = strlen(word);

    int pi[m];
    memset(pi, 0, sizeof(int)*m);
    compute_perfix(word, pi);
    int q = -1;
    CN index = CN_create(LIST, int_t);

    for (int i=0; i<n; ++i) {
        
        while(q > 0 && word[q+1] != sequence[i]) 
            q = pi[q];
        
        if (word[q+1] == sequence[i]) 
            q++;
        
        if (q == m-1) {
            CN_add(index, (i-m+1));
            q = pi[q];
        }

    }
    return index;
}

/**
 * @brief 真他妈的是一段精妙绝伦的代码。
 * 
 * @param word
 * @param pi 
 * @return int 
 */
int compute_perfix(const char* word, int pi[]) 
{
    int m = strlen(word);
    //int* pi = (int*) malloc(sizeof(int) * (m));
    pi[0] = 0;
    for (int q=1, k=0; q<m; ++q) {
        
        while (k>0 && word[k] != word[q]) 
            k = pi[k];

        if (word[k] == word[q]) 
            k++;

        pi[q] = k;
    }
    return 0;
}