#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <math.h>

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
 * @brief greedy exploring 与 epsilon greedy exploring 统一函数，去除递归调用，避免因为 trajectory 长度过长而产生程序崩溃问题。
 * 
 * @param agent 
 * @param start_id 
 * @param steps 
 * @param gamma 
 * @return int 
 */
static int __exploring(agent_t* agent, int start_id, int steps, float gamma, float (*returns)[MOVE_TYPE_NUM], int (*sa_count)[MOVE_TYPE_NUM])
{
    
    trajectory_t* first = NULL;
    trajectory_t* curr  = NULL;

    int    step_id   = start_id;
    move_t step_move = policy_take_action(agent->policy->actions[step_id]);

    for (int i=0; i<steps; ++i) {
        
        trajectory_t* trajectory = (trajectory_t*) malloc (sizeof(trajectory_t));
        
        trajectory->consequence = agent_move(agent, step_id, step_move);
        trajectory->step_id     = step_id;
        trajectory->step_move   = step_move;
        trajectory->next        = first;

        first = trajectory;

        step_id   = trajectory->consequence.stay_id;
        step_move = policy_take_action(agent->policy->actions[step_id]);
    }
    float g = 0;

    // 从后往前计算。
    while(first) {

        curr = first;
        g = g * gamma + curr->consequence.reward;
        returns[curr->step_id][curr->step_move]  += g;
        sa_count[curr->step_id][curr->step_move] += 1; 
        // 即用即弃
        free(curr);
        first = first->next;
    }
    return 0;
}

static int __explor_sampling(agent_t* agent, int start_id, move_t move,  int steps, float* reward, float gamma, float (*returns)[MOVE_TYPE_NUM], int (*sa_count)[MOVE_TYPE_NUM])
{

    
    
    // 往前走一步，
    consequence_t consequence = agent_move(agent, start_id, move);
    steps--;

    if (steps != 0) {
        // 若是没有走完，计算下一步要走的方向。
        int       next_id   = consequence.stay_id;
        move_t    next_move = agent->policy->actions[next_id]->move;
        __explor_sampling(agent, next_id, next_move, steps, reward, gamma, returns, sa_count);
    }

    
    (*reward) = consequence.reward + (*reward) * gamma;
    
    // if (disp_debug) {
    //     printf("<-(id:%d, move: %d, g: %0.2f)", start_id, move, (*reward));
    // }

    returns[start_id][move]  += (*reward);
    sa_count[start_id][move] += 1;

    return 0;
}



static int __explor_epsilon_sampling(agent_t* agent, int start_id, int steps, float* g, float gamma, float (*returns)[MOVE_TYPE_NUM], int (*sa_count)[MOVE_TYPE_NUM]) 
{
    move_t move               = policy_take_action(agent->policy->actions[start_id]);
    consequence_t consequence = agent_move(agent, start_id, move);
    steps--;

    if (steps != 0) {
        int next_id = consequence.stay_id;
        __explor_epsilon_sampling(agent, next_id, steps, g, gamma, returns, sa_count);
    }

    (*g) = consequence.reward + (*g) * gamma;

    returns[start_id][move]  += (*g);
    sa_count[start_id][move] += 1;
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
 * @brief 赵世钰老师的 《强化学习的数学原理》第七章的 TD 算法，主要是这玩意使用 sochastic approximate 中 RM 算法去逼近真实的
 * state value 并不需要任何模型，只需要数据即可。
 * 
 * @param agent 
 * @param state_values 
 * @return int 
 */
int agent_temporal_difference_for_state_value(agent_t* agent, matrix2_t** state_values, float alpha, float gamma, int episodes)
{
    float vt_st;
    float vt_st1;
    float vt1_st;

    float r_1;
    
    int   iter          = 0;
    int   world_rows    = agent->world->rows;
    int   world_cols    = agent->world->cols;
    int   state_number  = world_rows * world_cols;


    consequence_t consequence;
    move_t        move;
    matrix2_t*    Vs = Mat2_create(world_rows, world_cols);

    Mat2_fill(Vs, 0.f);
    // policy_take_action 需要预先提前设置 rand seed。
    srand(time(NULL));

    do {

        for (int i=0; i<state_number; ++i) {
            
            // 获取出在当前的 state 下的 action 中 move 的概率获取下一个move。
            move = policy_take_action(agent->policy->actions[i]);

            // 然后动一下看看结果如何。
            consequence = agent_move(agent, i, move);
            
            // 本轮的 state
            vt_st  = Mat2_get(Vs, i/world_cols, i%world_cols);
            // 本轮的 state take action 后，跳转的 state 的 value
            vt_st1 = Mat2_get(Vs, consequence.stay_id/world_cols, consequence.stay_id%world_cols);

            // 逼近下一轮的 state value
            vt1_st = vt_st - alpha * ( vt_st - (consequence.reward + gamma * vt_st1));
            
            // 计算完的 state value 跟新原来的 state value
            Mat2_put(Vs, i/world_cols, i%world_cols, vt1_st);
        }

    } while (iter++ < episodes);

    (*state_values) = Vs;

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
    // agent->policy->rows = agent->world->rows;
    // agent->policy->cols = agent->world->cols;
    // agent->policy->actions = (action_t**) malloc (agent->world->rows * agent->world->cols * sizeof(action_t*));

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
int agent_policy_itreation(agent_t* agent, matrix2_t** state_value, float gamma)
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

    // agent->policy->rows = agent->world->rows;
    // agent->policy->cols = agent->world->cols;
    // agent->policy->actions = (action_t**) malloc (agent->world->rows * agent->world->cols * sizeof(action_t*));
    
    int state_number = agent->world->rows * agent->world->cols;
    
    // 初始化为所有的 action 为 5，原地不动。
    for (i=0; i<state_number; ++i) {
        action_t** act = &agent->policy->actions[i];
        (*act) = (action_t*) malloc (sizeof(action_t));
        (*act)->next = NULL;
        (*act)->move = e_stay;
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

/**
 * @brief 基于蒙地卡罗采样 basic 的 policy iteration 算法，
 * 
 * @param agent 
 * @param state_value 
 * @param episode_length
 * @param sampling_time
 * @param gamma 
 * @return int 
 */
int agent_policy_iteration_bese_on_monte_carlo_basic(agent_t* agent, matrix2_t** state_value, int epsiodes, int trajectory_length, float gamma)
{
    int i, j, k, l;
    int is_close;
    int start_id;
    int iter = 0;
    float Qas;
    float Max_Qas;
    float gamma_exp = 0.f;

    move_t Max_move;
    consequence_t consequence;
    // agent->policy->rows = agent->world->rows;
    // agent->policy->cols = agent->world->cols;
    // agent->policy->actions = (action_t**) malloc (agent->world->rows * agent->world->cols * sizeof(action_t*));

    int state_number  = agent->world->rows * agent->world->cols;
    matrix2_t* Vs_k   = Mat2_create(state_number, 1);
    matrix2_t* Vs_k1  = NULL;

    Mat2_fill(Vs_k, 0.f);
    // 为每个 state 的 action 随机分配一个动作。
    srand(time(NULL));
    for (i=0; i<state_number; ++i) {
        action_t** a = &agent->policy->actions[i];
        // 随机生成五个动作中的一个。
        (*a)              = (action_t*) malloc (sizeof(action_t));
        (*a)->move        = rand()%5 + 1;
        (*a)->probability = 1.0f;
        (*a)->next        = NULL;
    }

    do { 

        for (int i=0; i<state_number; ++i) {
            
            // policy evaluation
            Max_Qas  = -FLT_MAX;
            Max_move = e_idle;
            
            start_id = i;
            
            for (j=e_go_up; j<MOVE_TYPE_NUM; ++k) {
                
                // 现实中在同一 state 下，从每个 action 出发，其实是需要经过多轮采样，然后将所有的采样结果求和，再除以采样次数 sampling times.
                // 得到一个近似的 action value 的 return 的 exception, 即 E[G] = sum( g )/ sampling_time. 其中 g 是每次采样的的值。
                // 但在此算法中，在初始化的 action 中，所有的 state 中 action 都是 100%， 所以取多少次样，都是一样，于是只需要取一次即可。
                gamma_exp = 0.f;
                Qas       = 0.f;
                for (k=0; k<trajectory_length; ++k) {

                    consequence = agent_move(agent, start_id, j);
                    Qas         =+ pow(gamma, gamma_exp) * consequence.reward;

                    // 准备下一步动作的计算。
                    start_id    = consequence.stay_id;
                    gamma_exp   += 1.0;                    
                }


                if (Qas > Max_Qas) {

                    Max_Qas  = Qas;
                    Max_move = j;

                }
                
            }

            // policy imporve
            action_t** act = &agent->policy->actions[i];
            (*act)->move = Max_move;
            // 把最大的 action value 当作是 state value 返回。
            Mat2_put(Vs_k, i, 0, Max_Qas);
        }

        // 检查一下算法是否收敛。
        if (Vs_k1 == NULL) {
            Vs_k1 = Mat2_create_cpy(Vs_k);
        } else {

            is_close = Mat2_is_close( Vs_k1, Vs_k, 1e-3 );
            if (!is_close) Mat2_cpy( Vs_k1, Vs_k );
        }

    } while(!is_close && iter++ < epsiodes);

    (*state_value) = Vs_k;
    Mat2_destroy(Vs_k1);

    return 0;
}
/**
 * @brief 基于蒙地卡罗采样 exploring 的 policy iteration 算法。算法开始也是使用那么初始化的策略。
 * 
 * @param agent 
 * @param state_value 
 * @param episode 采样长度。就是算 action value 的时候，采样的长度。
 * @param gamma 
 * @return int 
 */
int agent_policy_iteration_bese_on_monte_carlo_exploring_start(agent_t* agent, matrix2_t** state_value, int episodes, int trajectory_length, float gamma)
{
    int i, j, k;
    int is_close = 0;
    int iter = 0;
    
    // agent->policy->rows = agent->world->rows;
    // agent->policy->cols = agent->world->cols;
    // agent->policy->actions = (action_t**) malloc (agent->world->rows * agent->world->cols * sizeof(action_t*));

    int state_number = agent->world->rows * agent->world->cols;

    float returns[state_number][MOVE_TYPE_NUM];
    int   sa_count[state_number][MOVE_TYPE_NUM];
    //int   sa_hit[state_number][MOVE_TYPE_NUM];
    float Qas[state_number][MOVE_TYPE_NUM];

    float Max_Qas;
    float Cur_Qas;
    move_t Max_move;

    matrix2_t* Vs_k  = Mat2_create(agent->policy->rows, agent->policy->cols);
    matrix2_t* Vs_k1 = NULL;

    Mat2_fill(Vs_k, 0.f);

    // init 
    for (i=0; i<state_number; ++i) {
        action_t** a      = &agent->policy->actions[i];
        (*a)              = (action_t*) malloc (sizeof(action_t));
        (*a)->move        = rand() % (MOVE_TYPE_NUM-1) + 1;
        (*a)->probability = 1.0f;
        (*a)->next        = NULL;
    }

    agent_display_policy(agent);
    srand(time(NULL));

    for (i=0; i<state_number; ++i) {
        for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {
            Qas[i][j]      = -10000.f;
            //sa_hit[i][j]   = 0;
        }
    }

    do {

        // 每次采样前将所有记录清空。
        for (i=0; i<state_number; ++i) {
            for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {
                returns[i][j]  = 0.f;
                sa_count[i][j] = 0;

            }
        }

        // 随机选一个 state action 的对。
        int start_id    = rand() % state_number;
        // 必须是 random 的 action。
        int start_move  = rand() % (MOVE_TYPE_NUM-1) + 1;

        // 这里很重要。必须要跟着改变当前 random 选择的 state 的 move。一些回弹的动作：
        // 例如：原地不动，撞墙的。只会执行一次。
        
        // 这里对选中的 state 的动作进行了修改。原则上来说，所有的 state 的 action 的修改。
        // 应该是在计算完 Qas table 后，根据 Qas table 的值来进行调整。但是这里我的
        // Qas table 我只记录了最大的 Q 值，也就是说，当用新行动进行采样，采样后的 Q 值不如
        // 原来的 Q 值大，那么这个动动作就会改回原来的旧动作。
        agent->policy->actions[start_id]->move = start_move;

        // float reward    = 0;

        // if (start_id == 3 && start_move == e_stay) {
        //     int debug_stop_here = 0;
        // }

        // 这种递归求解 action value 也是不可取，当单次采样超过 100 万步，那么系统就会崩溃。这里也是需要改动，改为将结果用盏的形式保存。
        // policy evaluation
        // int print_debug = 0 && (start_id == 16 && start_move == e_go_right && agent->policy->actions[17]->move == e_stay);
        __exploring(agent, start_id, trajectory_length, gamma, returns, sa_count);
        //if (print_debug) printf("\n\n");
        //printf("reward %0.2f \n", reward);
        // policy imporve
        for (i=0; i<state_number; ++i) {

            Max_Qas    = -FLT_MAX;
            Max_move   = e_idle;

            for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {

                // 计算 exception 的值。
                if (sa_count[i][j] != 0) {

                    // 以下的代码相当重要：
                    // 记录当前这轮的 action value。
                    Cur_Qas = returns[i][j] / sa_count[i][j];

                    // 与历史记录比较，如果大于历史记录，那么将更新成当前的 Qas。如果不进行与最佳历史记录比对，那么当前算出的动作
                    // 会冲刷掉历史最佳动作。这些细节，在《强化学习的数学原理》中是没有提及的。书中只有算法的大概实现方向。
                    if (Cur_Qas > Qas[i][j]) Qas[i][j] = Cur_Qas;

                    //Qas[i][j] = returns[i][j] / sa_count[i][j];
                } 

                if (Qas[i][j] > Max_Qas) {
                    Max_Qas  = Qas[i][j];
                    Max_move = j;
                }
            }

            agent->policy->actions[i]->move = Max_move;
            Mat2_put(Vs_k, (i/agent->policy->cols), (i%agent->policy->cols), Max_Qas);
        }


    } while (1 && iter++ < episodes);

    // for (i=0; i<state_number; ++i) {
    //     printf("state %d: ", i);
    //     for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {
    //         printf("%d ", sa_hit[i][j]);
    //     }
    //     printf("\n");
    // }
    printf("\n");
    for (i=0; i<state_number; ++i) {
        printf("state %d: ", i);
        for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {
            printf("%0.2f ", Qas[i][j]);
        }
        printf("\n");
    }

    (*state_value) = Vs_k;

    return 0;
}

int agent_policy_iteration_base_on_monte_carlo_epsilon_greedy(agent_t* agent, matrix2_t** state_value, int episodes, int trajectory_length, float epsilon, float gamma)
{
    int i, j, k;
    int iter = 0;

    // agent->policy->rows = agent->world->rows;
    // agent->policy->cols = agent->world->cols;
    // agent->policy->actions = (action_t**) malloc (agent->world->rows * agent->world->cols * sizeof(action_t*));

    // memset(agent->policy->actions, NULL, agent->world->rows * agent->world->cols * sizeof(action_t*));

    move_t greedy_move      = e_idle;
    float  greedy_probility = 1 - (epsilon * (MOVE_TYPE_NUM-2)) / (MOVE_TYPE_NUM-1);
    float  other_probility  = epsilon / (MOVE_TYPE_NUM-1);

    int state_number = agent->world->rows * agent->world->cols;

    float Max_Qas;
    float Max_move;
    float qas;
    
    float returns[state_number][MOVE_TYPE_NUM];
    int   sa_count[state_number][MOVE_TYPE_NUM];
    float Qas[state_number][MOVE_TYPE_NUM];

    matrix2_t* Vs = Mat2_create(agent->world->rows, agent->world->cols);

    srand(time(NULL));

    

    for (i=0; i<state_number; ++i) {

        // 随机选中一个主要的前进方向，给予最大 probability。
        // greedy_move = rand() % (MOVE_TYPE_NUM-1) + 1;

        // action_t** act_link = &agent->policy->actions[i];
        
        // for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {

        //     action_t* act = (action_t*) malloc (sizeof(action_t));
        //     act->move = j;
        //     act->probability = (j==greedy_move)? greedy_probility : other_probility;
        //     act->next = (*act_link);
        //     // 挂到链表上。
        //     (*act_link) = act;
        // }

        // 初始化 agent policy, 所有的 action 分配相同概率。
        action_t** act_link = &agent->policy->actions[i];
        for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {

            action_t* act = (action_t*) malloc (sizeof(action_t));
            act->move = j;
            act->probability = 1.f / (MOVE_TYPE_NUM-1) ;
            act->next = (*act_link);
            // 挂到链表上。
            (*act_link) = act;
        }
    }

    // printf("\n");
    // agent_display_policy2(agent);
    // printf("\n");
    
    // 清空所有的 action value
    for (i=0; i<state_number; ++i) {
        for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {
            Qas[i][j] = 0.f;
        }
    }

    // 算法正式开始
    do {

        // 随机选一个 state 作为开始。
        int start_id = rand() % state_number;
        // 随机选一个主要的 action。
        greedy_move = rand() % (MOVE_TYPE_NUM-1) + 1;
        // 选中的这个把它的 action 模型改掉。
        action_t* first = agent->policy->actions[start_id];

        while (first)
        {
            if (first->move == greedy_move) {
                first->probability = greedy_probility;
            } else {
                first->probability = other_probility;
            }
            first = first->next;
        }

        for (i=0; i<state_number; ++i) {
            for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {
                returns[i][j]  = 0.f;
                sa_count[i][j] = 0;
            }
        }
        
        float g = 0;

        //__explor_epsilon_sampling(agent, start_id, trajectory_length, &g, gamma, returns, sa_count);
        __exploring(agent, start_id, trajectory_length, gamma, returns, sa_count);

        // prolicy imporve
        for (i=0; i<state_number; ++i) {

            Max_Qas = -FLT_MAX;
            Max_move = e_idle;
            
            for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {
                if (sa_count[i][j] > 0) {

                    qas = returns[i][j] / sa_count[i][j];
                    if (qas > Qas[i][j]) Qas[i][j] = qas;

                }

                if (Qas[i][j] > Max_Qas) {
                    Max_Qas  = Qas[i][j];
                    Max_move = j;
                }
            }

            // update policy 
            action_t* first = agent->policy->actions[i];
            while (first)
            {
                if (first->move == Max_move) {
                    first->probability = greedy_probility;
                } else {
                    first->probability = other_probility;
                }
                first = first->next;
            }

            Mat2_put(Vs, i/agent->world->cols, i%agent->world->cols, Max_Qas);
        }

    } while (iter++ < episodes);

    (*state_value) = Vs;

    // printf("\n");

    // for (i=0; i<state_number; ++i) {
    //     printf("state %d: ", i);
    //     for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {
    //         printf("%0.2f ", Qas[i][j]);
    //     }
    //     printf("\n");
    // }

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

int agent_load(const char* grid_path, float (*cell_reward)(cell_clazz_t), const char* policy_path, agent_t* agent)
{
    int ret = grid_world_load(grid_path, agent->world, cell_reward);

    // 如果 policy 的路径 null，那么我门
    if (ret == 0) {

        agent->policy->rows = agent->world->rows;
        agent->policy->cols = agent->world->cols;
        if (policy_path) {
            ret = policy_load(policy_path, agent->policy);
        } else {
            // 申请一堆 action 的指针表格。
            agent->policy->actions = (action_t**) malloc (agent->world->rows * agent->world->cols * sizeof (action_t*));
            memset(agent->policy->actions, NULL, agent->world->rows * agent->world->cols * sizeof (action_t*));
        }
    }
    return ret;
}


int agent_display_policy(agent_t* agent)
{
    return policy_display(agent->policy, 1);
}

int agent_display_policy2(agent_t* agent)
{
    return policy_display(agent->policy, 0);
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
            .reward = agent->world->cell_reward(cell.cell_type),
            .stay_id = cell.id
        };
    } else {
        return (consequence_t) {
            .reward  = agent->world->cell_reward(e_null),
            .stay_id = start_id
        };
    }
}

/**
 * @brief 这是赵世钰老师的 《强化学习的数学原理》中第七章 td 算法用于估计 action value 和 impove 的 policy 的。
 * 
 * @param agent 
 * @param state_value 
 * @param episodes 
 * @param trajectory_length 
 * @param gamma 
 * @return int 
 */
int agent_temporal_difference_of_sarsa(agent_t* agent, int start_id, int episodes, int max_trajectory_length, float epsilon, float gamma, float alpha)
{
    int i,j,k;
    int iter = 0;
    int step = 0;
    int world_rows   = agent->world->rows;
    int world_cols   = agent->world->cols;
    int state_number = world_rows * world_cols;
    float qt1_stat, qt_stat, qt_st1at1;
    float Q_table[state_number][MOVE_TYPE_NUM];
    
    float Max_Qsa;
    move_t Max_move;

    action_t* at;
    move_t    mt;

    action_t* at1;
    move_t    mt1;

    int st, st1;
    
    srand(time(NULL));

    // agent->policy->rows = world_rows;
    // agent->policy->cols = world_cols;
    // // 先申请一堆内存处理指针。
    // agent->policy->actions = (action_t**) malloc (world_rows * world_cols * sizeof(action_t*));
    // // 置为空指针，等待使用。应为从来未
    // memset(agent->policy->actions, NULL, world_rows*world_cols*sizeof(action_t));

    // 初始化 Q table，用于更新 policy
    for (i=0; i<state_number; ++i) {
        for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {
            Q_table[i][j] = 0.f;
        }
    }

    // 从 start 开始。
    while (iter++ < episodes) {

        st   = start_id;
        step = 0;

        //printf("%d -", st);
        
        // 在开始前，将所有的的 state 的 action 还原为 0.2 的概率。
        // 算法开始从指定的 state 开始，丢想不到怎么写下去
        while (agent->world->cells[st].cell_type != e_target && step++ < max_trajectory_length) {

            if (agent->policy->actions[st] == NULL) {
                // 如果这个点的 action 为空，那么我们给上一个添加一个随机的 actions
                policy_set_random_moves(&agent->policy->actions[st], e_idle, 1);
            } 

            // 将这个 action 取出来
            at = agent->policy->actions[st];
            // 执行这个 action 查看执行后的 reward
            mt = policy_take_action(at);
            consequence_t consequence = agent_move(agent, st, mt);

            st1 = consequence.stay_id;

            if (agent->policy->actions[st1] == NULL) {
                // 当这个 state 的 action 为空的时候，我们初始化为平均概率。
                policy_set_random_moves(&agent->policy->actions[st1], e_idle, 1);
            }

            at1 = agent->policy->actions[st1];
            mt1 = policy_take_action(at1);

            // 得到结果后开始评估这个 action value
            // todo: 1 把当前的 qa 值拿到。
            qt_stat   = Q_table[st][mt];
            qt_st1at1 = Q_table[st1][mt1];

            // 根据书中 7.2.1 节中的算法描述，式子 7.12 

            qt1_stat = qt_stat - alpha * ( qt_stat - (consequence.reward + (gamma * qt_st1at1)));
            // 更新 Q table, 这玩意可能又是要做那个最大值记忆。
            Q_table[st][mt]  = qt1_stat;
            
            // 马上根据 Q table 更新 policy
            Max_Qsa = -FLT_MAX;
            Max_move = e_idle;

            for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {
                
                if (Max_Qsa < Q_table[st][j] ) {
                    Max_Qsa  = Q_table[st][j];
                    Max_move = j;
                }
            }
            // update policy
            policy_update_greedy_move(agent->policy->actions[st], Max_move, epsilon);
            st = st1;
        }

    };

    // printf("\n");

    // for (i=0; i<state_number; ++i) {
    //     printf("state %d: ", i);
    //     for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {
    //         printf("%0.2f ", Q_table[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");
    // for (i=0; i<state_number; ++i) {
    //     printf("state %d: ", i);
    //     for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {
    //         printf("%d ", Q_touch[i][j]);
    //     }
    //     printf("\n");
    // }

    return 0;
}

/**
 * @brief 这个赵世钰老师的 《强化学习的数学原理》中第七中 td 算法中用于估计 action value 的改进版 Q learning。但是现在还是不太确定它的原理是什么，
 * 
 * @param agent 
 * @param state_value 
 * @param episodes 
 * @param trajectory_length 
 * @param gamma 
 * @return int 
 */
int agent_temporal_difference_of_Q_learning_online(agent_t* agent,  int start_id, int episodes, int max_trajectory_length, float epsilon, float gamma, float alpha)
{
    
    // 这个算法第一步是使用 0.2 的平均策略进行采样需要采样非常多步，然后再使用 RM 算法去找出最优的策略。之所以叫 off-policy。
    // 原因是我行为策略产生的行为策略和目标策略不是同一个东西，这里行为策略只为产生随机样本，然后目标策略在经过样本的毒打后产生
    // 最优的策略。

    int i,j,k;
    int iter = 0; 
    int step = 0;
    int st, st1;
    float qt1_stat, qt_stat, qt_st1a;
    float max_st1a;
    
    action_t* at;
    move_t    mt;
    consequence_t consequence;

    float  Max_Qas;
    move_t Max_move;

    int world_rows   = agent->world->rows;
    int world_cols   = agent->world->cols;

    int state_number = world_rows * world_cols;

    float Q_table[state_number][MOVE_TYPE_NUM];

    // // 初始化 所有的参数
    // agent->policy->rows    = world_rows;
    // agent->policy->cols    = world_cols;
    // agent->policy->actions = (action_t**) malloc (world_rows * world_cols * sizeof(action_t*));
    // memset(agent->policy->actions, NULL, world_rows * world_cols * sizeof(action_t*));

    for (i=0; i<state_number; ++i) {
        for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {
            Q_table[i][j] = 0.f;
        }
    }

    srand(time(NULL));


    while (iter++ < episodes) {

        st   = start_id;
        step = 0;

        while (agent->world->cells[st].cell_type != e_target && step++ < max_trajectory_length) {

            if (agent->policy->actions[st] == NULL) {
                policy_set_random_moves(&agent->policy->actions[st], e_idle, 1);
            }

            at = agent->policy->actions[st];
            mt = policy_take_action(at);

            qt_stat = Q_table[st][mt];

            consequence = agent_move(agent, st, mt);
            st1 = consequence.stay_id;

            // 找到 st1 中 最大的那个 action value
            max_st1a = -FLT_MAX;
            for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {
                if (max_st1a < Q_table[st1][j]) {
                    max_st1a = Q_table[st1][j];
                }
            }

            // 根据《强化学习的数学原理》第 7.4.3 节的算法实现。
            qt1_stat = qt_stat - alpha * (qt_stat - (consequence.reward + gamma * (max_st1a)));
            Q_table[st][mt] = qt1_stat;


            // 马上立刻更新 update policy
            Max_Qas  = - FLT_MAX;
            Max_move = e_idle;

            for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {
                if (Max_Qas < Q_table[st][j]) {
                    Max_Qas = Q_table[st][j];
                    Max_move = j;
                }
            }

            // 马上更新那个变态的 policy
            policy_update_greedy_move(agent->policy->actions[st], Max_move, epsilon);

            st = st1;

            if (agent->world->cells[st].cell_type == e_target) {
                printf("reach target after go %d steps\n ", step);
            }
        }
    }

    // printf("\n");

    // for (i=0; i<state_number; ++i) {
    //     printf("state %d: ", i);
    //     for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {
    //         printf("%0.2f ", Q_table[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");
    // for (i=0; i<state_number; ++i) {
    //     printf("state %d: ", i);
    //     for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {
    //         printf("%d ", Q_touch[i][j]);
    //     }
    //     printf("\n");
    // }
    return 0;
}


/**
 * @brief 根据 《强化学习的数学原理》 的中，7.4 节中算法 7.3 的实现，这里实现跟书中有点不一致，书中算法是在每一个 episode 中更新 target policy 的。
 * 本人的算法实实现是在所有 episode 都跑完后再更新 target policy 的。因为 offline 版本的算法中 target policy 根本不参与 behavior，
 * 所有的 behavior 都在 uniform distribute 的 policy 产生，那么 target policy 根本没必要在每个 episode 都更新。
 * 
 * @param agent 
 * @param start_id 
 * @param episodes 
 * @param max_trajectory_length 
 * @param epsilon 
 * @param gamma 
 * @param alpha 
 * @return int 
 */
int agent_temporal_difference_of_Q_learning_offline(agent_t* agent, int start_id, int episodes, int max_trajectory_length, float epsilon, float gamma, float alpha)
{
    int i,j,k;
    int iter=0, step=0;
    int st, st1;
    float qt1_stat, qt_stat, qt_st1a;
    float max_qt_st1a;
    float Max_Qas;
    move_t Max_move;
    action_t* at;
    move_t    mt;
    consequence_t consequence;
    

    int world_rows = agent->world->rows;
    int world_cols = agent->world->cols;
    int state_number = world_rows * world_cols;
    float Q_table[state_number][MOVE_TYPE_NUM];

    for (i=0; i<state_number; ++i) {
        for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {
            Q_table[i][j] = 0.f;
        }
    }

    //新建一个 behavior policy 用于每个 epsiode 超长采样
    policy_t behavior_policy;
    behavior_policy.rows = world_rows;
    behavior_policy.cols = world_cols;
    behavior_policy.actions = (action_t**) malloc (world_rows * world_cols * sizeof(action_t*));
    memset(behavior_policy.actions, NULL, world_rows * world_cols * sizeof(action_t*));

    // agent->policy->rows = world_rows;
    // agent->policy->cols = world_cols;
    // agent->policy->actions = (action_t**) malloc (world_rows * world_cols * sizeof(action_t*));
    // memset(agent->policy->actions, NULL, world_rows * world_cols * sizeof(action_t*));
    
    policy_t* target_policy = agent->policy;

    // 将当前的 agent 的 policy 设置为 behavior policy。用于产生足够多的采样。
    // behavior policy 的 action 采用 uniform disturb （平均分布）

    agent->policy = &behavior_policy;
    for (i=0; i<state_number; ++i) {
        policy_set_random_moves(&agent->policy->actions[i], e_idle, 1);
    }

    for (i=0; i<state_number; ++i) {
        for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {
            Q_table[i][j] = 0.f;
        }
    }

    while (iter++ < episodes) {

        // 每次  epsiodes 的时候，用 behavior policy 走 behavior policy sampling length 步来采样。
        st   = start_id;
        step = 0;
        while (agent->world->cells[st].cell_type != e_target &&  step++ < max_trajectory_length ) {

            // 此时的 agent 身上挂着的是 uniform distribute 的 policy，进行均匀的随机采样。

            at = agent->policy->actions[st];

            mt = policy_take_action(at);

            consequence = agent_move(agent, st, mt);

            st1 = consequence.stay_id;

            max_qt_st1a = -FLT_MAX;
            for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {
                
                if (max_qt_st1a < Q_table[st1][j]) {
                    max_qt_st1a = Q_table[st1][j];
                }
            }

            qt_stat = Q_table[st][mt];

            // 根据 《强化学习的数学原理》中算法 7.3 来实现。 Q learning 的 offline 版本。
            qt1_stat = qt_stat - alpha * ( qt_stat - (consequence.reward + gamma * max_qt_st1a));

            Q_table[st][mt] = qt1_stat;

            st = st1;

            if (agent->world->cells[st].cell_type == e_target) {
                printf("reach target after going %d steps\n", step);
            }
        }
    }

    // 最后使用训练过的 Q_table 统一更新 target policy 
    
    for (i=0; i<state_number; ++i) {
        Max_Qas  = -FLT_MAX;
        Max_move = e_idle;
        for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {

            if ( Max_Qas < Q_table[i][j]) {
                Max_Qas = Q_table[i][j];
                Max_move = j;
            }
        }

        // 使用绝对的概率跟新最大动作。
        //policy_update_greedy_move(target_policy->actions[i], Max_move, 0);
        policy_set_random_moves(&target_policy->actions[i], Max_move, 0);
    }

    // 把 target policy 挂回去。
    agent->policy = target_policy;
    
    policy_reset(&behavior_policy);
    return 0;
}

/**
 * @brief 此函数使用 function approximator 来进行 state value 逼近算法，《强化学习的数学原理》第八章 8.2 中的算法 8.1。
 * 
 * @param agent 
 * @param state_values 
 * @param W 
 * @param start_id 
 * @param epsiodes 
 * @param epsilon 
 * @param gamma 
 * @return int 
 */
int agent_value_function_approximation_of_td_state_value_with_linear_function(agent_t* agent, matrix2_t** state_values_out, int epsiodes, int trajctory_length, matrix2_t** W_out, int dimens, int (*S_figure)(matrix2_t*, int, int), float alpha, float gamma)
{
    int i,j,k;
    int iter = 0, step = 0;
    int S_id;
    int S1_id;
    float SdotW;
    float S1dotW;
    int world_rows = agent->world->rows;
    int world_cols = agent->world->cols;

    int state_number = world_rows * world_cols;

    matrix2_t* Vs = Mat2_create(world_rows, world_cols);
    matrix2_t* S  = Mat2_create(1, dimens);
    matrix2_t* S1 = Mat2_create(1, dimens);
    matrix2_t* W  = Mat2_create(dimens, 1);
    matrix2_t* delta_W = Mat2_create(dimens, 1);

    consequence_t consequence;
    action_t*     at;
    move_t        mt;
    
    Mat2_fill_random(W, 0,1);

    srand(time(NULL));

    while (iter++ <epsiodes) {

        step = 0;
        S_id = 0;
        while (step++ < trajctory_length) {

            at = agent->policy->actions[S_id];
            mt = policy_take_action(at);

            consequence = agent_move(agent, S_id, mt);
            S1_id = consequence.stay_id;

            S_figure(S, S_id/world_cols, S_id%world_cols);
            S_figure(S1, S1_id/world_cols, S1_id%world_cols);

            // 8.2 章节中，式子（8.13）中 delta V_hat, 在线性函数情况下，直接就是 S。
            Mat2_cpy(delta_W, S);

            Mat2_T(delta_W);

            Mat2_dot(S, W);
            Mat2_dot(S1, W);

            SdotW  = S->pool[0];
            S1dotW = S1->pool[0];

            // 
            float state_error = consequence.reward + gamma * S1dotW - SdotW;
            //printf("state_error : %0.3f\n", state_error);

            Mat2_scalar_multiply(delta_W, alpha * state_error);

            Mat2_add(W, delta_W);

            // 在经过 S 与 W 的点乘，S1 与 W 点乘后，S 变成了标量，于是只能把他的形状变回去。
            Mat2_reshape(S, 1, dimens);
            Mat2_reshape(S1, 1 , dimens);

            // 往前走。
            S_id = S1_id;
        }

    }

    // 计算完 W 后，我们开始使用 W 来推算 stat value
    Mat2_reshape(S, 1, dimens);

    for (i=0; i<world_rows; ++i) {

        for (j=0; j<world_cols; ++j) {

            S_figure(S, i, j);

            Mat2_dot(S, W);

            Mat2_put(Vs, i, j, S->pool[0]);
            
            Mat2_reshape(S, 1, dimens);
        }

    }

    // 删除用过的中间变量。
    Mat2_destroy(S);
    Mat2_destroy(S1);
    Mat2_destroy(delta_W);

    // 将结果输出到外面。
    *state_values_out = Vs;
    *W_out            = W;

    return 0;
}

/**
 * @brief 《强化学习的数学原理》中算法 8.3.1，使用函数逼近 action value。这里是 sarsa 算法。
 * 
 * @param agent 
 * @param W_out 
 * @param start_id 
 * @param episodes 
 * @param trajectory_length 
 * @param dimens 
 * @param S_transform 
 * @param alpah 
 * @param gamma 
 * @return int 
 */
int agent_value_function_approximation_sarsa_with_linear_function(agent_t* agent, matrix2_t** W_out, int start_id, int episodes, int trajectory_length, int dimens, int (*Q_figure)(matrix2_t*, int, int, int), float alpah, float gamma, float epsilon)
{
    int i,j,k;
    int st, st1;
    int iter = 0, step = 0;
    
    int world_rows = agent->world->rows;
    int world_cols = agent->world->cols;
    int state_number = world_rows * world_cols;

    float QdotW;
    float Q1dotW;
    float qa_error = 0.f;
    float last_qa_error = 0.f;

    float  Max_Qas;
    move_t Max_move;

    action_t*     at;
    action_t*     at1;
    move_t        mt;
    move_t        mt1;
    consequence_t consequence;
    
    matrix2_t* Q  = Mat2_create(1, dimens); 
    matrix2_t* Q1 = Mat2_create(1, dimens);
    matrix2_t* W  = Mat2_create(dimens, 1);
    matrix2_t* delta_W = Mat2_create(dimens, 1);
    Mat2_fill_random(W, 0, 1);
    srand(time(NULL));

    for (i=0;i<state_number; ++i) {
        policy_set_random_moves(&agent->policy->actions[i], e_go_up, 1);
    }

    while (iter++ < episodes) {

        step = 0;
        st = start_id;
        while ( agent->world->cells[st].cell_type != e_target && step++ < trajectory_length ) {

            // 用之前塑形。
            Mat2_reshape(Q, 1, dimens);
            Mat2_reshape(Q1, 1, dimens);

            at = agent->policy->actions[st];
            mt = policy_take_action(at);

            consequence = agent_move(agent, st, mt);
            st1 = consequence.stay_id;

            at1 = agent->policy->actions[st1];
            mt1 = policy_take_action(at1);

            Q_figure(Q, st/world_cols, st%world_cols, mt);
            Q_figure(Q1, st1/world_cols, st1%world_cols, mt1);

            // 如果 Q_hat 是线性函数，那么我们他的导数就是Q，因为 delta(Q dot W) = Q;
            Mat2_cpy(delta_W, Q);
            Mat2_T(delta_W);
            
            Mat2_dot(Q, W);
            Mat2_dot(Q1, W);

            QdotW  = Q->pool[0];
            Q1dotW = Q1->pool[0];
            
            qa_error = consequence.reward + gamma * Q1dotW - QdotW;

            /* for debug */
            float convergence_error = sqrt((last_qa_error - qa_error)*(last_qa_error - qa_error));
            printf("St: %d, St1: %d, QdotW: %0.2f, Q1dotW: %0.2f, reward: %0.2f, last_error: %0.3f, curr_error: %0.3f, error convergence: %0.3f \n", \ 
                st, st1, QdotW, Q1dotW, consequence.reward, last_qa_error, qa_error, convergence_error \
            );
            last_qa_error = qa_error;
            /* for debug */

            Mat2_scalar_multiply(delta_W, alpah * qa_error);

            // 更新 W 权重参数。
            Mat2_add(W, delta_W);

            // 更新 W 后开始跟新 policy。
            Max_Qas  = -FLT_MAX;
            Max_move = e_idle;
            
            for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {

                Mat2_reshape(Q, 1, dimens);
                
                Q_figure(Q, st/world_rows, st%world_cols, j);
                
                Mat2_dot(Q, W);

                QdotW = Q->pool[0];

                if (Max_Qas < QdotW) {
                    Max_Qas = QdotW;
                    Max_move = j;
                }
            }

            // 更新这个 policy。
            policy_update_greedy_move(agent->policy->actions[st], Max_move, epsilon);
            // 继续下一个点的计算。

            st = st1;

            if (agent->world->cells[st].cell_type == e_target) {
                printf("reach target after %d steps \n", step);
            }

        }
    }

    Mat2_destroy(Q);
    Mat2_destroy(Q1);
    Mat2_destroy(delta_W);
    (*W_out) = W;
    return 0;
}






