#ifndef __CG_OPT_H__
#define __CG_OPT_H__

typedef struct cg_opt_base {
    char id[32];
    int (*fp)(cg_opt_base_t*);
    int (*bp)(cg_opt_base_t*);
} cg_opt_base_t;


#endif