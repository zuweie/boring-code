/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:56:43
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-08 14:10:38
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE{}
 */
#include "cg_graph.h"
#include "cg_list.h"
#include "cg_znode.h"
#include "cg.h"

static int __recycle_zonde(cg_ref_t ref) 
{
    cg_znode_t* znode = (cg_znode_t*) ref;
    cg_znode_recycle(znode);
    return 0;
}

int cg_init(cg_t* cg)
{
    cg->compute_graph  = cg_graph_init(&cg->compute_graph);
    cg->compute_znodes = cg_list_create();
    cg->data_version  = 0;
    return 0;
}

int cg_reset(cg_t* cg)
{
    cg_graph_reset(&cg->compute_graph);
    cg_list_recycle(cg->compute_znodes, __recycle_zonde);
    cg->data_version = 0;
    return 0;
}



int cg_train(cg_t*);
int cg_predict(cg_t*);
int cg_save_model(cg_t*, const char* path);
int cg_load_model(cg_t*, const char* path);
int cg_show_roadmap(cg_t*);