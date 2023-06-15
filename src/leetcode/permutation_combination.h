/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2022-11-15 20:53:36
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2022-11-21 13:17:55
 * @FilePath: /boring-code/src/leetcode/permutation_combination.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __PERMUTATION_COMBINATION_H__
#define __PERMUTATION_COMBINATION_H__

/**
 * @brief 排列组合，返回 n 中取 m 的排列组合。此算法为自研算法，不需任何其他数据结构，返回 m 行 * C(n,m) 列的数组。
 * 例如 [1,2,3,4,5] 取 3 有：
 * 
 * 1,1,1,1,1,1,2,2,2,3
 * 2,2,2,3,3,4,3,3,4,4
 * 3,4,5,4,5,5,4,5,5,5   
 * 
 * @param input 
 * @param n 
 * @param m 
 * @param ret_row 
 * @param ret_col 
 * @return float** 
 */
float** permutation_combination(float* input, int n, int m, int* ret_row, int* ret_col);

/**
 * @brief 排列组合，返回 n 取 m 的排列组合。使用回溯算法。返回 C(n,m) * m 的二维数组。
 * 例如 [1,2,3,4,5] 取 3 有：
 * 
 * 1,2,3
 * 1,2,4
 * 1,2,5
 * 1,3,4
 * 1,3,5
 * 1,4,5
 * 2,3,4
 * 2,3,5
 * 2,4,5
 * 3,4,5
 * 
 * @param input 
 * @param n 
 * @param m 
 * @param ret_row 
 * @param ret_col 
 * @return float** 
 */
float** permutation_combination2(float* input, int n, int m, int* ret_row, int* ret_col);

#endif