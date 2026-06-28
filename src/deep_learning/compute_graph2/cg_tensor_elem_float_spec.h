/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-05-14 22:36:22
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-06-28 14:03:05
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_tensor_elem_float_spec.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_TENSOR_ELEM_FLOAT_SPEC_H__
#define __CG_TENSOR_ELEM_FLOAT_SPEC_H__

typedef float cg_tensor_elem_type;

#define cg_tensor_elem_size (sizeof(float))

#define cg_tensor_elem_ref_opt(dest_ptr, opt1_ptr, opt2_ptr, opt, assign) \
        *((float*)(dest_ptr)) assign *((float*)(opt1_ptr)) opt *((float*)(opt2_ptr))

#define cg_tensor_elem_number_opt(dest_ptr, number1, number2, opt, assign) \
        *((float*)(dest_ptr)) assign ((float)(number1)) opt ((float)(number2))

#define cg_tensor_elem_rn_opt(dest_ptr, opt_ptr, number, opt, assign) \
        *((float*)(dest_ptr)) assign *((float*)(opt_ptr)) opt ((float)(number))

#define  cg_tensor_elem_ref_assign(dest_ptr, opt_ptr) \
        *((float*)(dest_ptr)) = *((float*)(opt_ptr))

#define cg_tensor_elem_number_assign(dest_ptr, number) \
        *((float*)(dest_ptr)) = ((float)(number))

#define cg_tensor_elem_display(elem_ptr, precision) \
        printf("%" precision "f, ", *((float*)(elem_ptr)))

#endif