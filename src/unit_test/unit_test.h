/*
 * @Author: your name
 * @Date: 2020-10-13 06:51:56
 * @LastEditTime: 2020-10-15 09:50:17
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test.h
 */
#ifndef _UNIT_TEST_H_
#define _UNIT_TEST_H_
#include "container/Cn.h"

#define PRINTF_TV_ON_INT(tv) printf("%d ", t2i(tv))
#define PRINTF_TV_ON_FLOAT(tv) printf("%f ", t2f(tv))
#define PRINTF_TV_ON_CHAR(tv) printf("%c ", t2i(tv))
#define PRINTF_TV_ON_STRING(tv) printf("%s ", t2p(tv))

#define PRINTF_IT_ON_INT(it) PRINTF_TV_ON_CHAR(It_dref(it))
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

#define Graph_inspect(graph, printer) do{ \
    printf(" ********* inspection of Graph *****************\n"); \
    for (It i = CN_first( ((graph)->vertexes) ); !It_equal(i, CN_tail( ((graph)->vertexes) ) ); i = It_next(i)) { \
        vertex_t* pv = It_getptr(i); \
        printf("vertex: "); \
        printer(pv->vertex_id); \
        printf("(%d) ", pv->indexing); \
        printf("------> "); \
        for (It j = CN_first(pv->edges); !It_equal(j, CN_tail(pv->edges)); j = It_next(j)) { \
            edge_t* pnode = It_getptr(j); \
            printer(pnode->to->vertex_id); \
            printf("(%d)   ", pnode->to->indexing);\
        }\
        printf("\n\n"); \
    } \
}while(0)

#define Matrix_inspect(matrix) do{ \
    printf(" ************* inspection of matrix ********************** \n");\
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
int do_tv_test(void);
int do_hashmap_test(void);
#endif