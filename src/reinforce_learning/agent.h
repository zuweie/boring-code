/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-08-25 07:50:35
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-08-26 15:07:09
 * @FilePath: /boring-code/src/reinforce_learning/agent.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __AGENT_H__
#define __AGENT_H__

typedef enum action action_t;

typedef struct agent {

    grid_world_t* world;
    action_t*     policies;
    cell_t*       curr_stat;

} agent_t;


int agent_init(agent_t* agent, grid_world_t* world);
int agent_reset(agent_t* agent);
int do_action(agent_t* agent, action_t action);

#endif