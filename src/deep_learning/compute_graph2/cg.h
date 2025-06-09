/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:56:35
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-09 15:50:59
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_H__
#define __CG_H__


#define BEG_ELEM() cg_create_beg_flow_elem()
#define END_ELEM() cg_create_end_flow_elem()

typedef struct cg {
    cg_graph_t  compute_graph;
    cg_list_t*  flow_stack;    
    int         data_version;
    int         znode_count;
    cg_allocator_t* znode_alloc;
} cg_t;

static inline cg_flow_elem_t* cg_create_beg_flow_elem () 
{
    cg_flow_elem_t* elem = (cg_flow_elem_t*) malloc (sizeof(cg_flow_elem_t));
    elem->typ = beg;
    return elem;
}

static inline cg_flow_elem_t* cg_create_end_flow_elem()
{
    cg_flow_elem_t* elem = (cg_flow_elem_t*) malloc (sizeof (cg_flow_elem_t));
    elem->typ = end;
    return elem;
}

int cg_init(cg_t*);
int cg_reset(cg_t*);
int cg_flow_push(cg_t*, cg_flow_elem_t*);

int cg_train(cg_t*);
int cg_predict(cg_t*);
int cg_save_model(cg_t*, const char* path);
int cg_load_model(cg_t*, const char* path);
int cg_show_roadmap(cg_t*);
#endif