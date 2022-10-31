/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2022-10-27 15:56:53
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2022-10-31 14:50:51
 * @FilePath: /boring-code/src/dp/lcs.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __LCS_H__
#define __LCS_H__

#include "container/cn.h"

// typedef struct {
//     int x;
//     int y;
//     CN links;
// } vetex_t;


// typedef CN graph_t;

int lcs(const char* s1, const char* s2, int** lcs_table, int** path_table);
int lcs_strings(const char* s1, const char* s2, int **path, CN strings);


#endif