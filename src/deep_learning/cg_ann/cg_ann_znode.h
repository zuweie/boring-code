#ifndef __CG_ANN_ZNODE_H__
#define __CG_ANN_ZNODE_H__
#include "deep_learning/compute_graph2/cg_znode_base.h"

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
    cg_znode_base_t base;
    cg_tensor_t* payload;
    cg_tensor_t* gradient;
    ann_znode_type_t znode_type;
} ann_znode_t;

ann_znode_t* cg_ann_znode_create(cg_ann_t* ann, int r, int c, ann_znode_type_t type);
int cg_ann_znode_recycle(ann_znode_t* znode);
#endif