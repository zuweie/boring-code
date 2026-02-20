#ifndef __CG_NODE_H__
#define __CG_NODE_H__

#include <string.h>
#include "cg_list.h"
#include "cg_vertex.h"

typedef enum cg_node_type {
    e_operand = 1,
    e_operator,
} cg_node_type_t;

typedef struct cg_node {
    cg_vertex_t    vertex;
    cg_node_type_t node_type;
} cg_node_t;

static inline int cg_node_init(cg_node_t* thiz, const char* id, cg_node_type_t node_type) 
{
    thiz->node_type = node_type;
    strcpy(thiz->vertex.id, id);

    thiz->vertex.in      = cg_list_create();
    thiz->vertex.out     = cg_list_create();

    return 0;
}

static inline int cg_node_reset(cg_node_t* thiz) 
{
    cg_list_recycle(thiz->vertex.in, NULL);
    cg_list_recycle(thiz->vertex.out, NULL);
}

#endif