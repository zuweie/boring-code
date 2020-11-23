/*
 * @Author: your name
 * @Date: 2020-11-18 08:31:38
 * @LastEditTime: 2020-11-23 15:42:12
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_grap.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "graph/graph_research.h"
#include "unit_test.h"
#include "test_data.h"

#define Graph_inspect(graph, printer, exploring_printer) do{ \
    printf(" \n\n********* inspection of Graph *****************\n\n"); \
    for (It i = CN_first( ((graph)->vertexes) ); !It_equal(i, CN_tail( ((graph)->vertexes) ) ); i = It_next(i)) { \
        vertex_t* pv = It_getptr(i); \
        printf("vertex: "); \
        printer(pv->vertex_id); \
        exploring_printer(pv->exploring); \
        printf("------> "); \
        for (It j = CN_first(pv->edges); !It_equal(j, CN_tail(pv->edges)); j = It_next(j)) { \
            edge_t* pnode = It_getptr(j); \
            printer(pnode->to->vertex_id); \
        }\
        printf("\n\n"); \
    } \
}while(0)

#define BFS_exploring_printer(exploring) \ 
    ({  \
        if (exploring) { \
            bfs_explor_t* __p_marco_explor = (bfs_explor_t*) exploring; \
            printf("[ distance: %d ] ", __p_marco_explor->distance); \
            printf("[ pi_id: %d ] ", t2d(__p_marco_explor->pi->vertex_id)); \
        } \
    })  
    
#define DFS_exploring_printer(exploring) \
    ({ \
        if (exploring) { \
            dfs_explor_t* __p_marco_explor = (dfs_explor_t*) exploring; \
            printf("[ d time %d ]", __p_marco_explor->d_time); \
            printf("[ f time %d ]", __p_marco_explor->f_time); \
        } \
    })

#define NULL_exploring_printer(exploring) \
    ({ \
        printf("[]");\
    })

int find_vertex(Tv v1, Tv v2) 
{
    vertex_t* pv = t2p(v1);
    return Tv_equl(pv->vertex_id, v2);
}

int find_edge(Tv v1, Tv v2) 
{
    edge_t* pedge = t2p(v1);
    return Tv_equl(pedge->to->vertex_id, v2);
}

static int  suite_success_init (void) 
{
    printf("\nGraph research suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nGraph research suite success clean\n");
}

static void test_graph_vertex_edge (void)
{
    Graph* graph = Graph_create(find_vertex, find_edge);
    Graph_add_vertex(graph, getTSi(1));
    Graph_add_vertex(graph, getTSi(2));
    vertex_t* from = Graph_get_vertex(graph, getTSi(1));
    vertex_t* to   = Graph_get_vertex(graph, getTSi(2));

    Graph_add_edge(from, to, 0.f);
    Graph_add_edge(to, from, 0.f);


    Graph_inspect(graph, PRINTF_TV_ON_INT, NULL_exploring_printer);
    Graph_destroy(graph);
    CU_ASSERT_TRUE(1);
}

static void test_graph_matrix (void) 
{
    Graph* graph = Graph_create(find_vertex, find_edge);
    // Graph_add_vertex(graph, getTSi(1));
    // Graph_add_vertex(graph, getTSi(2));
    // Graph_add_vertex(graph, getTSi(3));
    // Graph_add_vertex(graph, getTSi(4));
    // Graph_add_vertex(graph, getTSi(5));
    // Graph_add_vertex(graph, getTSi(6));
    // Graph_add_vertex(graph, getTSi(7));
    // Graph_add_vertex(graph, getTSi(8));
    // Graph_add_vertex(graph, getTSi(9));
    // Graph_add_vertex(graph, getTSi(10));
    // Graph_add_vertex(graph, getTSi(11));
    // Graph_add_vertex(graph, getTSi(12));
    // Graph_add_vertex(graph, getTSi(13));
    // Graph_add_vertex(graph, getTSi(14));   
    //Graph_inspect(graph, PRINTF_TV_ON_INT, NULL_exploring_printer);

    Graph_add_vertex(graph, i2t(1));
    Graph_add_vertex(graph, i2t(2));
    Graph_add_vertex(graph, i2t(3));
    Graph_add_vertex(graph, i2t(4));
    Graph_add_vertex(graph, i2t(5));
    Graph_add_vertex(graph, i2t(6));
    Graph_add_vertex(graph, i2t(7));
    Graph_add_vertex(graph, i2t(8));
    Graph_add_vertex(graph, i2t(9));
    Graph_add_vertex(graph, i2t(10));
    Graph_add_vertex(graph, i2t(11));
    Graph_add_vertex(graph, i2t(12));
    Graph_add_vertex(graph, i2t(13));
    Graph_add_vertex(graph, i2t(14)); 
    CooMatrix* matrix = CooMatrix_create(CN_size(graph->vertexes), CN_size(graph->vertexes));

    Matrix_set(matrix, 1, 2, 1.0f);
    Matrix_set(matrix, 2, 3, 1.0f);
    Matrix_set(matrix, 3, 4, 1.0f);
    Matrix_set(matrix, 2, 6, 1.0f);
    Matrix_set(matrix, 7, 6, 1.0f);
    Matrix_set(matrix, 3, 9, 1.0f);
    Matrix_set(matrix, 0, 4, 1.0f);
    Matrix_set(matrix, 0, 5, 1.0f);
    Matrix_set(matrix, 9, 0, 1.0f);
    Matrix_set(matrix, 13, 2, 1.0f);
    Graph_add_edge_by_matrix(graph, matrix);
    Graph_inspect(graph, PRINTF_TV_ON_INT, NULL_exploring_printer);
    
    printf("Create reverse Graph \n\n");

    Graph* reverse = Graph_create_reverse(graph);
    
    Graph_inspect(reverse, PRINTF_TV_ON_INT, NULL_exploring_printer);
    
    CooMatrix_destroy(matrix);
    Graph_destroy(graph);
    Graph_destroy(reverse);
    CU_ASSERT_TRUE(1);
}

static void test_graph_bfs (void)
{

}

static void test_graph_dfs (void) 
{

    Graph* graph = Graph_create(find_vertex, find_edge);
    Graph_add_vertex(graph, i2t('a')); // 0
    Graph_add_vertex(graph, i2t('b')); // 1
    Graph_add_vertex(graph, i2t('c')); // 2
    Graph_add_vertex(graph, i2t('d')); // 3
    Graph_add_vertex(graph, i2t('e')); // 4
    Graph_add_vertex(graph, i2t('f')); // 5
    Graph_add_vertex(graph, i2t('g')); // 6
    Graph_add_vertex(graph, i2t('h')); // 7
    CooMatrix* matrix = CooMatrix_create(CN_size(graph->vertexes), CN_size(graph->vertexes));

    Matrix_set(matrix, 0, 1, 1.0f); // a b
    Matrix_set(matrix, 1, 2, 1.0f); // b c
    Matrix_set(matrix, 2, 3, 1.0f); // c d
    Matrix_set(matrix, 3, 2, 1.0f); // d c
    Matrix_set(matrix, 3, 7, 1.0f); // d h
    Matrix_set(matrix, 4, 0, 1.0f); // e a
    Matrix_set(matrix, 1, 4, 1.0f); // b e
    Matrix_set(matrix, 4, 5, 1.0f); // e f
    Matrix_set(matrix, 1, 5, 1.0f); // b f
    Matrix_set(matrix, 5, 6, 1.0f); // f g
    Matrix_set(matrix, 6, 5, 1.0f); // g f
    Matrix_set(matrix, 2, 6, 1.0f); // c g
    Matrix_set(matrix, 6, 7, 1.0f); // g h
    Matrix_set(matrix, 7, 7, 1.0f); // h h
    Graph_add_edge_by_matrix(graph, matrix);
    //Graph_inspect(graph, PRINTF_TV_ON_CHAR, NULL_exploring_printer);
    grp_dfs_exploring(graph);
    grp_topological_sort(graph);
    Graph_inspect(graph, PRINTF_TV_ON_CHAR, DFS_exploring_printer);
    
    printf("\nreverse graph ...\n");
    Graph* reverse = Graph_create_reverse(graph);
    grp_dfs_exploring(reverse);
    Graph_inspect(reverse, PRINTF_TV_ON_CHAR, DFS_exploring_printer);
    

    // clean up the malloc memory
    grp_cleanup_exploring(graph);
    grp_cleanup_exploring(reverse);

    CooMatrix_destroy(matrix);
    Graph_destroy(graph);
    Graph_destroy(reverse);
    CU_ASSERT_TRUE(1);
}

int do_graph_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test Graph Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    // if (NULL == CU_add_test(pSuite, "test graph bfs", test_graph_bfs) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    if (NULL == CU_add_test(pSuite, "test graph dfs", test_graph_dfs) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // if (NULL == CU_add_test(pSuite, "test graph vertex edge", test_graph_vertex_edge) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test graph vertex matrix", test_graph_matrix) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }
}