#ifndef __CG_FLOW_H__
#define __CG_FLOW_H__

typedef enum {

    beg = 1,
    end,
    znode,
    opt,

} flow_elem_typ_t;

typedef union cg_flow_elem {
    flow_elem_typ_t typ;
} cg_flow_elem_t;

#endif