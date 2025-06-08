/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:56:35
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-08 14:13:52
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_H__
#define __CG_H__

typedef struct cg {
    cg_graph_t  compute_graph;
    cg_list_t*  compute_znodes;    
    int         data_version;
} cg_t;

int cg_init(cg_t*);
int cg_reset(cg_t*);

int cg_train(cg_t*);
int cg_predict(cg_t*);
int cg_save_model(cg_t*, const char* path);
int cg_load_model(cg_t*, const char* path);
int cg_show_roadmap(cg_t*);
#endif