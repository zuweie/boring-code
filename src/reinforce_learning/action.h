/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-08-22 09:51:36
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-08-26 15:01:31
 * @FilePath: /boring-code/src/reinforce_learning/action.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __ACTION_H__
#define __ACTION_H__

typedef enum action {
    e_idle  = 0,
    e_go_up = 1,
    e_go_right,
    e_go_donw,
    e_go_left,
    e_no_move

} action_t;

#endif