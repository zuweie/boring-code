/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-08-25 07:50:35
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-11-03 16:40:08
 * @FilePath: /boring-code/src/reinforce_learning/agent.h
 * @Description: 本算法是基于 B 站 赵世钰 老师的公开课《强化学习的数学原理》而实现的。除了公开课，他还有一个同名的电子书《强化学习的数学原理》。
 */
#ifndef __AGENT_H__
#define __AGENT_H__
typedef struct policy policy_t;
typedef struct grid_world grid_world_t;
typedef enum move move_t;
typedef struct nn nn_t;

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
int agent_load(const char* grid_path, float (*cell_reward)(cell_clazz_t), const char* policy_path, agent_t* agent);
int agent_display_policy(agent_t* agent);
int agent_display_policy2(agent_t* agent);
int agent_display_gridworld(agent_t* agent);
consequence_t agent_move(agent_t* agent, int start_id, move_t move);
int agent_calculate_state_values(agent_t* agent, matrix2_t** state_values, matrix2_t** rewards, matrix2_t** transitions, int max_iter, float gamma);
int agent_temporal_difference_for_state_value(agent_t* agent, matrix2_t** state_values, float alpha, float gamma, int episodes);
int agent_value_iteration(agent_t* agent, matrix2_t** state_value, float gamma);
int agent_policy_itreation(agent_t* agent, matrix2_t** state_value, float gamma);
int agent_policy_iteration_bese_on_monte_carlo_basic(agent_t* agent, matrix2_t** state_value, int epsiodes, int trajectory_length, float gamma);
int agent_policy_iteration_bese_on_monte_carlo_exploring_start(agent_t* agent, matrix2_t** state_value, int episodes, int trajectory_length, float gamma);
int agent_policy_iteration_base_on_monte_carlo_epsilon_greedy(agent_t* aent, matrix2_t** state_value, int episodes, int trajectory_length, float epsilon, float gamma);
int agent_temporal_difference_of_sarsa(agent_t* agent, int start_id, int episodes, int trajectory_length, float epsilon, float gamma, float alpha);
int agent_temporal_difference_of_Q_learning_online(agent_t* agent,  int start_id, int episodes, int max_trajectory_length, float epsilon, float gamma, float alpha);
int agent_temporal_difference_of_Q_learning_offline(agent_t* agent, int start_id, int episodes, int max_trajectory_length, float epsilon, float gamma, float alpha);
int agent_value_function_approximation_of_td_state_value_with_linear_function(agent_t* agent, matrix2_t** state_values, int epsiodes, int trajctory_length, matrix2_t** W_out, int dimens, int (*S_feature)(matrix2_t*, int, int), float alpha, float gamma);
int agent_value_function_approximation_sarsa_with_linear_function(agent_t* agent, matrix2_t** W_out, int start_id, int episodes, int trajectory_length, int dimens, int (*Q_feature)(matrix2_t*, int, int, int), float alpah, float gamma, float epsilon);
int agent_value_function_approximation_of_Q_learning_off_policy_with_neural_network(\
    agent_t* agent, int start_id, int episodes, int trajectory_length, float gamma, float greedy_epsilon, \
    int feature_dimens, int (*Q_feature)(matrix2_t*, int, int, int), nn_t* target_nn, nn_t* main_nn, void (*progress)(const char* str, int, int, float)\
);
int agent_policy_gradient_advantage_actor_critic( \
    agent_t* agent, int start_id, int episodes, int trajectory_length, float gamma, float alpha_theta, float alpha_w,\
    int feature_dimens, int (*S_feature)(matrix2_t*, int, int), nn_t* pi_nn
);
#endif