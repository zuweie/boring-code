/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-05-14 22:56:54
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-06-28 14:04:24
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_tensor_elem_int_spec.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_TENSOR_ELEM_INT_SPEC_H__
#define __CG_TENSOR_ELEM_INT_SPEC_H__

typedef int cg_tensor_elem_type;

#define cg_tensor_elem_size (sizeof(int))

#define cg_tensor_elem_ref_opt(dest_ptr, opt1_ptr, opt2_ptr, opt, assign) \
        *((int*)(dest_ptr)) assign *((int*)(opt1_ptr)) opt *((int*)(opt2_ptr))

#define cg_tensor_elem_number_opt(dest_ptr, number1, number2, opt, assign) \
        *((int*)(dest_ptr)) assign (number1) opt (number2)

#define cg_tensor_elem_rn_opt(dest_ptr, opt_ptr, number, opt, assign) \
        *((int*)(dest_ptr)) assign *((int*)(opt_ptr)) opt (number)

#define  cg_tensor_elem_ref_assign(dest_ptr, opt_ptr) \
        *((int*)(dest_ptr)) = *((int*)(opt_ptr))

#define cg_tensor_elem_number_assign(dest_ptr, number) \
        *((int*)(dest_ptr)) = ((int)(number))

#define cg_tensor_elem_display(elem_ptr, precision) \
        printf("%" precision "d, ", *((int*)(elem_ptr)))

#endif