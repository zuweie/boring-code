/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-06-14 08:11:05
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-18 11:38:26
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_znode_base.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <string.h>
#include "cg_ref.h"
#include "cg_list.h"
#include "cg_graph.h"
#include "cg_opt_base.h"
#include "cg_znode_base.h"
#include "cg_base.h"

static int __recycle_path(cg_ref_t ref) 
{
    cg_list_t* path = (cg_list_t*) ref;
    cg_list_recycle(path, NULL);
    return 0;
}

// 基类的构造函数
int cg_znode_base_init(cg_base_t* cg, cg_znode_base_t* znode, const char* id)
{
    strcpy(znode->vertex.id, id);
    znode->flow_elem.elem_type = e_znode;
    znode->opt                 = NULL;
    znode->vertex.in_vertexes  = NULL;
    znode->vertex.out_vertexes = NULL;
    cg_graph_add_vertex(&cg->compute_graph, &znode->vertex);
}

// 基类的析构函数
int cg_znode_base_reset(cg_znode_base_t* znode)
{
    // 释放 paths
    if (znode->gradient_paths) {
        cg_list_recycle(znode->gradient_paths, __recycle_path);
        znode->gradient_paths = NULL;
    }
    
    while (znode->opt)
    {   
        cg_opt_base_t* opt = znode->opt;
        znode->opt = znode->opt->next;
        
        if (opt->recycle)
            opt->recycle(opt);
        free(opt);
    }
    znode->opt = NULL;
    return 0;
}