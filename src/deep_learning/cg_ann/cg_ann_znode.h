/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-06-13 13:30:57
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-15 16:06:03
 * @FilePath: /boring-code/src/deep_learning/cg_ann/cg_ann_znode.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_ANN_ZNODE_H__
#define __CG_ANN_ZNODE_H__
#include "deep_learning/compute_graph2/cg_znode_base.h"
typedef struct cg_ann cg_ann_t;

typedef enum {
    e_x = 1,
    e_y,
    e_y_hat,
    e_weight,
    e_bais,
    e_loss,
    e_auto,
} ann_znode_type_t;

typedef struct ann_znode {
    cg_znode_base_t znode_base;
    cg_tensor_t* payload;
    cg_tensor_t* gradient;
    ann_znode_type_t znode_type;
    cg_ann_t*        ann;
} ann_znode_t;

ann_znode_t* cg_ann_znode_create(cg_ann_t* ann, int r, int c, ann_znode_type_t type);
int cg_ann_znode_recycle(ann_znode_t* znode);
#endif