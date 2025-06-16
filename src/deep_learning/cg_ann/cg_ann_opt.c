/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-06-12 11:14:43
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-16 17:01:42
 * @FilePath: /boring-code/src/deep_learning/cg_ann/cg_ann_opt.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include "base/container_of.h"
#include "deep_learning/compute_graph2/cg_graph.h"
#include "deep_learning/compute_graph2/cg_tensor.h"
#include "deep_learning/compute_graph2/cg_flow.h"
#include "deep_learning/compute_graph2/cg_opt_base.h"
#include "deep_learning/compute_graph2/cg_znode_base.h"
#include "deep_learning/compute_graph2/cg_debug.h"
#include "cg_ann_znode.h"
#include "cg_ann_opt.h"

static int __dot_fp(cg_znode_base_t* znode, void* param)
{
    // 是在是太累了，现在暂时要休息一下，不能再继续做这个框架啦。所以现现在展示使用 printf 内容输出，替代真正的代码运行。
    #if 0
    // 检查该节点是否只有两个 in 节点，多于两个为错误
    ann_znode_t* znode = (ann_znode_t*) z;
    cg_ann_t*    ann   = znode->ann;
    
    if (cg_list_size(znode->znode_base.vertex.in_vertexes) != 2) {
        CG_DEBUG("Error:znode(%s) has no euqal 2 in come znode!\n", znode->znode_base.vertex.id);
        return -1;
    }

    if (znode->payload == NULL) {
        CG_DEBUG("Error: znode(%s)`s payload is NULL!\n", znode->znode_base.vertex.id);
        return -1;
    }

    cg_vertex_t* v_left  = CG_LIST_TOP(znode->znode_base.vertex.in_vertexes)->ref;
    cg_vertex_t* v_right = CG_LIST_TOP(znode->znode_base.vertex.in_vertexes)->prev->ref;

    ann_znode_t* left_znode  = (ann_znode_t*)container_of(v_left, cg_znode_base_t, vertex);
    ann_znode_t* right_znode = (ann_znode_t*)container_of(v_right, cg_znode_base_t, vertex);
    
    
    int dot_ret = 0;
    if (znode->znode_type == e_x) {
        
        if (TENSOR_DIMEN(right_znode->payload, 1) != TENSOR_DIMEN(left_znode->payload, 1)) {
            CG_DEBUG("Error: Left Tensor (%s) row (%d) is no euq Right Tensor (%s) col (%d) \n", \
                right_znode->znode_base.vertex.id, TENSOR_DIMEN(right_znode->payload, 1),     \
                left_znode->znode_base.vertex.id, ENSOR_DIMEN(left_znode->payload, 1)         \
            );

            return -1;
        }

        cg_tensor_t* x = cg_tensor_create(ann->alloc, 2, ann->x_dimens, 1);
        
        cg_tensor_sub_to_tensor(x, cg_tensor_get_sub(left_znode->payload, 1, ann->step));
        cg_tensor_T(x, 1, 0);
        dot_ret = cg_tensor_dot(znode->payload, right_znode->payload, x);
        cg_tensor_recycle(x);
        
    } else {

        if (TENSOR_DIMEN(right_znode->payload, 1) != TENSOR_DIMEN(left_znode->payload, 0)) {
            CG_DEBUG("Error: Left Tensor (%s) row (%d) is no euq Right (%s) Tensor col (%d) \n", \
                right_znode->znode_base.vertex.id, TENSOR_DIMEN(right_znode->payload, 1),     \
                left_znode->znode_base.vertex.id, TENSOR_DIMEN(left_znode->payload, 0)        \
            );

            return -1;
        }

        if (znode->payload == NULL) {
            // znode 的 payload 矩阵可能是 NULL;
            znode->payload = cg_tensor_create(ann->alloc, 2, 1,1);
        }

        dot_ret =  cg_tensor_dot(znode->payload, right_znode->payload, left_znode->payload);
    }

    if (dot_ret != 0) {
        CG_DEBUG("Error: %s dot %s error\n", right_znode->znode_base.vertex.id, left_znode->znode_base.vertex.id);
    }

    return dot_ret;
    #endif

    CG_DEBUG("MSG: Doing dot fp on %s\n", znode->vertex.id);

    cg_node_t* first    = CG_LIST_TOP(znode->vertex.in_vertexes);
    cg_vertex_t* vertex = first->ref;
    ann_znode_t* left   = (ann_znode_t*) container_of(vertex, cg_znode_base_t, vertex);
    vertex              = first->prev->ref;
    ann_znode_t* right  = (ann_znode_t*) container_of(vertex, cg_znode_base_t, vertex);;

    CG_DEBUG("MSG: %s DOT %s to %s \n", left->znode_base.vertex.id, right->znode_base.vertex.id, znode->vertex.id);
    return 0;

}

static int __dot_bp(cg_znode_base_t* superior, cg_znode_base_t* znode, void* param) 
{
    #if 0
    // 这个关于 dot_bp 的梯度是将兄弟节点的 payload 拿过来然后转置，然后与 superior 的做内积
    ann_znode_t* superior_znode = (ann_znode_t*) superior;
    ann_znode_t* thiz_znode     = (ann_znode_t*) znode;

    if (cg_list_size(superior->vertex.in_vertexes) != 2){
        CG_DEBUG("Error: %s has %d znode!\n", superior->vertex.id, cg_list_size(superior->vertex.in_vertexes));
        return -1;
    }
    cg_vertex_t* borther_vertex = NULL;
    cg_node_t* first = CG_LIST_HEAD(superior->vertex.in_vertexes);
    while (first != CG_LIST_TOP(superior->vertex.in_vertexes)) {

        if (first->ref != &znode->vertex) 
        {
            borther_vertex = first->ref;
            break;
        }

        first = first->prev;
    }
    if (borther_vertex == NULL) {
        CG_DEBUG("Error: %s has no brother znode\n", znode->vertex.id);
        return -1;
    }
    ann_znode_t* brother_znode = (ann_znode_t*) container_of(borther_vertex, cg_znode_base_t, vertex);
    cg_tensor_t* gradient = cg_tensor_create(&superior_znode->ann.alloc, 2 , 1, 1,);
    cg_tensor_t* brother_gradient_cpy = cg_tensor_create_cpy(brother_znode->gradient);
    cg_tenaor_T(brother_gradient_cpy, 1, 0);

    cg_tensor_dot(gradient, superior_znode->gradient, brother_gradient_cpy);
    cg_tensor_sum(thiz_znode->gradient, gradient);

    return 0;
    #endif

    CG_DEBUG("MSG: Doing dot bp on %s and %s\n", superior->vertex.id, znode->vertex.id);
    cg_vertex_t* vertex_brother;
    cg_node_t* first = CG_LIST_TOP(superior->vertex.in_vertexes);
    if (first->ref != &znode->vertex) {
        vertex_brother = first->ref;
    } else {
        vertex_brother = first->prev->ref;
    }
    cg_znode_base_t* brother_znode = container_of(vertex_brother, cg_znode_base_t, vertex);
    CG_DEBUG("MSG: @sperior(%s) / @znode(%s) is brother(%s)` payload^T, and Dot superior(%s)`s Gridient\n", \
        superior->vertex.id, znode->vertex.id, brother_znode->vertex.id, superior->vertex.id                \ 
    );

    CG_DEBUG("MSG: @J/@znode(%s)`s gradient plus to zonde(%s)`s gradient\n", znode->vertex.id, znode->vertex.id);

    return 0;
}


static int __plus_fp(cg_znode_base_t* znode, void* param)
{
    CG_DEBUG("MSG: Doing plus fp on %s\n", znode->vertex.id);
    cg_znode_base_t* sub_znode;
    cg_vertex_t* vertex;
    cg_node_t* first = CG_LIST_TOP(znode->vertex.in_vertexes);
    while (first != CG_LIST_HEAD(znode->vertex.in_vertexes)) {

        vertex = first->ref;
        sub_znode = container_of(vertex, cg_znode_base_t, vertex);
        CG_DEBUG("MSG: Plus %s`s payload\n", sub_znode->vertex.id);
        first = first->prev;
    }
    return 0;
}

static int __plus_bp(cg_znode_base_t* superior, cg_znode_base_t* znode, void* param) 
{
    CG_DEBUG("MSG: Doing plus bp for @%s/@%s\n", superior->vertex.id, znode->vertex.id);
    CG_DEBUG("MSG: @%s/@%s is 1, dot %s`s gradient\n", superior->vertex.id, znode->vertex.id, superior->vertex.id);
    return 0;
}

static int __relu_fp(cg_znode_base_t* znode, void* param)
{
    CG_DEBUG("MSG: Doing relu fp on %s \n", znode->vertex.id);
    return 0;
}

static int __relu_bp(cg_znode_base_t* superior, cg_znode_base_t* znode, void* param)
{
    CG_DEBUG("MSG: Doing relu bp @%s/@%s \n", znode->vertex.id);
    CG_DEBUG("MSG: @%s/@%s dot %s`s gradient\n", superior->vertex.id, znode->vertex.id, superior->vertex.id);
    return 0;
}

static int __softmax_fp(cg_znode_base_t* znode, void* param)
{
    CG_DEBUG("MSG: Doing softmax fp on %s \n", znode->vertex.id);
    return 0;
}

static int __softmax_bp(cg_znode_base_t* superior, cg_znode_base_t* znode, void* param)
{
    CG_DEBUG("MSG: Doing softmax bp @%s/@%s \n", superior->vertex.id, znode->vertex.id);
    CG_DEBUG("MSG: @%s/@%s dot %s`s gradient\n", superior->vertex.id, znode->vertex.id, superior->vertex.id);
    return 0;
}

static int __cross_fp(cg_znode_base_t* znode, void* param)
{
    CG_DEBUG("MSG: Doing cross fp on %s \n", znode->vertex.id);
    return 0;
}

static int __cross_bp(cg_znode_base_t* superior, cg_znode_base_t* znode, void* param)
{
    CG_DEBUG("MSG: Doing cross bp @%s/@%s \n", superior->vertex.id, znode->vertex.id);
    CG_DEBUG("MSG: @%s/@%s dot %s`s gradient\n", superior->vertex.id, znode->vertex.id, superior->vertex.id);
    return 0;
}

static int __empty_fp(cg_znode_base_t* znode, void* param) 
{
    CG_DEBUG("MSG: empty fp on %s \n", znode->vertex.id);
    return 0;
}

static int __empty_bp(cg_znode_base_t* superior, cg_znode_base_t* znode, void* param) 
{
    CG_DEBUG("MSG: empty bp @%s/@%s \n", superior->vertex.id, znode->vertex.id);
    return 0;
}

cg_opt_base_t* dot_opt (void* param)
{
    cg_opt_base_t* opt = malloc( sizeof(cg_opt_base_t) );
    *opt = (cg_opt_base_t){.flow_elem = e_opt, .fp=__dot_fp, .bp=__dot_bp, .recycle=NULL};
    return opt;
}

cg_opt_base_t* plus_opt (void* param)
{
    cg_opt_base_t* opt = malloc( sizeof(cg_opt_base_t) );
    *opt = (cg_opt_base_t){.flow_elem=e_opt, .fp=__plus_fp, .bp=__plus_bp, .recycle=NULL};
    return opt;
}


cg_opt_base_t* relu_opt (void* param)
{
    cg_opt_base_t* opt = malloc( sizeof(cg_opt_base_t) );
    *opt = (cg_opt_base_t){.flow_elem=e_opt, .fp=__relu_fp, .bp=__relu_bp, .recycle=NULL};
    return opt;
}


cg_opt_base_t* softmax_opt (void* param)
{
    cg_opt_base_t* opt = malloc( sizeof(cg_opt_base_t) );
    *opt = (cg_opt_base_t){.flow_elem=e_opt, .fp=__softmax_fp, .bp=__softmax_bp, .recycle=NULL};
    return opt;
}

cg_opt_base_t* cross_entroy_opt (void* param)
{
    cg_opt_base_t* opt = malloc( sizeof(cg_opt_base_t) );
    *opt = (cg_opt_base_t){.flow_elem=e_opt, .fp=__cross_fp, .bp=__cross_bp, .recycle=NULL};
    return opt;
}

cg_opt_base_t* mse_opt (void* param)
{
    cg_opt_base_t* opt = malloc( sizeof(cg_opt_base_t) );
    *opt = (cg_opt_base_t){.flow_elem=e_opt, .fp=__cross_fp, .bp=__cross_bp, .recycle=NULL};
    return opt;
}

cg_opt_base_t* empty_opt (void* param) 
{
    cg_opt_base_t* opt = malloc( sizeof(cg_opt_base_t) );
    *opt = (cg_opt_base_t){.flow_elem=e_opt, .fp=__empty_fp, .bp=__empty_bp, .recycle=NULL};
    return opt;
}
