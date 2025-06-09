/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2022-10-27 15:57:03
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-09 12:24:05
 * @FilePath: /boring-code/src/dp/lcs.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <string.h>
// #include "container/cn.h"
#include "lcs.h"

#define UP 1
#define LEFT 4
#define UPLEFT 2

static int combine_strings(int** path, int shape, int x, int y, const char* s1, char* str_lcs, int lcs_capacity, CN strings);
static char* copy_lcs_string(const char* s1, int lcs_capactity);

/**
 * @brief 用动态规划算法实现，两个字符串最大公共子序列的查找。
 * 
 * @param s1 字符串 1
 * @param s2 字符串 2
 * @param table_dp 用来动态规划的表, 返回一个 (len_s1+1) * (len_s2+1) 的二维数组
 * @param table_path 用来记录动态规定路径，使用低三位来作方向判断，
 *                   最低一位为上 ↑ 1、
 *                   倒数第二位为左上 ↖ 2、
 *                   倒数第三位为左 ← 4。
 *                   当字符不相等的情况下，左和上相等的情况下，path需要记录两条路径。
 *                   返回一个 (len_s1+1) * (len_s2+1) 的二维数组
 * @return int 
 */

int lcs(const char* s1, const char* s2, int** table_dp, int** table_path)
{
    int len_s1 = strlen(s1);
    int len_s2 = strlen(s2);

    *table_dp = malloc(sizeof(int) * (len_s1+1) * (len_s2+1));
    *table_path = malloc(sizeof(int) * (len_s1+1) * (len_s2+1));

    memset(*table_dp, 0, sizeof(int) * (len_s1+1) * (len_s2+1));
    memset(*table_path, 0, sizeof(int) * (len_s1+1) * (len_s2+1));

    int (*dp)[len_s2+1] = *table_dp;
    int (*path)[len_s2+1] = *table_path;

    for (int i=1; i<=len_s1; ++i) {
        for (int j=1; j<=len_s2; ++j) {

            if (s1[i-1] == s2[j-1]) {
                // 字符相等的情况下
                dp[i][j] = dp[i-1][j-1] + 1;
                // 记录路径往左上的情况。
                path[i][j] |= UPLEFT;
            } else {
                if (dp[i -1][j] > dp[i][j-1]) {
                    dp[i][j] = dp[i-1][j];
                    // 路径往左的情况。
                    path[i][j] |= LEFT;
                } else if (dp[i][j-1] > dp[i-1][j]) {
                    // 路径往上的情况。
                    dp[i][j] = dp[i][j-1];
                    path[i][j] |= UP;
                } else {
                    // 在左边和上边都相等的情况下 dp[i-1][j] == dp[i][j-1]， 
                    // dp[i][j] 的值取其中一个即可，但是那个 path 需要把左边和上边的路径记录下来即可获取全部最长公共子序列。

                    // 这里要判断是否有环，有环就只要其中一条路径好了。
                    // 再判断是否左边，上边为空，为空就不要白new一条string了。
                    if (dp[i][j-1] != 0){

                        dp[i][j] = dp[i][j-1];
                        path[i][j] |= UP;
                        path[i][j] |= LEFT;
                    }
                }
            }
        }
    }
    return 0;
}

/**
 * @brief 根据 path 获取全部的最长公共子序列。
 * 
 * @param s1 刚做完 lcs table 的两个最长公共子序列的
 * @param path 刚做完 lcs table 得到的path
 * @param strings 所有的最长公共子序列。 
 * @return int 
 */
CN lcs_strings(const char* s1, const char* s2, int **path)
{
    CN strings = CN_create(LIST, str_t);
    // 一个最长公共子序列，不会比 s1 长，
    int len_s1 = strlen(s1);
    int len_s2 = strlen(s2);
    
    //那最小外加八个。
    int lcs_capacity = (len_s1 < len_s2 ? len_s1 : len_s2) + 1;

    // 取八的倍数。
    lcs_capacity = (((lcs_capacity) + 8 - 1) & ~(8 - 1));

    char* str_lcs = malloc(lcs_capacity);
    memset(str_lcs, 0, lcs_capacity);
    CN_add(strings, str_lcs);
    combine_strings(path, len_s2+1, len_s1, len_s2, s1,  str_lcs, lcs_capacity, strings);
    
    return strings;
}

/**
 * @brief 递归算法，得出所有最长公共子序列
 * 
 * @param path 
 * @param shape 
 * @param x 
 * @param y 
 * @param s1 
 * @param str_lcs 
 * @param lcs_capacity 
 * @param strings 
 * @return int 
 */
static int combine_strings(int** path, int shape, int x, int y, const char* s1, char* str_lcs, int lcs_capacity, CN strings) {

    int (*ppath)[shape] = path;
    int direction = ppath[x][y];

    if (x == 0 || y == 0)
        return 0;

    if (direction & UPLEFT) {
        // 
        int len_lcs = strlen(str_lcs);
        str_lcs[len_lcs] = s1[x-1];
        
        combine_strings(path, shape, x-1, y-1, s1, str_lcs, lcs_capacity, strings);
        
    } else {
        int separate = 0;
        int fission = (direction & UP) && (direction & LEFT);
        if (fission) {
            //遇上分叉，必须裂变一个 string 出来进行新路子的获取。
            char* str_cpy = copy_lcs_string(str_lcs, lcs_capacity);
            CN_add(strings, str_cpy);
            //一条往上。
            combine_strings(path, shape, x, y-1, s1, str_lcs, lcs_capacity, strings);
            // 一条往左。
            combine_strings(path, shape, x-1, y, s1, str_cpy, lcs_capacity, strings);

        } else {
            if (direction & UP) {
                // 上
                combine_strings(path, shape, x, y-1, s1, str_lcs, lcs_capacity, strings);
            } else if (direction & LEFT) {
                // 左
                combine_strings(path, shape, x-1, y, s1, str_lcs, lcs_capacity, strings);
            }
        }
    }
    return 0;
}

static char* copy_lcs_string(const char* s1, int lcs_capactity)
{
    char* str_cpy = malloc(lcs_capactity);
    memset(str_cpy, 0, lcs_capactity);
    strcpy(str_cpy, s1);
    return str_cpy;
}
