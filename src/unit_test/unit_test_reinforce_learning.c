/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-08-23 13:39:18
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-10-22 11:29:22
 * @FilePath: /boring-code/src/unit_test/unit_test_reinforce_learning.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include <CUnit/Basic.h>
#include <time.h>
#include <limits.h>
#include "matrix2/matrix2.h"
#include "reinforce_learning/grid_world.h"
#include "reinforce_learning/policy.h"
#include "reinforce_learning/agent.h"
#include "reinforce_learning/neural_network.h"
#include "reinforce_learning/neural_network_functions.h"



static const float cell_reward_1[4]   = {-1., 0, -10., 1};
static const float cell_reward_ql[4]  = {-10., -1., -10., 0.,};
static const float cell_reward_ql_offline[4] = {-10., 0, -10, 10};
static const float cell_reward_fa_td[4] = {-1, 0, -1, 1};
static const float cell_reward_fa_sarsa[4] = {-10.f, 0.f, -10.f, 1.f};

static int  suite_success_init (void) 
{
    printf("\n RL suite success init\n");
    return 0;
}


static int suite_success_clean (void) 
{   
    printf("\n RL suite success clean\n");
    return 0;
}

static float cell_reward_a(cell_clazz_t cell_type) {
    return cell_reward_1[cell_type];
}

static float cell_reward_b(cell_clazz_t cell_type) {
    return cell_reward_ql[cell_type];
}

static float cell_reward_c(cell_clazz_t cell_type) {
    return cell_reward_ql_offline[cell_type];
}

static float cell_reward_d(cell_clazz_t cell_type) {
    return cell_reward_fa_td[cell_type];
}

static float cell_reward_e(cell_clazz_t cell_type) {
    return cell_reward_fa_sarsa[cell_type];
}

static int S_3_dimens_transform (matrix2_t* S, int x, int y) 
{
    S->pool[0] = 1.0f;
    S->pool[1] = x;
    S->pool[2] = y;
    return 0;
}

static int S_6_dimens_transform(matrix2_t* S, int x, int y) 
{
    S->pool[0] = 1.f;
    S->pool[1] = (float)x / (float) 4;
    S->pool[2] = (float)y / (float) 4;
    S->pool[3] = (float)(x*x) / (float) (4*4) ;
    S->pool[4] = (float)(y*y) / (float) (4*4);
    S->pool[5] = (float)(x*y) / (float) (4*4);
    return 0;
}

static int Q_4_dimens_feature(matrix2_t* Q, int x, int y, int mt) 
{
    Q->pool[3] = 1.f;
    // Q->pool[1] = (float)x/4;
    // Q->pool[2] = (float)y/4;
    // Q->pool[3] = (float)mt/5;
    Q->pool[0] = x;
    Q->pool[1] = y;
    Q->pool[2] = mt;
    return 0;
}

static int Q_8_dimens_fourier_feature(matrix2_t* Q, int x, int y, int mt)
{   const float pi = 3.1415926;
    float fx = (float) x / 4.f;
    float fy = (float) y / 4.f;
    float fmt = (float) (mt -1) / 4.f;
    Q->pool[0] = cos(pi * (0*fx + 0*fy + 0*fmt));
    Q->pool[1] = cos(pi * (0*fx + 0*fy + 1*fmt));
    Q->pool[2] = cos(pi * (0*fx + 1*fy + 0*fmt));
    Q->pool[3] = cos(pi * (0*fx + 1*fy + 1*fmt));
    Q->pool[4] = cos(pi * (1*fx + 0*fy + 0*fmt));
    Q->pool[5] = cos(pi * (1*fx + 0*fy + 1*fmt));
    Q->pool[6] = cos(pi * (1*fx + 1*fy + 0*fmt));
    Q->pool[7] = cos(pi * (1*fx + 1*fy + 1*fmt));
    return 0;
}   

/**
 * @brief 这个傅立叶是非常他妈的重要的吊他娘娘的，搞了三天，一直忽视这个函数，导致调试了三天才能算出收敛的 policy。操他妈个逼。自己就是大傻逼。
 * 《强化学习的数学原理》书中 8.2 节中关于线性逼近函数的 feature，如何构造的。书上只是展示了输入是 2 的情况，当输入为 3 的时候，例如本本例子中，
 * 我需要输入 x y 和 at 这三个参数，那么整个 feature 的维度则为 (q+1)^3, q 是指为每个输入的参数提供 q+1 种的系数，例如：输入为 x1，x2, x3.
 * q 为 2， x1 有 0*x1，1*x1，2*x3，这三种系数组合，加上有 3 个输入参数，那么总共的组合有 (2 + 1)^3 = 9, feature 的维度为 9.
 * @param Q 
 * @param x 
 * @param y 
 * @param mt 
 * @return int 
 */
static int Q_x_dimens_fourier_feature(matrix2_t* Q, int x, int y, int mt) 
{
    const float pi = 3.1415926;
    float fx = (float) x / 4.f;
    float fy = (float) y / 4.f;
    float fmt = (float) (mt-1) / 4.f;

    int dimens = Q->rows * Q->cols;
    int q = pow(dimens, 1.f/3.f) - 1;

    int i=0;
    for (int c1=0; c1<q+1; c1++) {
        for (int c2=0; c2<q+1; c2++) {
            for (int c3=0; c3<q+1; c3++) {
                Q->pool[i++] = cos ( pi * (c1*fx + c2*fy + c3*fmt) );
            }
        }
    }
    return 0;
}

static int Q_9_dimens_feature(matrix2_t* Q, int x, int y, int mt) 
{
    Q->pool[0] = 1.f;
    Q->pool[1] = (float) (x) / (float) 4;
    Q->pool[2] = (float) (y) / (float) 4;
    Q->pool[3] = (float) (mt) / (float) 5;
    Q->pool[4] = (float) (x*x) / (float)(4*4);
    Q->pool[5] = (float) (y*y) / (float)(4*4);
    Q->pool[6] = (float) (mt*mt) / (float)(5*5);
    Q->pool[7] = (float) (x*y) / (float)(4*4);
    Q->pool[8] = (float) (x*mt) / (float)(4*5);
    return 0;
}

static int S_2_dimens_transform(matrix2_t* S, int x, int y) 
{
    S->pool[0] = x;
    S->pool[1] = y;
    return 0;
}
static void display_state_value(agent_t* agent, matrix2_t* state_value) 
{
    for (int i=0; i<agent->world->rows; ++i) {
        for (int j=0; j<agent->world->cols; ++j) {

            printf(" %0.2f", state_value->pool[i*agent->world->cols+j]);

        }
        printf("\n");
    }
}

static void test_grid_world_save_load(void) 
{
    const char* save_path = "/Users/zuweie/code/c-projects/boring-code/src/unit_test/reinforce_learning_data/g5x5_save.txt";
    const char* load_path = "/Users/zuweie/code/c-projects/boring-code/src/unit_test/reinforce_learning_data/g5x5_load.txt";
    grid_world_t grid = (grid_world_t){.cells=NULL, .rows=0, .cols=0};
    grid_world_load(load_path, &grid, &cell_reward_a);
    grid_world_display(&grid);
    grid_world_save(save_path, &grid);
    grid_world_reset(&grid);
    return;
}

static void test_policy_load(void) 
{
    const char* grid_path   = "/Users/zuweie/code/c-projects/boring-code/src/unit_test/reinforce_learning_data/g2x2.txt";
    const char* policy_path = "/Users/zuweie/code/c-projects/boring-code/src/unit_test/reinforce_learning_data/a2x2.txt";
    agent_t agent;
    agent_init(&agent);
    agent_load(grid_path, &cell_reward_a, policy_path, &agent);
    printf("\n grid world: \n");
    agent_display_gridworld(&agent);
    printf("\n policy: \n");
    agent_display_policy(&agent);
    agent_reset(&agent);
    return;
    
}
static void test_state_value_calculate(void) 
{
    const char* grid_path   = "/Users/zuweie/code/c-projects/boring-code/src/unit_test/reinforce_learning_data/g2x2.txt";
    const char* policy_path = "/Users/zuweie/code/c-projects/boring-code/src/unit_test/reinforce_learning_data/a2x2.txt";
    agent_t agent;
    agent_init(&agent);
    agent_load(grid_path, &cell_reward_a, policy_path, &agent);
    matrix2_t* rewards = NULL;
    matrix2_t* transition = NULL;
    matrix2_t* Vs = NULL;
    agent_calculate_state_values(&agent, &Vs, &rewards, &transition, INT_MAX, 0.9);

    MAT2_INSPECT(Vs);
    MAT2_INSPECT(rewards);
    MAT2_INSPECT(transition);

    Mat2_destroy(Vs);
    Mat2_destroy(rewards);
    Mat2_destroy(transition);

    agent_reset(&agent);
    return;
}

static void test_state_value_calcualte2(void) 
{
    const char* grid_path   = "/Users/zuweie/code/c-projects/boring-code/src/unit_test/reinforce_learning_data/g5x5.txt";
    const char* policy_path = "/Users/zuweie/code/c-projects/boring-code/src/unit_test/reinforce_learning_data/a5x5.txt";
    agent_t agent;
    agent_init(&agent);
    agent_load(grid_path, &cell_reward_a, policy_path, &agent);
    
    matrix2_t* Vs         = NULL;
    matrix2_t* rewards    = NULL;
    matrix2_t* transition = NULL;
    agent_calculate_state_values(&agent, &Vs, &rewards, &transition, INT_MAX, 0.9);

    MAT2_INSPECT(rewards);
    MAT2_INSPECT(transition);
    
    for (int i=0; i<agent.world->rows; ++i) {
        for (int j=0; j<agent.world->cols; ++j) {

            printf(" %0.2f", Vs->pool[i*agent.world->cols+j]);

        }
        printf("\n");
    }

    Mat2_destroy(Vs);
    Mat2_destroy(transition);
    Mat2_destroy(rewards);

    agent_reset(&agent);
    return;
}

static void test_boe_value_iteration(void) 
{
    const char* grid_path = "/Users/zuweie/code/c-projects/boring-code/src/unit_test/reinforce_learning_data/g5x5.txt";
    agent_t agent;
    matrix2_t* state_value;
    printf("\n");
    agent_init(&agent);
    agent_load(grid_path, &cell_reward_a, NULL, &agent);
    agent_display_gridworld(&agent);
    agent_value_iteration(&agent, &state_value, 0.9);
    printf("\n");
    agent_display_policy(&agent);
    printf("\n");
    display_state_value(&agent, state_value);
    Mat2_destroy(state_value);
    agent_reset(&agent);
}

static void test_boe_policy_iteration(void) 
{

    const char* grid_path = "/Users/zuweie/code/c-projects/boring-code/src/unit_test/reinforce_learning_data/g5x5.txt";
    agent_t agent;
    matrix2_t* state_value;
    printf("\n");
    agent_init(&agent);
    agent_load(grid_path, &cell_reward_a, NULL, &agent);
    agent_display_gridworld(&agent);
    agent_policy_itreation(&agent, &state_value, 0.9);
    printf("\n");
    agent_display_policy(&agent);
    printf("\n");
    display_state_value(&agent, state_value);
    Mat2_destroy(state_value);
    agent_reset(&agent);
    return;
}

static void test_boe_policy_iteration_base_mc_exploring(void) 
{
    const char* grid_path = "/Users/zuweie/code/c-projects/boring-code/src/unit_test/reinforce_learning_data/g5x5.txt";
    agent_t agent;
    matrix2_t* state_value;
    printf("\n");
    agent_init(&agent);
    agent_load(grid_path, &cell_reward_a, NULL, &agent);
    printf("\n");
    agent_display_gridworld(&agent);
    agent_policy_iteration_bese_on_monte_carlo_exploring_start(&agent, &state_value, 6000, 15, 0.9);
    printf("\n");
    agent_display_policy(&agent);
    printf("\n");
    display_state_value(&agent, state_value);
    printf("\n");
    Mat2_destroy(state_value);
    agent_reset(&agent);
    return;
}

static void test_action_probability(void) 
{
    const int TEST_NUM = 10000;
    float epsilon = 0.2;
    float p_choose = 1 - epsilon * (MOVE_TYPE_NUM - 2) / (MOVE_TYPE_NUM-1);
    float p_other  = epsilon / (MOVE_TYPE_NUM-1);

    agent_t agent;
    agent_init(&agent);
    agent.policy->actions = (action_t**) malloc (sizeof(action_t*));

    action_t* act1 = (action_t*) malloc(sizeof(action_t));
    act1->move = e_go_up;
    act1->probability = p_other;
    act1->next = NULL;

    action_t* act2 = (action_t*) malloc (sizeof(action_t));
    act2->move = e_go_right;
    act2->probability = p_other;
    act2->next = act1;

    action_t* act3 = (action_t*) malloc (sizeof(action_t));
    act3->move = e_go_down;
    act3->probability = p_other;
    act3->next = act2;

    action_t* act4 = (action_t*) malloc (sizeof(action_t));
    act4->move = e_go_left;
    act4->probability = p_other;
    act4->next = act3;

    action_t* act5 = (action_t*) malloc (sizeof(action_t));
    act5->move = e_stay;
    act5->probability = p_choose;
    act5->next = act4;
    
    agent.policy->actions[0] = act5;

    srand(time(NULL));

    int num_up = 0, num_right = 0, num_down = 0, num_left = 0, num_stay = 0;
    for (int i=0; i<TEST_NUM; ++i) {
        move_t move = policy_take_action(agent.policy->actions[0]);
        switch (move)
        {
        case e_go_up:
            num_up++;
            break;
        case e_go_down:
            num_down++;
            break;
        case e_go_right:
            num_right++;
            break;
        case e_go_left:
            num_left++;
            break;
        case e_stay:
            num_stay++;
            break;
        default:
            break;
        }
    }
    float p_up    = (float) num_up / (float) TEST_NUM;
    float p_right = (float) num_right / (float ) TEST_NUM;
    float p_down  = (float) num_down / (float) TEST_NUM;
    float p_left  = (float) num_left / (float) TEST_NUM;
    float p_stay  = (float) num_stay / (float) TEST_NUM;

    printf("\n");
    printf("choose probability: %0.2f, other probability: %0.2f\n", p_choose, p_other);
    printf("up: %d / %d = %0.2f, right: %d / %d = %0.2f , down: %d / %d = %0.2f, left: %d / %d = %0.2f, stay: %d / %d = %0.2f\n", \
        num_up, TEST_NUM, p_up, \
        num_right, TEST_NUM, p_right, \
        num_down, TEST_NUM,  p_down, \
        num_left, TEST_NUM, p_left, \
        num_stay, TEST_NUM, p_stay);
    
    agent_reset(&agent);
}

static void test_boe_policy_iteration_epsilon_greedy_exploring (void)
{
    const char* grid_path = "/Users/zuweie/code/c-projects/boring-code/src/unit_test/reinforce_learning_data/g5x5.txt";
    agent_t agent;
    matrix2_t* state_value;
    printf("\n");
    agent_init(&agent);
    agent_load(grid_path, &cell_reward_a, NULL, &agent);
    printf("\n");
    agent_display_gridworld(&agent);
    // printf("\n");
    // agent_display_policy2(&agent);
    agent_policy_iteration_base_on_monte_carlo_epsilon_greedy(&agent, &state_value, 10000, 15, 0.1f, 0.7);
    // printf("\n");
    // agent_display_policy(&agent);
    printf("\n");
    agent_display_policy2(&agent);
    printf("\n");
    MAT2_INSPECT(state_value);
    Mat2_destroy(state_value);
    agent_reset(&agent);
    return;
}

static void test_temploral_difference_for_state_value(void)
{
    const char* grid_path   = "/Users/zuweie/code/c-projects/boring-code/src/unit_test/reinforce_learning_data/g5x5.txt";
    const char* policy_path = "/Users/zuweie/code/c-projects/boring-code/src/unit_test/reinforce_learning_data/a5x5.txt";
    agent_t agent;
    matrix2_t* state_values;
    agent_init(&agent);
    agent_load(grid_path, &cell_reward_a, policy_path, &agent);
    agent_temporal_difference_for_state_value(&agent, &state_values, 0.5, 0.9, 1000);
    printf("\n");
    agent_display_gridworld(&agent);
    agent_display_policy(&agent);
    MAT2_INSPECT(state_values);
    Mat2_destroy(state_values);
    agent_reset(&agent);
    return;

}

static void test_temploral_difference_for_boe_sarsa(void) {
    const char* grid_path = "/Users/zuweie/code/c-projects/boring-code/src/unit_test/reinforce_learning_data/g5x5.txt";
    agent_t agent;
    agent_init(&agent);
    agent_load(grid_path, &cell_reward_a, NULL, &agent);
    agent_temporal_difference_of_sarsa(&agent, 0, 600, 700, 0.1, 0.9, 0.1);
    printf("\n");
    agent_display_gridworld(&agent);
    agent_display_policy2(&agent);
    agent_reset(&agent);
    return;
}

static void test_Q_learning_online(void) 
{
    const char* grid_path = "/Users/zuweie/code/c-projects/boring-code/src/unit_test/reinforce_learning_data/g5x5.txt";
    agent_t agent;
    agent_init(&agent);
    agent_load(grid_path, &cell_reward_b, NULL, &agent);
    agent_temporal_difference_of_Q_learning_online(&agent, 0, 300, 10000, 0.1, 0.9, 0.1);
    printf("\n");
    agent_display_gridworld(&agent);
    agent_display_policy2(&agent);
    agent_reset(&agent);
    return;
}

static void test_Q_learning_offline(void) 
{
    const char* grid_path = "/Users/zuweie/code/c-projects/boring-code/src/unit_test/reinforce_learning_data/g5x5.txt";
    agent_t agent;
    agent_init(&agent);
    agent_load(grid_path, &cell_reward_c, NULL, &agent);
    agent_temporal_difference_of_Q_learning_offline(&agent, 0, 500, 10000, 0.1, 0.9, 0.1);
    printf("\n");
    agent_display_gridworld(&agent);
    agent_display_policy2(&agent);
    agent_reset(&agent);
    return;
}

static void test_function_approximation_of_td_state_value(void) 
{
    matrix2_t* state_value = NULL;
    matrix2_t* state_value2 = NULL;
    matrix2_t* state_value3 = NULL;
    matrix2_t* reward = NULL;
    matrix2_t* transform = NULL;
    matrix2_t* W_out = NULL;
    const char* grid_path = "/Users/zuweie/code/c-projects/boring-code/src/unit_test/reinforce_learning_data/g5x5.txt";
    agent_t agent;
    agent_init(&agent);
    agent_load(grid_path, &cell_reward_d, NULL, &agent);

    // 
    // srand(time(NULL));
    int state_number = agent.policy->rows * agent.policy->cols;

    for (int i=0; i<state_number; ++i) {
        policy_set_random_moves(&agent.policy->actions[i], e_go_up, 1);
    }

    agent_calculate_state_values(&agent, &state_value3, &reward, &transform, 1000, 0.9);

    agent_temporal_difference_for_state_value(&agent, &state_value, 0.1, 0.9, 1000);

    agent_value_function_approximation_of_td_state_value_with_linear_function(     \
        &agent, &state_value2, 500, 500, &W_out, 3, &S_3_dimens_transform, 0.1, 0.9 \
    );

    printf("\n");

    agent_display_gridworld(&agent);
    agent_display_policy2(&agent);

    Mat2_reshape(state_value3, 5, 5);

    MAT2_INSPECT(state_value3);
    MAT2_INSPECT(state_value);
    MAT2_INSPECT(state_value2);

    MAT2_INSPECT(W_out);

    Mat2_destroy(state_value);
    Mat2_destroy(state_value2);
    Mat2_destroy(state_value3);
    Mat2_destroy(reward);
    Mat2_destroy(transform);
    Mat2_destroy(W_out);

    agent_reset(&agent);

    return;

}

static void test_function_approximation_of_sarsa(void) 
{
    matrix2_t* W_out;
    const char* grid_path = "/Users/zuweie/code/c-projects/boring-code/src/unit_test/reinforce_learning_data/g5x5.txt";
    agent_t agent;
    agent_init(&agent);
    agent_load(grid_path, &cell_reward_e, NULL, &agent);
    printf("\n\n");
    agent_value_function_approximation_sarsa_with_linear_function(           \
        &agent, &W_out, 0, 500, 10000, pow((6+1), 3), &Q_x_dimens_fourier_feature, 0.001, 0.9, 0.1 \
    );

    printf("\n");
    MAT2_INSPECT(W_out);

    agent_display_gridworld(&agent);
    printf("\n");
    agent_display_policy2(&agent);
    Mat2_destroy(W_out);

    agent_reset(&agent);

    return;
}

static void nn_progress(const char* log_str, float err, int step) 
{
    char buffer[1024];
    memset(buffer, 0x0, sizeof(buffer));
    sprintf(buffer, "%s step: %d, error: %0.2f", log_str, step, err);
    printf("%s\r", buffer);
    fflush(stdout);
}

static void test_nn(void) 
{
    #define x_data_row 60
    #define x_data_col 4

    #define y_data_row 60
    #define y_data_col 3

    vfloat_t trainingDatas[x_data_row][x_data_col] ={
    /************* S *************/
        {5.1f, 3.5f, 1.4f, 0.2f}, 
        {4.9f, 3.0f, 1.4f, 0.2f}, 
        {4.7f, 3.2f, 1.3f, 0.2f},

        {4.6f, 3.1f, 1.5f, 0.2f}, 
        {5.0f, 3.6f, 1.4f, 0.2f}, 
        {5.4f, 3.9f, 1.7f, 0.4f},

        {4.6f, 3.4f, 1.4f, 0.3f}, 
        {5.0f, 3.4f, 1.5f, 0.2f}, 
        {4.4f, 2.9f, 1.4f, 0.2f},

        {4.9f, 3.1f, 1.5f, 0.1f}, 
        {5.4f, 3.4f, 1.5f, 0.2f}, 
        {4.8f, 3.4f, 1.6f, 0.2f},

        {4.8f, 3.0f, 1.4f, 0.1f}, 
        {4.3f, 3.0f, 1.1f, 0.1f}, 
        {5.8f, 4.0f, 1.2f, 0.2f},

        {5.7f, 4.4f, 1.5f, 0.4f}, 
        {5.4f, 3.9f, 1.3f, 0.4f}, 
        {5.1f, 3.5f, 1.4f, 0.3f},

        {5.7f, 3.8f, 1.7f, 0.3f}, 
        {5.1f, 3.8f, 1.5f, 0.3f},
    /************* v ************/
        {7.0f, 3.2f, 4.7f, 1.4f}, 
        {6.4f, 3.2f, 4.5f, 1.5f}, 
        {6.9f, 3.1f, 4.9f, 1.5f},

        {5.5f, 2.3f, 4.0f, 1.3f}, 
        {6.5f, 2.8f, 4.6f, 1.5f}, 
        {5.7f, 2.8f, 4.5f, 1.3f}, 

        {6.3f, 3.3f, 4.7f, 1.6f}, 
        {4.9f, 2.4f, 3.3f, 1.0f}, 
        {6.6f, 2.9f, 4.6f, 1.3f},

        {5.2f, 2.7f, 3.9f, 1.4f}, 
        {5.0f, 2.0f, 3.5f, 1.0f}, 
        {5.9f, 3.0f, 4.2f, 1.5f}, 

        {6.0f, 2.2f, 4.0f, 1.0f}, 
        {6.1f, 2.9f, 4.7f, 1.4f}, 
        {5.6f, 2.9f, 3.6f, 1.3f}, 

        {6.7f, 3.1f, 4.4f, 1.4f}, 
        {5.6f, 3.0f, 4.5f, 1.5f}, 
        {5.8f, 2.7f, 4.1f, 1.0f},

        {6.2f, 2.2f, 4.5f, 1.5f}, 
        {5.6f, 2.5f, 3.9f, 1.1f},
    /*********** R **************/
        {6.3f, 3.3f, 6.0f, 2.5f}, 
        {5.8f, 2.7f, 5.1f, 1.9f}, 
        {7.1f, 3.0f, 5.9f, 2.1f},

        {6.3f, 2.9f, 5.6f, 1.8f}, 
        {6.5f, 3.0f, 5.8f, 2.2f}, 
        {7.6f, 3.0f, 6.6f, 2.1f},

        {4.9f, 2.5f, 4.5f, 1.7f}, 
        {7.3f, 2.9f, 6.3f, 1.8f}, 
        {6.7f, 2.5f, 5.8f, 1.8f}, 

        {7.2f, 3.6f, 6.1f, 2.5f}, 
        {6.5f, 3.2f, 5.1f, 2.0f}, 
        {6.4f, 2.7f, 5.3f, 1.9f}, 

        {6.8f, 3.0f, 5.5f, 2.1f}, 
        {5.7f, 2.5f, 5.0f, 2.0f}, 
        {5.8f, 2.8f, 5.1f, 2.4f},

        {6.4f, 3.2f, 5.3f, 2.3f}, 
        {6.5f, 3.0f, 5.5f, 1.8f}, 
        {7.7f, 3.8f, 6.7f, 2.2f}, 
        
        {7.7f, 2.6f, 6.9f, 2.3f}, 
        {6.0f, 2.2f, 5.0f, 1.5f} 
    };
    // fucking data
    vfloat_t labelDatas[y_data_row][y_data_col] = {
        {1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},
        {1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},
        {0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},
        {0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},
        {0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},
        {0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.}
    };

    vfloat_t _sample [4] = {
        4.6f, 3.2f, 1.3f, 0.2f
    };

    matrix2_t* datas = Mat2_create(1,1);
    matrix2_t* labels = Mat2_create(1,1);
    matrix2_t* _Input = Mat2_create(1,1);
    matrix2_t* predict = Mat2_create(1,1);

    Mat2_load_on_shape(datas, trainingDatas, x_data_row, x_data_col);
    Mat2_load_on_shape(labels, labelDatas, y_data_row, y_data_col);
    Mat2_load_on_shape(_Input, _sample, 4, 1);

    Mat2_T(datas);
    Mat2_T(labels);
    //Mat2_T(_Input);

    srand(time(NULL));

    nn_t nn;
    int layers        = 1;
    int neruals[]     = {3, 5, 3};
    int input_dimens  = 4;
    int output_dimens = 3;
    int batch         = 10;
    int max_iter      = 10000;
    float alpha       = 0.1;
    float epsilon     = 0.01;

    nn_build(&nn, input_dimens, output_dimens, batch, max_iter, alpha, epsilon, layers, neruals, \
        sigmoid1, gradient_sigmoid1, softmax1, gradient_softmax1, crossentropy, gradient_corssentropy    \
    );

    nn_feed(&nn, datas, labels);
    // train the data
    nn_fit(&nn, &nn_progress);

    nn_perdict(&nn, _Input, predict);

    MAT2_INSPECT(predict);

    Mat2_destroy(datas);
    Mat2_destroy(labels);
    Mat2_destroy(_Input);
    Mat2_destroy(predict);

    nn_reset(&nn);

    return;
}

int do_reinforce_learning_test(void) 
{

    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("The Set Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    // if (NULL == CU_add_test(pSuite, "test hashset set", test_grid_world_save_load) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "world and policy load", test_policy_load) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "V(s) calculation", test_state_value_calculate) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "V(s) calculation 2", test_state_value_calcualte2) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "V(s) value iteration ", test_boe_value_iteration) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "V(s) policy iteration ", test_boe_policy_iteration) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "V(s) policy iteration exploring", test_boe_policy_iteration_base_mc_exploring) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "agent action probability", test_action_probability) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }


    // if (NULL == CU_add_test(pSuite, "epsilon exploring", test_boe_policy_iteration_epsilon_greedy_exploring) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "templor diffrence", test_temploral_difference_for_state_value) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "templor diffrence", test_temploral_difference_for_boe_sarsa) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "templor diffrence", test_Q_learning_online) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "templor diffrence", test_Q_learning_offline) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "function approximation td state value", test_function_approximation_of_td_state_value) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "functin approximation sarsa", test_function_approximation_of_sarsa) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    if (NULL == CU_add_test(pSuite, "test nn", test_nn) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    return 0;

}