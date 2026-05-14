/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-05-01 15:35:16
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-05-14 23:18:43
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_elem_spec.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_ELEM_SPEC_H__
#define __CG_ELEM_SPEC_H__

#define CG_ELEM_TYPE_FLOAT  1
#define CG_ELEM_TYPE_DOUBLE 2
#define CG_ELEM_TYPE_INT    3

#ifndef CG_ELEM_TYPE 
#define CG_ELEM_TYPE CG_ELEM_TYPE_DOUBLE
#endif

#if CG_ELEM_TYPE == CG_ELEM_TYPE_FLOAT
    #include "cg_float_spec.h"
#elif CG_ELEM_TYPE == CG_ELEM_TYPE_DOUBLE
    #include "cg_double_spec.h"
#elif CG_ELEM_TYPE == CG_ELEM_TYPE_INT
    #include "cg_int_spec.h"
#else
    #error "unknown elem type define"
#endif

#endif