/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2022-11-23 07:19:38
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2022-11-24 11:01:49
 * @FilePath: /boring-code/src/leetcode/891.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __891_H__
#define __891_H__

/**
 * @brief leetcode 891 题
 * https://leetcode.cn/problems/sum-of-subsequence-widths/
 * 一个序列的 宽度 定义为该序列中最大元素和最小元素的差值。
 * 给你一个整数数组 nums ，返回 nums 的所有非空 子序列 的 宽度之和 。由于答案可能非常大，请返回对 109 + 7 取余 后的结果。
 * 子序列 定义为从一个数组里删除一些（或者不删除）元素，但不改变剩下元素的顺序得到的数组。例如，[3,6,2,7] 就是数组 [0,3,1,6,2,2,7] 的一个子序列。
 * 第一种算法，回溯法，10个内可以计算出来。
 * @param input 
 * @param input_size 
 * @return int 
 */
int sum_sub_seq_widths(int* input, int input_size);

/**
 * @brief leetcode 891 题
 * https://leetcode.cn/problems/sum-of-subsequence-widths/
 * 一个序列的 宽度 定义为该序列中最大元素和最小元素的差值。
 * 给你一个整数数组 nums ，返回 nums 的所有非空 子序列 的 宽度之和 。由于答案可能非常大，请返回对 109 + 7 取余 后的结果。
 * 子序列 定义为从一个数组里删除一些（或者不删除）元素，但不改变剩下元素的顺序得到的数组。例如，[3,6,2,7] 就是数组 [0,3,1,6,2,2,7] 的一个子序列。
 * 第二种算法： 官方答案, 没看懂，以后在看
 * 解法: https://leetcode.cn/problems/sum-of-subsequence-widths/solutions/1977661/by-nxyh36-wcy1/
 * @param input 
 * @param input_size 
 * @return int 
 */
int sum_sub_seq_widths2(int* input, int input_size);

#endif