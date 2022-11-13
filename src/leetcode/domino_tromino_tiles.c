/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2022-11-13 21:57:44
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2022-11-13 22:45:34
 * @FilePath: /boring-code/src/leetcode/domino_tromino_tiles.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <string.h>
#include "domino_tromino_tiles.h"

long fn(int n, long fn_values[]) {

    if (n == 1) return 1;
    if (n == 2) return 2;

    if (fn_values[n] != 0)
        return fn_values[n];

    long total = 2;

    for (int i=1; i<=n-1; ++i) {
        total += i >= 3 ? fn(n-i, fn_values) << 1 : fn(n-i, fn_values);
    }
    fn_values[n] = total;
    return total;
}
long domino_tromino_tilings (int n) 
{
    long fn_values[n+1];
    memset(fn_values, 0, sizeof(fn_values));
    long total = fn(n, fn_values);
    return total;
}