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

static int __explor_sampling(agent_t* agent, int start_id, move_t move,  int steps, float* reward, float gamma, float (*returns)[MOVE_TYPE_NUM], int (*sa_count)[MOVE_TYPE_NUM])
{
    
    // 往前走一步，
    consequence_t consequence = agent_move(agent, start_id, move);
    steps--;

    if (steps != 0) {
        // 若是没有走完，计算下一步要走的方向。
        int       next_id   = consequence.stay_id;
        move_t    next_move = agent->policy->actions[next_id]->move;
        __explor_sampling(agent, next_id, next_move, steps, reward, gamma, returns, sa_count, disp_debug);
    }

    
    (*reward) = consequence.reward + (*reward) * gamma;
    
    // if (disp_debug) {
    //     printf("<-(id:%d, move: %d, g: %0.2f)", start_id, move, (*reward));
    // }

    returns[start_id][move]  += (*reward);
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

    agent->policy->rows = agent->world->rows;
    agent->policy->cols = agent->world->cols;
    agent->policy->actions = (action_t**) malloc (agent->world->rows * agent->world->cols * sizeof(action_t*));
    
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
    agent->policy->rows = agent->world->rows;
    agent->policy->cols = agent->world->cols;
    agent->policy->actions = (action_t**) malloc (agent->world->rows * agent->world->cols * sizeof(action_t*));

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
int agent_policy_iteration_bese_on_monte_carlo_exploring(agent_t* agent, matrix2_t** state_value, int episodes, int trajectory_length, float gamma)
{
    int i, j, k;
    int is_close = 0;
    int iter = 0;
    
    agent->policy->rows = agent->world->rows;
    agent->policy->cols = agent->world->cols;
    agent->policy->actions = (action_t**) malloc (agent->world->rows * agent->world->cols * sizeof(action_t*));

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
        agent->policy->actions[start_id]->move = start_move;

        sa_hit[start_id][start_move] += 1;
        float reward    = 0;

        if (start_id == 3 && start_move == e_stay) {
            int debug_stop_here = 0;
        }
        // policy evaluation
        // int print_debug = 0 && (start_id == 16 && start_move == e_go_right && agent->policy->actions[17]->move == e_stay);
        __explor_sampling(agent, start_id, start_move, trajectory_length, &reward, gamma, returns, sa_count);
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
    // printf("\n");
    // for (i=0; i<state_number; ++i) {
    //     printf("state %d: ", i);
    //     for (j=e_go_up; j<MOVE_TYPE_NUM; ++j) {
    //         printf("%0.2f ", Qas[i][j]);
    //     }
    //     printf("\n");
    // }

    (*state_value) = Vs_k;

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



