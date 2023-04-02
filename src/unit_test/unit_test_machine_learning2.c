#include <stdio.h>
#include <CUnit/Basic.h>

#define PRINTF_DOUBLES(x) printf("%lf ", (x));

static int  suite_success_init (void) 
{
    printf("\n machine learning2 suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\n machine learning 2 suite success clean\n");
}


int do_machine_learning2_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test machine learning 2 Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

}