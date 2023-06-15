/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2022-11-03 14:57:41
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2022-11-04 19:32:59
 * @FilePath: /boring-code/src/string_match/kmp.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __KMP_H__
#define __KMP_H__

#include "container/cn.h"

/**
 * @brief KMP 字符串比较算法。返回装着匹配位置的。CN 用完记得释放。
 * 
 * @param sequence 
 * @param word 
 * @return CN 
 */

CN kmp_matcher(const char* sequence, const char* word);
#endif