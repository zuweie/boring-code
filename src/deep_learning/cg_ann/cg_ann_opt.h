#ifndef __CG_ANN_OPT_H__
#define __CG_ANN_OPT_H__


cg_opt_base_t* dot_opt (void*);
cg_opt_base_t* plus_opt (void*)
cg_opt_base_t* relu_opt (void*);
cg_opt_base_t* softmax_opt (void*);
cg_opt_base_t* cross_entroy_opt (void*);
cg_opt_base_t* mse_opt (void*);

#endif