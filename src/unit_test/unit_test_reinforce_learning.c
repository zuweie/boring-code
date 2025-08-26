/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-08-23 13:39:18
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-08-24 17:01:21
 * @FilePath: /boring-code/src/unit_test/unit_test_reinforce_learning.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include <CUnit/Basic.h>

#include "reinforce_learning/grid_world.h"

static int  suite_success_init (void) 
{
    printf("\n RL suite success init\n");
}


static int suite_success_clean (void) 
{   
    printf("\n RL suite success clean\n");
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
    
}

int do_reinforce_learning_test(void) 
{

    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("The Set Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test hashset set", test_grid_world_save_load) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    return 0;

}