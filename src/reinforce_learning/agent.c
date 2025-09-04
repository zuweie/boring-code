#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <float.h>
#include <limits.h>

#include "matrix2/matrix2.h"
#include "grid_world.h"
#include "policy.h"
#include "agent.h"

/**
 * @brief 计算立即奖励，和状态转移矩阵。
 * 
 * @param agent 
 * @param reward_vet 
 * @param trans_matrix 
 * @return int 
 */
static int __calculate_reward_trans_probibality(agent_t * agent, matrix2_t** reward_vet, matrix2_t** trans_matrix) 
{
    // 需要从 policy 的 action 
    // todo: 统计有多少个的 state。
    int state_number = agent->world->rows * agent->world->cols;
    if (*reward_vet == NULL)
        *reward_vet      = Mat2_create(state_number, 1);
    if (*trans_matrix == NULL)
        *trans_matrix    = Mat2_create(state_number, state_number);

    // j
    Mat2_fill(*reward_vet, 0.);
    Mat2_fill(*trans_matrix, 0.);
    
    for (int i=0; i<state_number; ++i) {
        float reward = 0.;
        action_t* first_action = agent->policy->actions[i];
        while (first_action) {
            // todo 1: 计算每一个 state 在 take action 
            consequence_t cons = agent_move(agent, i, first_action->move);
            reward += first_action->probability * cons.reward;
    
            // todo 2: 填写从 si 到 sj 的转移概率。
            Mat2_put(*trans_matrix, i, cons.stay_id, first_action->probability);
            first_action = first_action->next;
        }
        Mat2_put(*reward_vet, i, 0, reward);
    }

    return 0;
}

/**
 * @brief 根据赵世钰老师的《强化学习的数学原理》中的 state values 迭代算法求解法。具体是《第二章贝尔曼公式的向量形式与求解》中的算法。
 * 
 * @param agent 
 * @param state_values 
 * @param rewards 
 * @param transitions 
 * @param gamma 
 * @return int 
 */
int agent_calculate_state_values(agent_t* agent, matrix2_t** state_values, matrix2_t** rewards, matrix2_t** transitions, int max_iter, float gamma) 
{
    int state_number = agent->world->rows * agent->world->cols;

    if (*state_values == NULL)
        *state_values = Mat2_create(state_number, 1);

    Mat2_fill(*state_values, 0.f);

    matrix2_t* calculating  = Mat2_create(1,1);
    __calculate_reward_trans_probibality(agent, rewards, transitions);

    int is_close = 0;
    int iter     = 0;
    do {

        //printf("%d times state value calculate\n", iter);

        // cpy the data from trans_mat;
        Mat2_cpy(calculating, *transitions);

        // 1 gamma * trans_mat
        Mat2_scalar_multiply(calculating, gamma);
        
        // 2 trans_mat dot V(s)
        Mat2_dot(calculating, *state_values);

        // 3 immediately reward + trans_mat dot V(s)
        Mat2_add(calculating, *rewards);
        
        // 4 compare two vetor, if close, exit itertion, else continue.
        is_close = Mat2_is_close(calculating, *state_values, 1e-3);

        // 5 is no close do it one more time.
        if (!is_close) Mat2_cpy(*state_values, calculating);

    } while (!is_close && iter++ < max_iter);

    // 
    Mat2_destroy(calculating);
    
    return 0;
}


/**
 * @brief 赵世钰老师的《强化学习的数学原理》第四章的 value iteration 算法
 * 
 * @param agent 
 * @param gamma 
 * @return int 
 */
int agent_value_iteration(agent_t* agent, matrix2_t** state_values, float gamma)
{
    // value iteration 算法：
    // 当前的策略是未知，此算法的功能就是要找出最佳的 policy，所以当前的 agent 的 policy 指针为空。算法步骤如下：
    // 1 生成一些随机的 state value 例如：全部为0。
    // 2 根据当前的 state value 找到每个 state 最佳的 move。
    

    // todo 0: 给 agent 的 policy 申请空间。
    agent->policy->rows = agent->world->rows;
    agent->policy->cols = agent->world->cols;
    agent->policy->actions = (action_t**) malloc (agent->world->rows * agent->world->cols * sizeof(action_t*));

    int state_number = agent->world->rows * agent->world->cols;
    
    matrix2_t* Vs_k = Mat2_create(state_number, 1);
    matrix2_t* Vs_k1 = Mat2_create(state_number, 1);
    // 先给 state value 一个 0 的初始值。
    Mat2_fill(Vs_k, 0.f);
    
    float Max_Qas;
    float Qas;
    move_t Max_move;
    consequence_t consequence;
    int is_close   = 0;
    int iter = 1;
    // 迭代所有的 Vs 元素，
    do {
        //printf(" 第 %d 次迭代\n", iter++);

        for (int i = 0; i < state_number; ++i){

            Max_Qas  = -FLT_MAX;
            Max_move = e_idle;

            // 遍历五种的 action 得到 action value 最大哪一个。
            for (int move = e_go_up; move < MOVE_TYPE_NUM; ++move) {
                // 第 0 种的 action 是 idel，没有任何作用，所有从 第一种开始。
                consequence = agent_move(agent, i, move);
                Qas = consequence.reward + gamma * Mat2_get(Vs_k, consequence.stay_id, 0);

                if (Qas > Max_Qas)
                {
                    Max_Qas = Qas;
                    Max_move = move;
                }
            }

            // 更新 Vs_k1
            Mat2_put(Vs_k1, i, 0, Max_Qas);
            // 更新 actions 的动作。
            action_t **act = &agent->policy->actions[i];

            if (*act == NULL){
                // 如果是空的，那么就 new 一个 action 在里面。
                *act = (action_t *)malloc(sizeof(action_t));
                (*act)->next = NULL;
            }

            (*act)->move = Max_move;
            (*act)->probability = 1.0f; // greedy 的 option，只要 action value 最大的
        }
        // 比较两个 Vs Vs_k1 , check if they are close.

        is_close = Mat2_is_close(Vs_k, Vs_k1, 1e-3);

        // agent_display_policy(agent);
        // printf("\n");

        if (!is_close) Mat2_cpy(Vs_k, Vs_k1);
        
    } while (!is_close);

    // 把 Vs_k 弄出去。
    *state_values = Vs_k;
    // 把 Vs_k1 销毁。
    Mat2_destroy(Vs_k1);
    return 0;
}

/**
 * @brief 赵世钰老师《强化学习的数学原理》第四章的 policy iteration 算法。
 * 
 * @param agent 
 * @param gamma 
 * @param 
 * @return int 
 */
int agent_policy_itreation(agent_t* agent, matrix2_t** state_value, int truncated, float gamma)
{
    // 策略迭代算法。
    // 此算法地一步就是先初始化所有的策略，给每个状态一个随机策略。
    // 然后算出那个 state value，然后回过头来去算那个 action value。
    // 然后再去更新那个策略，然后重复这一个过程。

    int iter = 0;

    matrix2_t* Vs_k  = NULL;
    matrix2_t* Vs_k1 = NULL;

    matrix2_t* rewards = NULL;
    matrix2_t* transition = NULL;

    int is_close = 0;
    int i, j;

    float Max_Qas;
    float Qas;

    move_t Max_move;
    consequence_t consequence;

    agent->policy->rows = agent->world->rows;
    agent->policy->cols = agent->world->cols;
    agent->policy->actions = (action_t**) malloc (agent->world->rows * agent->world->cols * sizeof(action_t*));
    
    int state_number = agent->world->rows * agent->world->cols;
    
    // 初始化为所有的 action 为 5，原地不动。
    for (i=0; i<state_number; ++i) {
        action_t** act = &agent->policy->actions[i];
        (*act) = (action_t*) malloc (sizeof(action_t));
        (*act)->next = NULL;
        (*act)->move = e_idle;
        (*act)->probability = 1.0f;
    }

    do {

        printf("%d times policy iteration ... \n", iter++);

        // 在当前假设的 action 下，计算每个 state 的 value。
        // policy evaluation
        agent_calculate_state_values(agent, &Vs_k, &rewards, &transition, INT_MAX, gamma);

        // 计算最大化的那个 policy， update policy
        // policy improve
        for(i=0; i<state_number;++i) {

            Max_Qas  = -FLT_MAX;
            Max_move = e_idle;
            
            for (move_t move=e_go_up; move<MOVE_TYPE_NUM; ++move) {

                consequence = agent_move(agent, i, move);
                Qas         = consequence.reward + gamma * Mat2_get(Vs_k, consequence.stay_id, 0);

                if (Qas > Max_Qas) {
                    // 记录下最大的那一个 move。
                    Max_Qas  = Qas;
                    Max_move = move;
                }

            }

            // 这里更新了 agent 的 policy
            action_t** act      = &agent->policy->actions[i];
            (*act)->move        = Max_move;
            (*act)->probability = 1.0f;            
        }

        if (Vs_k1) {
            is_close = Mat2_is_close(Vs_k, Vs_k1, 1e-3);
            if (!is_close) {
                // 若尚未达到精度要求，继续下一轮。
                Mat2_cpy(Vs_k1, Vs_k);
            }
        } else {
            Vs_k1 = Mat2_create_cpy(Vs_k);
        }

    } while (!is_close);

    // 把 state 返回去。
    *state_value = Vs_k;

    Mat2_destroy(Vs_k1);
    Mat2_destroy(rewards);
    Mat2_destroy(transition);

    return 0;
}


int agent_init(agent_t* agent)
{
    agent->world  = (grid_world_t*) malloc (sizeof(grid_world_t));
    agent->policy = (policy_t*) malloc (sizeof(policy_t));
    return 0;
}

int agent_reset(agent_t* agent)
{
    grid_world_reset(agent->world);
    policy_reset(agent->policy);

    free(agent->world);
    free(agent->policy);
    return 0;
}

int agent_load(const char* grid_path, const char* policy_path, agent_t* agent)
{
    int ret = grid_world_load(grid_path, agent->world);

    // 如果 policy 的路径 null，那么我门
    if (policy_path && ret == 0) {

        agent->policy->rows = agent->world->rows;
        agent->policy->cols = agent->world->cols;

        ret = policy_load(policy_path, agent->policy);
    }
    return ret;
}


int agent_display_policy(agent_t* agent)
{
    return policy_display(agent->policy);
}

int agent_display_gridworld(agent_t* agent)
{
    return grid_world_display(agent->world);
}

consequence_t agent_move(agent_t* agent, int start_id, move_t move) 
{
    int x = start_id / agent->world->cols;
    int y = start_id % agent->world->cols;

    switch (move)
    {
    case e_go_up:
        x = x - 1;
        break;
    case e_go_right:
        y = y + 1;
        break;
    case e_go_down:
        x = x + 1;
        break;
    case e_go_left:
        y = y - 1;
        break;;
    default:
        break;
    }
    
    if (x >=0 && x < agent->world->rows && y >=0 && y < agent->world->cols) {
        cell_t cell = agent->world->cells[x * agent->world->cols + y];
        return (consequence_t){
            .reward = cell_reward(cell.cell_type),
            .stay_id = cell.id,
        };
    } else {
        return (consequence_t) {
            .reward = cell_reward(e_null),
            .stay_id = start_id,
        };
    }
}


