/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-11-26 08:17:48
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-11-27 13:31:09
 * @FilePath: /boring-code/src/reinforce_learning/nn2_znode.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include "matrix2/matrix2.h"
#include "nn2_znode.h"

static int __znode_recycle(znode2_t* znode) 
{
    Mat2_destroy(znode->x);
    Mat2_destroy(znode->Gx);
    return 0;
}

static int __linear_calculate(znode2_t* znode, matrix2_t* z)
{
    z->pool[0] = znode->x->pool[0] + 1.f;
    printf("%s, do linear calculate, x(%f) --> z(%f)\n", znode->name, znode->x->pool[0], z->pool[0]);

    return 0;
}

static int __linear_gradient(znode2_t* znode, matrix2_t* delta_z)
{
    znode->Gx->pool[0] = delta_z->pool[0] - 1.f;
    printf("%s, do linear gradient, delta_z(%f) --> Gx(%f)\n", znode->name, delta_z->pool[0], znode->Gx->pool[0]);
    return 0;
}

static int __relu_calculate(znode2_t* znode, matrix2_t* z)
{
    z->pool[0] = znode->x->pool[0] + 1.f;
    printf("%s, do relu calculate, x(%f) --> z(%f)\n", znode->name, znode->x->pool[0], z->pool[0]);
    return 0;
}

static int __relu_gradient(znode2_t* znode, matrix2_t* delta_z)
{
    znode->Gx->pool[0] = delta_z->pool[0] - 1.f;
    printf("%s, do relu gradient, delta_z(%f) --> Gx(%f)\n", znode->name, delta_z->pool[0], znode->Gx->pool[0]);
    return 0;
}

static int __mse_calculate(znode2_t* znode, matrix2_t* z)
{
    z->pool[0] = znode->x->pool[0] + 1.f;
    printf("%s, do mse calculate, x(%f) --> z(%f)\n", znode->name, znode->x->pool[0], z->pool[0]);
    return 0;

}

static int __mse_gradient(znode2_t* znode, matrix2_t* delta_z)
{
    znode->Gx->pool[0] = delta_z->pool[0] - 1.f;
    printf("%s, do mse gradient, delta_z(%f) --> Gx(%f)\n", znode->name, delta_z->pool[0], znode->Gx->pool[0]);
    return 0;
}

static int __crossentropy_calculate(znode2_t* znode, matrix2_t* z) 
{
    z->pool[0] = znode->x->pool[0] + 1.f;
    printf("%s, do crossentropy calculate, x(%f) --> z(%f)\n", znode->name, znode->x->pool[0], z->pool[0]);
    return 0;
}

static int __crossentropy_gradient(znode2_t* znode, matrix2_t* delta_z)
{
    znode->Gx->pool[0] = delta_z->pool[0] - 1.f;
    printf("%s, do crossentropy gradient, delta_z(%f) --> Gx(%f)\n", znode->name, delta_z->pool[0], znode->Gx->pool[0]);
    return 0;
}   

static int __softmax_calculate(znode2_t* znode, matrix2_t* z) 
{
    z->pool[0] = znode->x->pool[0] + 1.f;
    printf("%s, do softma calculate, x(%f) --> z(%f)\n", znode->name, znode->x->pool[0], z->pool[0]);
    return 0;
}

static int __softmax_gradient(znode_t* znode, matrix2_t* delta_z)
{
    znode->Gx->pool[0] = delta_z->pool[0] - 1.f;
    printf("%s, do softmax gradient, delta_z(%f) --> Gx(%f)\n", znode->name, delta_z->pool[0], znode->Gx->pool[0]);
    return 0;
}

static znode2_t* __create_znode(const char* name) 
{
    znode2_t* node = (znode2_t*) malloc (sizeof(znode2_t));
    *node = (znode2_t) {
        .calculate = __linear_calculate,
        .gradient  = __linear_gradient,
        .recycle   = __znode_recycle,
        .x         = Mat2_create(1,1),
        .Gx        = Mat2_create(1,1),
    };
    sprintf(node->name, "%s", name);
    Mat2_fill(node->x, 0.f);
    Mat2_fill(node->Gx, 0.f);
    return node;
}

znode2_t* create_linear(const char* name)
{
    return __create_znode(name);
}  

znode2_t* create_relu(const char* name)
{
    return __create_znode(name);
}

znode2_t* create_softmax(const char* name)
{
    return __create_znode(name);
}

znode2_t* create_mse(const char* name)
{
    return __create_znode(name);
}

znode2_t* create_crossentropy(const char* name)
{
    return __create_znode(name);
}