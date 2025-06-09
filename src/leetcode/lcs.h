/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2022-10-27 15:56:53
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-09 12:16:17
 * @FilePath: /boring-code/src/dp/lcs.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __LCS_H__
#define __LCS_H__

#include "container/Cont.h"

// typedef struct {
//     int x;
//     int y;
//     CN links;
// } vetex_t;


// typedef CN graph_t;

/**
 * @brief 最长公共子序列
 * 
 * @param s1 
 * @param s2 
 * @param lcs_table 
 * @param path_table 
 * @return int 
 */
int lcs(const char* s1, const char* s2, int** lcs_table, int** path_table);
/**
 * @brief 返回所有最长公共子序列，但此方法有重复的最长公共子序列，暂时没有办法解决。
 * 
 * @param s1 
 * @param s2 
 * @param path 
 * @return CN 
 */
CN lcs_strings(const char* s1, const char* s2, int **path);


#endif