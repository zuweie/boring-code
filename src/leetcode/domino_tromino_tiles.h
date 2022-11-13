/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2022-11-13 21:58:00
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2022-11-13 22:52:10
 * @FilePath: /boring-code/src/leetcode/domino_tromino_tiles.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __DOMINO_TROMINO_TILES_H__
#define __DOMINO_TROMINO_TILES_H__

/**
 * @brief leetcode 第 790 题(https://leetcode.cn/problems/domino-and-tromino-tiling/description/?page=2) :
 * 有两种形状的瓷砖：一种是 2 x 1 的多米诺形，另一种是形如 "L" 的托米诺形。两种形状都可以旋转。
 * 给定整数 n ，返回可以平铺 2 x n 的面板的方法的数量。返回对 10^9 + 7 取模 的值。
 * 平铺指的是每个正方形都必须有瓷砖覆盖。两个平铺不同，当且仅当面板上有四个方向上的相邻单元中的两个，使得恰好有一个平铺有一个瓷砖占据两个正方形。
 * 
 * @param n 
 * @return long 
 */
long domino_tromino_tilings (int n);

#endif