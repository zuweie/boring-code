/*
 * @Author: your name
 * @Date: 2020-11-18 08:31:38
 * @LastEditTime: 2021-02-27 18:42:17
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_grap.c
 */
#include <stdio.h>
#include <stddef.h>
#include <CUnit/Basic.h>
#include "graph/graph_research.h"
#include "container/List.h"
#include "unit_test.h"
#include "test_data.h"
#include "cmp_component.h"

#define Graph_inspect(graph, printer, exploring_printer) do{ \
    printf(" \n\n********* inspection of Graph *****************\n\n"); \
    for (It i = CN_first( ((graph)->vertexes) ); !It_equal(i, CN_tail( ((graph)->vertexes) ) ); i = It_next(i)) { \
        vertex_t* pv = It_getptr(i); \
        printf("vertex: "); \
        printer(pv->vertex_id); \
        exploring_printer(pv->exploring); \
        printf("------> "); \
        for (It j = CN_first(pv->paths); !It_equal(j, CN_tail(pv->paths)); j = It_next(j)) { \
            path_t* pnode = It_getptr(j); \
            printf("{ ");\
            printer(pnode->to->vertex_id); \
            exploring_printer(pnode->to->exploring); \
            printf(" w:%f ", pnode->weight); \
            printf(" }"); \
        }\
        printf("\n\n"); \
    } \
}while(0)

#define BFS_exploring_printer(exploring, printer, printer_from) \ 
    ({  \
        if (exploring) { \
            bfs_explor_t* __p_marco_explor = (bfs_explor_t*) exploring; \
            printf("[ distance: %d ] ", __p_marco_explor->distance); \
            printf("[ pi_id: %d ] ", t2i(__p_marco_explor->pi->vertex_id)); \
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

#define PRIM_exploring_printer(exploring) \
    ({ \
        if (exploring) { \
            prim_explor_t* __p_marco_explor = (prim_explor_t*) exploring;   \
            if (__p_marco_explor->pi) { \
                printf("[ pi_id: %c, key:%f] ", t2i(__p_marco_explor->pi->vertex_id), __p_marco_explor->key); \
            } \
        } \
    })
#define RELAX_exploring_printer(exploring) \
    ({ \
        if (exploring) { \
            relax_explor_t* __p_marco_explor = (relax_explor_t*) exploring; \
            if (__p_marco_explor->pi) { \
                printf("[ pi_id: %c, distance: %f] ", t2i(__p_marco_explor->pi->vertex_id), __p_marco_explor->distance); \
            } \
        } \
    }) 

#define NULL_exploring_printer(exploring) \
    ({ \
        printf("[]");\
    })

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
    Graph* graph = Graph_create(graph_match_vertex, graph_match_path, 0);
    Graph_add_vertex(graph, getTSi(1));
    Graph_add_vertex(graph, getTSi(2));
    vertex_t* from = Graph_get_vertex(graph, getTSi(1));
    vertex_t* to   = Graph_get_vertex(graph, getTSi(2));

    Graph_add_path(from, to, 0.f);
    Graph_add_path(to, from, 0.f);


    Graph_inspect(graph, PRINTF_TV_ON_INT, NULL_exploring_printer);
    Graph_destroy(graph);
    CU_ASSERT_TRUE(1);
}

static void test_graph_matrix (void) 
{
    Graph* graph = Graph_create(graph_match_vertex, graph_match_path, 0);
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
    Graph_connect_vertexes(graph, matrix);
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

    Graph* graph = Graph_create(graph_match_vertex, graph_match_path, sizeof(dfs_explor_t));
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
    Graph_connect_vertexes(graph, matrix);
    //Graph_inspect(graph, PRINTF_TV_ON_CHAR, NULL_exploring_printer);
    grp_dfs_exploring(graph);
    grp_topological_sort(graph);
    Graph_inspect(graph, PRINTF_TV_ON_CHAR, DFS_exploring_printer);
    
    printf("\nreverse graph ...\n");
    Graph* reverse = Graph_create_reverse(graph);
    grp_dfs_exploring(reverse);
    Graph_inspect(reverse, PRINTF_TV_ON_CHAR, DFS_exploring_printer);

    List list = _List(CMP_PTR);   
    grp_calculate_component(reverse, list);
    
    for (It first = CN_first(list); !It_equal(first, CN_tail(list)); first = It_next(first)) {
        vertex_t* v = It_getptr(first);
        if (v) {
            PRINTF_TV_ON_CHAR(v->vertex_id);
        }else {
            printf(" 0 ");
        }
    }
    // clean up the malloc memory
    List_(list, NULL);

    CooMatrix_destroy(matrix);
    Graph_destroy(graph);
    Graph_destroy(reverse);
    CU_ASSERT_TRUE(1);
}

static void test_grap_strongly_connect(void) {

    Graph* graph = Graph_create(graph_match_vertex, graph_match_path, sizeof(dfs_explor_t));
    Graph_add_vertex(graph, i2t('a')); // 0
    Graph_add_vertex(graph, i2t('b')); // 1
    Graph_add_vertex(graph, i2t('c')); // 2
    CooMatrix* matrix = CooMatrix_create(CN_size(graph->vertexes), CN_size(graph->vertexes));
    Matrix_set(matrix, 0, 1, 1.0f);
    Matrix_set(matrix, 0, 2, 1.0f);
    Matrix_set(matrix, 2, 1, 1.0f);
    Graph_connect_vertexes(graph, matrix);

    Graph* strongly_connected = grp_calculate_strongly_connected_component_graph(graph);
    List list = _List(CMP_PTR);
    grp_calculate_component(strongly_connected, list);
    
    for (It first = CN_first(list); !It_equal(first, CN_tail(list)); first = It_next(first)) {
        vertex_t* v = It_getptr(first);
        if (v) {
            PRINTF_TV_ON_CHAR(v->vertex_id);
        }else {
            printf(" 0 ");
        }
    }

    List_(list, NULL);
    //grp_cleanup_exploring(graph);
    //grp_cleanup_exploring(strongly_connected);
    Graph_destroy(graph);
    Graph_destroy(strongly_connected);
    CooMatrix_destroy(matrix);
    CU_ASSERT_TRUE(1);
}

static void test_udgraph_mst_prim(void)
{
    Graph* graph = Graph_create(graph_match_vertex, graph_match_path, sizeof(prim_explor_t));

    Graph_add_vertex(graph, i2t('a')); // 0
    Graph_add_vertex(graph, i2t('b')); // 1
    Graph_add_vertex(graph, i2t('c')); // 2
    Graph_add_vertex(graph, i2t('d')); // 3
    Graph_add_vertex(graph, i2t('e')); // 4
    Graph_add_vertex(graph, i2t('f')); // 5
    Graph_add_vertex(graph, i2t('g')); // 6
    Graph_add_vertex(graph, i2t('h')); // 7
    Graph_add_vertex(graph, i2t('i')); // 8

    CooMatrix* matrix = CooMatrix_create(CN_size(graph->vertexes), CN_size(graph->vertexes));
    Matrix_set(matrix, 0, 1, 4); // ab
    Matrix_set(matrix, 1, 2, 8); // bc
    Matrix_set(matrix, 2, 3, 7); // cd
    Matrix_set(matrix, 3, 4, 9); // de
    Matrix_set(matrix, 4, 5, 10); // ef
    Matrix_set(matrix, 5, 6, 2); // fg
    Matrix_set(matrix, 6, 7, 1); // gh
    Matrix_set(matrix, 0, 7, 8); // ah
    Matrix_set(matrix, 1, 7, 11); // bh
    Matrix_set(matrix, 7, 8, 7); // hi
    Matrix_set(matrix, 6, 8, 6); // gi
    Matrix_set(matrix, 2, 8, 2); // ci
    Matrix_set(matrix, 2, 5, 4); // cf
    Matrix_set(matrix, 3, 5, 14); // df
    
    Graph_connect_vertexes(graph, matrix);

    Matrix_trans(matrix);
    Graph_connect_vertexes(graph, matrix);
    vertex_t* start = Graph_get_vertex(graph, i2t('a'));
    grp_calculate_mst_prim(graph, start);
    Graph_inspect(graph, PRINTF_TV_ON_CHAR, PRIM_exploring_printer);

    Graph_destroy(graph);
    CooMatrix_destroy(matrix);
}

static void 
test_graph_bellman_ford(void) 
{
    Graph* graph = Graph_create(graph_match_vertex, graph_match_path, sizeof(relax_explor_t));
    Graph_add_vertex(graph, i2t('s')); // 0
    Graph_add_vertex(graph, i2t('t')); // 1
    Graph_add_vertex(graph, i2t('x')); // 2
    Graph_add_vertex(graph, i2t('y')); // 3
    Graph_add_vertex(graph, i2t('z')); // 4

    CooMatrix* matrix = CooMatrix_create(CN_size(graph->vertexes), CN_size(graph->vertexes));
    Matrix_set(matrix, 0, 1, 6); // s t
    Matrix_set(matrix, 1, 2, 5); // t x
    Matrix_set(matrix, 2, 1, -2); // x t
    Matrix_set(matrix, 0, 3, 7); // s y
    Matrix_set(matrix, 1, 3, 8); // t y
    Matrix_set(matrix, 1, 4, -4);// t z
    Matrix_set(matrix, 3, 4, 9); // y z
    Matrix_set(matrix, 3, 2, -3);// y x
    Matrix_set(matrix, 4, 2, 7); // z x
    Matrix_set(matrix, 4, 0, 2); // z s

    Graph_connect_vertexes(graph, matrix);
    
    vertex_t* start = Graph_get_vertex(graph, i2t('s'));

    grp_calculate_bellman_ford(graph, start);

    Graph_inspect(graph, PRINTF_TV_ON_CHAR, RELAX_exploring_printer);

    Graph_destroy(graph);
    CooMatrix_destroy(matrix);
}

static void
test_graph_dijkstra(void) {

    Graph* graph = Graph_create(graph_match_vertex, graph_match_path, sizeof(relax_explor_t));
    Graph_add_vertex(graph, i2t('s')); // 0
    Graph_add_vertex(graph, i2t('t')); // 1
    Graph_add_vertex(graph, i2t('x')); // 2
    Graph_add_vertex(graph, i2t('y')); // 3
    Graph_add_vertex(graph, i2t('z')); // 4

    CooMatrix* matrix = CooMatrix_create(CN_size(graph->vertexes), CN_size(graph->vertexes));
    Matrix_set(matrix, 0, 1, 10); // s t
    Matrix_set(matrix, 0, 3, 5);  // s y
    Matrix_set(matrix, 1, 2, 1); // t x
    Matrix_set(matrix, 1, 3, 2); // t y
    Matrix_set(matrix, 2, 4, 4); // x z
    Matrix_set(matrix, 4, 2, 6); // z x
    Matrix_set(matrix, 4, 0, 7); // z s
    Matrix_set(matrix, 3, 1, 3); // y t
    Matrix_set(matrix, 3, 2, 9); // y x
    Matrix_set(matrix, 3, 4, 2); // y z

    Graph_connect_vertexes(graph, matrix);
    vertex_t* start = Graph_get_vertex(graph, i2t('s'));
    List list = _List(NULL);

    grp_calculate_dijkstra(graph, start, list);

    Graph_inspect(graph, PRINTF_TV_ON_CHAR, RELAX_exploring_printer);

    Graph_connect_vertexes(graph, matrix);
    CooMatrix_destroy(matrix);
    List_(list, NULL);
    Graph_destroy(graph);
}
static void
test_pi_offset(void) {
    ptrdiff_t offset = &(((relax_explor_t*)(0))->pi);
    printf("\nrelax pi offset: %ld ", offset);

    offset = &(((prim_explor_t*)0)->pi);
    printf("\nprim pi offset: %ld", offset);

    offset = &(((dfs_explor_t*)0)->pi);
    printf("\ndfs pi offset: %ld\n", offset);

    offset = &(((bfs_explor_t*)0)->pi);
    printf("\nbfs pi offset: %ld\n", offset);
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

    // if (NULL == CU_add_test(pSuite, "test graph dfs", test_graph_dfs) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test graph vertex edge", test_graph_vertex_edge) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test graph vertex matrix", test_graph_matrix) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    if (NULL == CU_add_test(pSuite, "test graph strongly connected", test_grap_strongly_connect) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }    


    if (NULL == CU_add_test(pSuite, "test mst prim", test_udgraph_mst_prim) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }


    if (NULL == CU_add_test(pSuite, "test bellman ford", test_graph_bellman_ford) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test graph dijkstra", test_graph_dijkstra) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test pi offset", test_pi_offset) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}