/*
 * @Author: your name
 * @Date: 2020-10-13 06:51:56
 * @LastEditTime: 2021-11-02 10:19:15
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test.h
 */
#ifndef _UNIT_TEST_H_
#define _UNIT_TEST_H_
#include "container/cn.h"

#define PRINTF_TV_ON_X(tv, label, x, trans) printf( " %s: "#x"| ", label, trans(tv))

#define PRINTF_TV_ON_INT(tv)    printf("%d ", t2i(tv) )
#define PRINTF_TV_ON_FLOAT(tv)  printf("%f ", t2f(tv) )
#define PRINTF_TV_ON_CHAR(tv)   printf("%c ", t2i(tv) )
#define PRINTF_TV_ON_STRING(tv) printf("%s ", t2p(tv) )

#define PRINTF_IT_ON_INT(it) PRINTF_TV_ON_INT(It_dref(it))
#define PRINTF_IT_ON_CHAR(it) PRINTF_TV_ON_CHAR(It_dref(it))
#define PRINTF_IT_ON_FLOAT(it) PRINTF_TV_ON_FLOAT(It_dref(it))
#define PRINTF_IT_ON_STRING(it) PRINTF_TV_ON_STRING(It_dref(it))

#define PRINTF_IT_ON_VERTEX_CHAR(it) do{ \
    vertex_t* v = It_getptr(it); \
    PRINTF_TV_ON_CHAR(v->vertex_id);\
    printf(" -- %d -> ", ((bfs_explor_t*)v->exploring)->distance);\
}while(0) 

#define CN_inspect(con, type)  do{ \
    printf(" ********* inspection of container *****************\n"); \
    CN_travel(con, PRINTF_IT_ON_##type); \
    printf("\n"); \
}while(0)

#define Matrix_inspect(matrix) do{ \
    printf(" \n\n************* inspection of matrix ********************** \n\n");\
    float (*pdata)[(matrix)->col] = (matrix)->elems;\
    for (int i=0; i<(matrix)->row; ++i) { \
        if (i == 0) { \
            printf("   ");\
            for (int h=0; h<(matrix)->col; ++h) { \
                printf("%4d  ", h); \
            } \
            printf("\n"); \
        }\
        for (int j=0; j<(matrix)->col; ++j) { \
            if (j == 0) { \
                printf("%2d ", i); \
            } \
            printf("%.2f  ", Matrix_data(matrix)[i][j]);\
        } \
        printf("\n"); \
    } \
}while(0)


int do_vector_test (void);
int do_list_test(void);
int do_tv_test(void);
int do_hashmap_test(void);
int do_memory_pool_test(void);
int do_coo_matrix_test(void);
int do_dense_matrix_test(void);
int do_matrix_test(void);
int do_graph_test(void);
int do_udgraph_test (void);
int do_set_test(void);
int do_heapsort_test(void);
int do_fft_test(void);
int do_dct_test(void);
int do_asr_test(void);
int do_ultra_array_test(void);
int do_macine_learning_test(void);
int do_toutiao_test(void);
int do_svm_test(void);
#endif