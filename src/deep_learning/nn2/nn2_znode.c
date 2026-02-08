/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-11-26 08:17:48
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-02-08 20:42:14
 * @FilePath: /boring-code/src/reinforce_learning/nn2_znode.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "matrix2/matrix2.h"
#include "nn2_znode.h"

static int __base_znode_recycle(nn2_znode_base_t* thiz)
{
    return 0;
}

static int __linear_calculate(nn2_znode_base_t* thiz, matrix2_t* out, void*)
{
    return 0;
}

static int __linear_gradient(nn2_znode_base_t* thiz, matrix2_t* in, void*)
{   
    return 0;
}

static int __linear_recycle(nn2_znode_base_t* thiz)
{
    return 0;
}

static int __relu_calculate(nn2_znode_base_t* thiz, matrix2_t* out, void*)
{
    return 0;
}

static int __relu_gradient(nn2_znode_base_t* thiz, matrix2_t* in, void*)
{
    return 0;
}

static int __softmax_calculate(nn2_znode_base_t* thiz, matrix2_t* out, void*)
{
    return 0;
}

static int __softmax_gradient(nn2_znode_base_t* thiz, matrix2_t* in, void*) 
{
    return 0;
}

static int __mse_loss_calculate(nn2_znode_base_t* thiz, matrix2_t* out, void*)
{
    return 0;
}

static int __mse_loss_gradient(nn2_znode_base_t* thiz, matrix2_t* in, void*) 
{
    return 0;
}


static int __crossentropy_loss_calculate(nn2_znode_base_t* thiz, matrix2_t* out, void*)
{
    return 0;
}

static int __crossentropy_loss_gradient(nn2_znode_base_t* thiz, matrix2_t* in, void*)
{
    return 0;
}



nn2_znode_base_t* create_linear(int in_dimens, int out_dimens, const char* name)
{
    nn2_znode_base_t* znode = (nn2_znode_base_t*) malloc (sizeof (nn2_linear_t));
    // init
    *((nn2_linear_t*)znode) = (nn2_linear_t) {
        ._base = (nn2_znode_base_t) {
            .x         = Mat2_create(in_dimens, 1),
            .Gx        = Mat2_create(in_dimens, 1),
            .calculate = __linear_calculate,
            .gradient  = __linear_gradient,
            .recycle   = __linear_recycle,
            .name      = "lienar"
        },
        .W     = Mat2_create(out_dimens, in_dimens),
        .GW    = Mat2_create(out_dimens, in_dimens),
        .b     = Mat2_create(out_dimens, 1),
        .Gb    = Mat2_create(out_dimens, 1),
    };
    // copy the znode name
    return znode;
}

nn2_znode_base_t* create_relu()
{   
    nn2_znode_base_t* znode = (nn2_znode_base_t*) malloc (sizeof (nn2_relu_t));
    *((nn2_relu_t*) znode)  = (nn2_relu_t){
        ._base = (nn2_znode_base_t) {
            .x  = Mat2_create(1,1),
            .Gx = Mat2_create(1,1),
            .calculate = __relu_calculate,
            .gradient  = __relu_gradient,
            .recycle   = __base_znode_recycle,
            .name      = "relu"
        }
    };
    return znode;
}
nn2_znode_base_t* create_softmax()
{
    nn2_znode_base_t* znode   = (nn2_znode_base_t*) malloc (sizeof (nn2_softmax_t));
    *((nn2_softmax_t*) znode) = (nn2_softmax_t) {
        ._base = (nn2_znode_base_t) {
            .x  = Mat2_create(1,1),
            .Gx = Mat2_create(1,1),
            .calculate = __softmax_calculate,
            .gradient  = __softmax_gradient,
            .recycle   = __base_znode_recycle,
            .name      = "softmax"
        }
    };
    return znode;
}
nn2_znode_base_t* create_mse_loss()
{
    nn2_znode_base_t* znode    = (nn2_znode_base_t*) malloc (sizeof (nn2_mse_loss_t));
    *((nn2_mse_loss_t*) znode) = (nn2_mse_loss_t) {
        ._base = (nn2_znode_base_t) {
            .x  = Mat2_create(1,1),
            .Gx = Mat2_create(1,1),
            .calculate = __mse_loss_calculate,
            .gradient  = __mse_loss_gradient,
            .recycle   = __base_znode_recycle,
            .name      = "mse loss"
        }
    };
    return znode;
}

nn2_znode_base_t* create_crossentropy_loss()
{
    nn2_znode_base_t* znode             = (nn2_znode_base_t*) malloc (sizeof (nn2_crossentropy_loss_t));
    *((nn2_crossentropy_loss_t*) znode) = (nn2_crossentropy_loss_t) {
        ._base = (nn2_znode_base_t) {
            .x  = Mat2_create(1,1),
            .Gx = Mat2_create(1,1),
            .calculate = __crossentropy_loss_calculate,
            .gradient  = __crossentropy_loss_gradient,
            .recycle   = __base_znode_recycle,
            .name      = "crossentropy loss"
        }
    };

    return znode;
}