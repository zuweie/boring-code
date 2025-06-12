/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:56:35
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-12 13:49:01
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_H__
#define __CG_H__


typedef struct cg {
    cg_graph_t      compute_graph;
    cg_list_t*      flow_stack;    
    int             data_version;
    int             znode_count;
    // 将所有创建的 znode 收集于此，最后集中销毁。
    cg_list_t*      znode_list;
    cg_allocator_t* znode_alloc;
} cg_t;


int cg_init(cg_t*, cg_allocator_t* alloc);
int cg_reset(cg_t*);

cg_flow_elem_t* cg_flow_push(cg_t*, cg_flow_elem_t*);
cg_flow_elem_t* cg_comb_zonde(cg_t*, znode_type_t znode_type);


int cg_forward_propagation(cg_t* cg);
int cg_backward_propagation(cg_t* cg);
int cg_update_data_version(cg_t* cg);
int cg_show_roadmap(cg_t*);

#endif