/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2022-12-04 09:55:15
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2022-12-04 10:04:48
 * @FilePath: /boring-code/src/leetcode/51.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __51_H__
#define __51_H__

/**
 * @brief 按照国际象棋的规则，皇后可以攻击与之处在同一行或同一列或同一斜线上的棋子。
 * n 皇后问题 研究的是如何将 n 个皇后放置在 n×n 的棋盘上，并且使皇后彼此之间不能相互攻击。
 * 给你一个整数 n ，返回所有不同的 n 皇后问题 的解决方案。
 * 每一种解法包含一个不同的 n 皇后问题 的棋子放置方案，该方案中 'Q' 和 '.' 分别代表了皇后和空位。
 * 
 * @param n 
 * @param return_size 
 * @param return_column_sizes 
 * @return char*** 
 */
char*** solve_n_queens(int n, int* return_size, int** return_column_sizes);

#endif