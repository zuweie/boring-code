#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>

static int  suite_success_init (void) 
{
    printf("\ndeep learning suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\ndeep learning suite success clean\n");
}

int do_deep_learning_test (void) 
{

    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("The deep learning Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

}