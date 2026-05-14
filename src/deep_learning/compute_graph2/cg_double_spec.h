/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-05-14 23:13:41
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-05-14 23:15:19
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_double_spec.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%A
 */
#ifndef __CG_DOUBLE_SPEC_H__
#define __CG_DOUBLE_SPEC_H__

#define tensor_elem_size (sizeof(double))

#define elem_opt_add(dest_ptr, opt1_ptr, opt2_ptr) \
    do {\
        *((double*)(dest_ptr)) = *((double*)(opt1_ptr)) + *((double*)(opt2_ptr)); \
    } while(0)

#define elem_opt_substract(dest_ptr, opt1_ptr, opt2_ptr) \
    do {\
        *((double*)(dest_ptr)) = *((double*)(opt1_ptr)) - *((double*)(opt2_ptr)); \
    } while(0)

#define elem_opt_mulitply(dest_ptr, opt1_ptr, opt2_ptr) \
    do {\
        *((double*)(dest_ptr)) = *((double*)(opt1_ptr)) *  *((double*)(opt2_ptr)); \
    } while(0)

#define elem_opt_divide(dest_ptr, opt1_ptr, opt2_ptr) \
    do {\
        *((double*)(dest_ptr)) = *((double*)(opt1_ptr)) / *((double*)(opt2_ptr)); \
    } while(0)

#define elem_opt_to(dest_ptr, src_ptr) \
    do {\
        *((double*)(dest_ptr)) = *((double*)(src_ptr)); \
    } while(0)

#define elem_opt_assign(dest_ptr, numberic) \
    do {\
        *((double*)(dest_ptr)) = (double)(numberic); \
    } while(0)

#endif