#ifndef __CG_NODE_H__
#define __CG_NODE_H__

typedef void* cg_ref_t;
typedef struct _cg_node cg_node_t;

struct _cg_node {
    cg_ref_t ref;
    cg_node_t* prev;
};

#endif