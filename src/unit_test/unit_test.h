/*
 * @Author: your name
 * @Date: 2020-10-13 06:51:56
 * @LastEditTime: 2025-05-31 23:13:00
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test.h
 */
#ifndef _UNIT_TEST_H_
#define _UNIT_TEST_H_
#include "container/cn.h"

#define PRINTF_TV_ON_X(tv, label, x, trans) printf( " %s: "#x"| ", label, trans(tv))

#define PRINTF_T_ON_INT(tv)    printf("%d ", T_int(tv) )
#define PRINTF_T_ON_FLOAT(tv)  printf("%f ", T_float(tv) )
#define PRINTF_T_ON_DOUBLE(tv) printf("%f ", T_double(tv) )
#define PRINTF_T_ON_CHAR(tv)   printf("%c ", T_char(tv) ) 
#define PRINTF_T_ON_STRING(tv) printf("%s ", T_str(tv) )

#define PRINTF_IT_ON_INT(it) PRINTF_T_ON_INT(It_refer(it))
#define PRINTF_IT_ON_CHAR(it) PRINTF_T_ON_FLOAT(It_refer(it))
#define PRINTF_IT_ON_FLOAT(it) PRINTF_T_ON_CHAR(It_refer(it))
#define PRINTF_IT_ON_DOUBLE(it) PRINTF_T_ON_DOUBLE(It_refer(it))
#define PRINTF_IT_ON_STRING(it) PRINTF_T_ON_STRING(It_refer(it))

#define PRINTF_IT_ON_VERTEX_CHAR(it) do{ \
    vertex_t* v = It_getptr(it); \
    PRINTF_TV_ON_CHAR(v->vertex_id);\
    printf(" -- %d -> ", ((bfs_explor_t*)v->exploring)->distance);\
}while(0) 

#define CN_inspect(con, T_printer)  do{ \
    printf("\n ********* inspection of container *****************\n"); \
    for(It first=CN_first(con); !It_equal(first, CN_tail(con)); It_next(first)) {\
        T_printer(first._iter.reference);\
    } \
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
int do_t_test(void);
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
int do_neural_test(void);
int do_leetcode_test(void);
int do_matrix2_test(void);
int do_statistical_learning_test(void);
int do_deep_learning_test(void);
int do_cg2_test(void);
#endif