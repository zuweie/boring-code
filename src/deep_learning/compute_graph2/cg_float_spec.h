#ifndef __CG_FLOAT_SPEC_H__
#define __CG_FLOAT_SPEC_H__

#define tensor_elem_size (sizeof(float))

#define elem_opt_add(dest_ptr, opt1_ptr, opt2_ptr) \
    do {\
        *((float*)(dest_ptr)) = *((float*)(opt1_ptr)) + *((float*)(opt2_ptr)); \
    } while(0)

#define elem_opt_substract(dest_ptr, opt1_ptr, opt2_ptr) \
    do {\
        *((float*)(dest_ptr)) = *((float*)(opt1_ptr)) - *((float*)(opt2_ptr)); \
    } while(0)

#define elem_opt_mulitply(dest_ptr, opt1_ptr, opt2_ptr) \
    do {\
        *((float*)(dest_ptr)) = *((float*)(opt1_ptr)) *  *((float*)(opt2_ptr)); \
    } while(0)

#define elem_opt_divide(dest_ptr, opt1_ptr, opt2_ptr) \
    do {\
        *((float*)(dest_ptr)) = *((float*)(opt1_ptr)) / *((float*)(opt2_ptr)); \
    } while(0)

#define elem_opt_to(dest_ptr, src_ptr) \
    do {\
        *((float*)(dest_ptr)) = *((float*)(src_ptr)); \
    } while(0)

#define elem_opt_assign(dest_ptr, numberic) \
    do {\
        *((float*)(dest_ptr)) = (float)(numberic); \
    } while(0)

#endif