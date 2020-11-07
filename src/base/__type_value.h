/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-03 13:29:13
 * @LastEditTime: 2020-11-08 01:35:56
 * @LastEditors: Please set LastEditors
 */

#ifndef _TYPE_VALUE_H_
#define _TYPE_VALUE_H_

#if __x86_64__
   typedef unsigned long v_type;
   typedef double floating_type;
#else
   typedef int v_type;
   typedef float floating_type;
#endif


#define vtype_int(t) ((v_type)t.pointer)
#define vtype_float(t) ((floating_type)t.floating)
#define vtype_pointer(t) (t.pointer)

#define int_vtype(v) \
   ({ \
      type_value_t t; \
      t.pointer = (void*)v; \
      t; \
   })

#define float_vtype(v) \
   ({ \
      type_value_t t; \
      t.floating = v; \
      t; \
   })

#define pointer_vtype(v) int_vtype(v)

#define vtype_equl(t1, t2) ((((v_type)t1.pointer)^((v_type)t2.pointer))?1:0)
#define vtype_cmp_on_int(t1, t2) ((vtype_equl(t1, t2))?(((vtype_int(t1)>vtype_int(t2)))?1:-1):0)
#define vtype_cmp_on_flt(t1, t2) ((vtype_equl(t1, t2))?((vtype_float(t1)>vtype_float(t2))?1:-1):0)
#define vtype_cmp_on_ptr(t1, t2) (!(vtype_pointer(t1)==vtype_pointer(t2)))

#define bad_vtype int_vtype(~((v_type)0))


typedef union _type_value{
   void* pointer;
   floating_type floating;
} type_value_t;
#endif