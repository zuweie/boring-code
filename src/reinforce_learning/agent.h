/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-08-25 07:50:35
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-09-13 12:01:42
 * @FilePath: /boring-code/src/reinforce_learning/agent.h
 * @Description: 本算法是基于 B 站 赵世钰 老师的公开课《强化学习的数学原理》而实现的。除了公开课，他还有一个同名的电子书《强化学习的数学原理》。
 */
#ifndef __AGENT_H__
#define __AGENT_H__
typedef struct policy policy_t;
typedef struct grid_world grid_world_t;
typedef enum move move_t;

typedef struct agent {

    grid_world_t* world;
    policy_t*     policy;

} agent_t;

typedef struct consequence {
    float reward;
    int   stay_id;
} consequence_t;

typedef struct trajectory {

    consequence_t consequence;
    int           step_id;
    move_t        step_move;
    struct trajectory* next;
} trajectory_t;


int agent_init(agent_t* agent);
int agent_reset(agent_t* agent);
int agent_load(const char* grid_path, const char* policy_path, agent_t* agent);
int agent_display_policy(agent_t* agent);
int agent_display_policy2(agent_t* agent);
int agent_display_gridworld(agent_t* agent);
consequence_t agent_move(agent_t* agent, int start_id, move_t move);
int agent_calculate_state_values(agent_t* agent, matrix2_t** state_values, matrix2_t** rewards, matrix2_t** transitions, int max_iter, float gamma);
int agent_value_iteration(agent_t* agent, matrix2_t** state_value, float gamma);
int agent_policy_itreation(agent_t* agent, matrix2_t** state_value, float gamma);
int agent_policy_iteration_bese_on_monte_carlo_basic(agent_t* agent, matrix2_t** state_value, int epsiodes, int trajectory_length, float gamma);
int agent_policy_iteration_bese_on_monte_carlo_exploring_start(agent_t* agent, matrix2_t** state_value, int episodes, int trajectory_length, float gamma);
int agent_policy_iteration_base_on_monte_carlo_epsilon_greedy(agent_t* aent, matrix2_t** state_value, int episodes, int trajectory_length, float epsilon, float gamma);


#endif