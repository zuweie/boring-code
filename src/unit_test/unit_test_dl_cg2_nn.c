#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "unit_test.h"

#include "deep_learning/nn/nn.h"
#include "deep_learning/nn_operand/nn_operand.h"
#include "deep_learning/nn_operator/linear_opt.h"


static int  suite_success_init (void) 
{
    printf("\ndl_cg2_nn suite success init\n");
    return 0;
}

static int suite_success_clean (void) 
{   
    printf("\ndl_cg2_nn suite success clean\n");
    return 0;
}

static void test_nn_build (void) 
{
    printf(" \n build a simple neural network with computer graph 2\n");
    
    nn_t simple_nn;
    nn_init(&simple_nn);
    
    

    return 0;
}



int do_dl_cg2_nn_test(void)
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("The dl cg2 nn Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }


    return 0;
}