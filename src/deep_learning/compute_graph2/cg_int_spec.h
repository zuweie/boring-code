#ifndef __CG_INT_SPEC_H__
#define __CG_INT_SPEC_H__

#define tensor_elem_size (sizeof(int))

#define elem_opt_add(dest_ptr, opt1_ptr, opt2_ptr) \
    do {\
        *((int*)(dest_ptr)) = *((int*)(opt1_ptr)) + *((int*)(opt2_ptr)); \
    } while(0)

#define elem_opt_substract(dest_ptr, opt1_ptr, opt2_ptr) \
    do {\
        *((int*)(dest_ptr)) = *((int*)(opt1_ptr)) - *((int*)(opt2_ptr)); \
    } while(0)

#define elem_opt_mulitply(dest_ptr, opt1_ptr, opt2_ptr) \
    do {\
        *((int*)(dest_ptr)) = *((int*)(opt1_ptr)) *  *((int*)(opt2_ptr)); \
    } while(0)

#define elem_opt_divide(dest_ptr, opt1_ptr, opt2_ptr) \
    do {\
        *((int*)(dest_ptr)) = *((int*)(opt1_ptr)) / *((int*)(opt2_ptr)); \
    } while(0)

#define elem_opt_to(dest_ptr, src_ptr) \
    do {\
        *((int*)(dest_ptr)) = *((int*)(src_ptr)); \
    } while(0)

#define elem_opt_assign(dest_ptr, numberic) \
    do {\
        *((int*)(dest_ptr)) = (int)(numberic); \
    } while(0)

#endif