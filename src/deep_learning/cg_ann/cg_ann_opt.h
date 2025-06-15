/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-06-12 11:14:36
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-15 09:31:01
 * @FilePath: /boring-code/src/deep_learning/cg_ann/cg_ann_opt.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_ANN_OPT_H__
#define __CG_ANN_OPT_H__


cg_opt_base_t* dot_opt (void*);
cg_opt_base_t* plus_opt (void*);
cg_opt_base_t* relu_opt (void*);
cg_opt_base_t* softmax_opt (void*);
cg_opt_base_t* cross_entroy_opt (void*);
cg_opt_base_t* mse_opt (void*);

#endif