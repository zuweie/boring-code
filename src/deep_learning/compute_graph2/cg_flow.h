#ifndef __CG_FLOW_H__
#define __CG_FLOW_H__

typedef enum {

    e_beg = 1,
    e_end,
    e_znode,
    e_opt,

} flow_elem_type_t;

typedef union cg_flow_elem {
    flow_elem_type_t elem_type;
} cg_flow_elem_t;

#endif