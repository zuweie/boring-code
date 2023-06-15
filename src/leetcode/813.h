/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2022-11-30 09:24:56
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2022-11-30 09:27:18
 * @FilePath: /boring-code/src/leetcode/813.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __831_H__
#define __813_H__
/**
 * @brief 给定数组 nums 和一个整数 k 。我们将给定的数组 nums 分成 最多 k 个相邻的非空子数组 。 分数 由每个子数组内的平均值的总和构成。
 * 注意我们必须使用 nums 数组中的每一个数进行分组，并且分数不一定需要是整数。
 * 返回我们所能得到的最大 分数 是多少。答案误差在 10-6 内被视为是正确的。
 * 
 * 
 * 示例 1:
 * 输入: nums = [9,1,2,3,9], 
 * k = 输出: 20.0000
 * 解释: 
 * nums 的最优分组是[9], [1, 2, 3], [9]. 得到的分数是 9 + (1 + 2 + 3) / 3 + 9 = 20. 
 * 我们也可以把 nums 分成[9, 1], [2], [3, 9]. 
 * 这样的分组得到的分数为 5 + 2 + 6 = 13, 但不是最大值.
 * 
 * @param nums 
 * @param numsSize 
 * @param k 
 * @return double 
 */
double largestSumOfAverages(int* nums, int numsSize, int k);

#endif;