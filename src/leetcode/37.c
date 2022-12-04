/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2022-12-01 11:12:42
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2022-12-03 17:45:46
 * @FilePath: /boring-code/src/leetcode/37.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include "37.h"
/**
 * @brief 编写一个程序，通过填充空格来解决数独问题。
 * 数独的解法需 遵循如下规则：
 * 数字 1-9 在每一行只能出现一次.
 * 数字 1-9 在每一列只能出现一次。
 * 数字 1-9 在每一个以粗实线分隔的 3x3 宫内只能出现一次。（请参考示例图）
 * 数独部分空格内已填入了数字，空白格用 '.' 表示。
 * 输入：board = [["5","3",".",".","7",".",".",".","."],
 *               ["6",".",".","1","9","5",".",".","."],
 *               [".","9","8",".",".",".",".","6","."],
 *               ["8",".",".",".","6",".",".",".","3"],
 *               ["4",".",".","8",".","3",".",".","1"],
 *               ["7",".",".",".","2",".",".",".","6"],
 *               [".","6",".",".",".",".","2","8","."],
 *               [".",".",".","4","1","9",".",".","5"],
 *               [".",".",".",".","8",".",".","7","9"]]
 * 
 * 输出：[["5","3","4","6","7","8","9","1","2"],
 *       ["6","7","2","1","9","5","3","4","8"],
 *       ["1","9","8","3","4","2","5","6","7"],
 *       ["8","5","9","7","6","1","4","2","3"],
 *       ["4","2","6","8","5","3","7","9","1"],
 *       ["7","1","3","9","2","4","8","5","6"],
 *       ["9","6","1","5","3","7","2","8","4"],
 *       ["2","8","7","4","1","9","6","3","5"],
 *       ["3","4","5","2","8","6","1","7","9"]]
 * 
 * 解释：输入的数独如上图所示，唯一有效的解决方案如下所示：
 * https://leetcode.cn/problems/sudoku-solver/
 * 
 * @param board 
 * @param boardSize 
 * @param boardColSize 
 */ 
int solve_sudoku(char** board, int board_size, int* board_col_size) 
{
}

/**
 * @brief  计算当前位(x, y)置允许出现的字符。
 * 
 * @param board 
 * @param row 
 * @param col 
 * @param x 
 * @param y 
 * @param out
 * @return int 
 */
int calculate_valid_numbers(char** board, int x, int y)
{
    char (*pboard)[9] = board;
    int out = ~0;

    // 找到位置 x，y 所在网格。
    int base_x = x / 3;
    int base_y = y / 3;
    int i,j;
    // 首先去定哪一位可以输出。
    for (i = base_x*3; i<base_x*3 + 3; i++){
        for (j = base_y*3; j<base_y*3 + 3; ++j) {
            if (pboard[i][j] != '.') {
                int mask = 1 << (pboard[i][j] - '0' -1);
                out &= ~mask;
            }
        }
    }

    // // 把行中出现的去除掉
    // for (i=0; i<9; ++i) {
    //     if (pboard[x][i] != '.') {
    //         int mask = 1 << (pboard[x][i] - '0' -1 );
    //         out &= ~mask;
    //     }
    // }

    // // 吧列中出现去除掉
    // for (j=0; j<9; ++j) {
    //     if (pboard[j][y] != '.') {
    //         int mask = 1 << (pboard[j][y] - '0' - 1 );
    //         out &= ~mask;
    //     }
    // }
    return out; //& ~(~0<<9);
}

