/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-08-23 13:39:18
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-10-09 15:51:32
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

    // e_null = 0,
    // e_normel,
    // e_forbitten,
    // e_target

static const float cell_reward_1[4]   = {-1., 0, -10., 1};
static const float cell_reward_ql[4]  = {-10., -1., -10., 0.,};
static int  suite_success_init (void) 
{
    printf("\n RL suite success init\n");
}


static int suite_success_clean (void) 
{   
    printf("\n RL suite success clean\n");
}

static float cell_reward_a(cell_clazz_t cell_type) {
    return cell_reward_1[cell_type];
}

static float cell_reward_b(cell_clazz_t cell_type) {
    return cell_reward_ql[cell_type];
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

    if (NULL == CU_add_test(pSuite, "templor diffrence", test_Q_learning_online) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    return 0;

}