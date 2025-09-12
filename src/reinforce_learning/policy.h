/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-08-22 09:51:36
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-09-11 23:43:04
 * @FilePath: /boring-code/src/reinforce_learning/action.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __ACTION_H__
#define __ACTION_H__

#define MOVE_TYPE_NUM 6

typedef enum move {
    
    e_idle  = 0,
    e_go_up = 1,
    e_go_right = 2,
    e_go_down = 3,
    e_go_left = 4,
    e_stay = 5

} move_t;

typedef struct action {

    float probability;
    move_t move;
    struct action* next;

} action_t;

typedef struct policy {
    int rows;
    int cols;
    action_t** actions;
} policy_t;

int policy_load(const char* path, policy_t* policy);
int policy_reset(policy_t* policy);
int policy_display(policy_t* policy, int disp_all);
action_t* action_get_max(action_t*);
int action_display(action_t* act);

#endif