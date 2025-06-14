/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:56:35
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-14 08:10:44
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_BASE_H__
#define __CG_BASE_H__


typedef struct cg_base {
    cg_graph_t      compute_graph;
    cg_list_t*      flow_stack;    
    int             gradient_version;
} cg_base_t;


int cg_base_init(cg_base_t*, cg_allocator_t* alloc);
int cg_base_reset(cg_base_t*);

cg_flow_elem_t* cg_flow_beg(cg_base_t* );
cg_flow_elem_t* cg_flow_end(cg_base_t* , cg_znode_base_t* );
cg_flow_elem_t* cg_flow_push(cg_base_t*, cg_flow_elem_t*);

int cg_do_forward(cg_base_t*, cg_znode_base_t* );
int cg_do_gradient(cg_base_t*, cg_znode_base_t*, cg_list_t*) 
int cg_update_version(cg_base_t* cg);
int cg_show_roadmap(cg_base_t*);

#endif