#include <stdlib.h>
#include <string.h>
#include "cg_opt_dot.h"
#include "../cg_debug.h"

static int __fp(cg_tensor_t* r, cg_tensor_t*, t1, cg_tensor_t* t2, void* param)
{
    CG_DEBUG("dot operation for fp\n");
    cg_tensor_dot(r, t1, t2);
}

static int __bp(cg_tensor_t* r, cg_tensor_t* t1, cg_tensor_t* t2, void* param)
{

}


cg_opt_base_t* cg_opt_dot()
[
    cg_opt_dot_t* dot = (cg_opt_dot_t*) malloc (sizeof(cg_opt_dot_t));
    
    return dot;
]
