#include <stdio.h>
#include <stdlib.h>
#include "nn2_znode.h"
#include "nn2.h"
static int __endpoint_calculate(znode2_t* node, matrix2_t* z)
{
    printf("%s, do nothing calculating\n", node->name);
    return 0;
}

static int __endpoint_gradient(znode2_t* node, matrix2_t* delta_z)
{
    printf("%s, do nothing gradienting\n", node->name);
    return 0;
}

static int __do_backward(nn2_t* nn2, znode2_t* node, matrix2_t* delta_z)
{
    if (node != layer_tail(nn2)) {
        __do_backward(nn2, node->next, node->gradient);
    }
    node->gradient(node, delta_z);
    return 0;
}   

static int __do_forward(nn2_t* nn2, znode2_t* node, matrix2_t* z)
{   
    if (node != layer_head(nn2)) {
        __do_forward(nn2, node->prev, node->x);
    }
    node->calculate(node, z);
    return 0;
}

int nn2_init(nn2_t* nn2)
{
    nn2->znode_counter   = 0;
    nn2->znode_head      = (znode2_t) {
        .x         = (matrix2_t*) NULL,
        .Gx        = (matrix2_t*) NULL,
        .calculate = __endpoint_calculate,
        .gradient  = __endpoint_gradient,
        .next      = layer_tail(nn2),
        .prev      = layer_head(nn2)
    };
    sprintf(nn2->znode_head.name, "<znode head>");
    return 0;
}

int nn2_reset(nn2_t* nn2)
{
    znode2_t* first = layer_first(nn2);
    znode2_t* del;
    while (first != layer_tail(nn2)) {
        del = first;
        layer_remove(del);
        del->recycle(del);
        free(del);
        first = first->next;
    }
    return 0;
}
int nn2_linear(nn2_t* nn2, int in_dimens, int out_dimens)
{   
    char name[128];
    sprintf(name, "<linear %d>", ++nn2->znode_counter);
    znode2_t* linear = create_linear(name);
    layer_insert(layer_tail(nn2), linear)
    return 0;
}
int nn2_relu(nn2_t* nn2)
{
    char name[128];
    sprintf(name, "<relu %d>", ++nn2->znode_counter);
    znode2_t* relu = create_relu(name);
    layer_insert(layer_tail(nn2), relu);
    return 0;
}
int nn2_softmax(nn2_t* nn2)
{
    char name[128];
    sprintf(name, "<softmax %d>", ++nn2->znode_counter);
    znode2_t* softmax = create_softmax(name);
    layer_insert(layer_tail(nn2), softmax);
    return 0;
}

int nn2_mse(nn2_t* nn2)
{
    char name[128];
    sprintf(name, "<mse %d>", ++nn2->znode_counter);
    znode2_t* mse = create_mse(name);
    layer_insert(layer_tail(nn2), mse);
    return 0;
}

int nn2_crossentropy(nn2_t* nn2)
{
    char name[128];
    sprintf(name, "<crossentropy %d>", ++nn2->znode_counter);
    znode2_t* crossentropy = create_crossentropy(name);
    layer_insert(layer_tail(nn2), crossentropy);
    return 0;
}

int nn2_forward(nn2_t* nn2, matrix2_t* z)
{
    znode2_t* last = layer_last(nn2);
    __do_forward(nn2, last, z);
    return 0;
}

int nn2_backward(nn2_t* nn2, matrix2_t* delta_z)
{
    znode2_t* first = layer_first(nn2);
    __do_forward(nn2, first, delta_z);
    return 0;
}

int nn2_fit(nn2_t*)
{
    return 0;
}

