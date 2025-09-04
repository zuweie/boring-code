/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-08-23 13:39:18
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-09-04 10:20:28
 * @FilePath: /boring-code/src/unit_test/unit_test_reinforce_learning.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include <CUnit/Basic.h>
#include <limits.h>
#include "matrix2/matrix2.h"
#include "reinforce_learning/grid_world.h"
#include "reinforce_learning/agent.h"

static int  suite_success_init (void) 
{
    printf("\n RL suite success init\n");
}


static int suite_success_clean (void) 
{   
    printf("\n RL suite success clean\n");
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
    grid_world_load(load_path, &grid);
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
    agent_load(grid_path, policy_path, &agent);
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
    agent_load(grid_path, policy_path, &agent);
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
    agent_load(grid_path, policy_path, &agent);
    
    matrix2_t* Vs;
    matrix2_t* rewards;
    matrix2_t* transition;
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
    agent_load(grid_path, NULL, &agent);
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
    agent_load(grid_path, NULL, &agent);
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

    if (NULL == CU_add_test(pSuite, "V(s) policy iteration ", test_boe_policy_iteration) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    return 0;

}