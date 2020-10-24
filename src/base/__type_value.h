/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-03 13:29:13
 * @LastEditTime: 2020-10-24 10:16:11
 * @LastEditors: Please set LastEditors
 */

#ifndef _TYPE_VALUE_H_
#define _TYPE_VALUE_H_

#if __x86_64__
   typedef long v_type;
   typedef double floating_type;
#else
   typedef int v_type;
   typedef float floating_type;
#endif

//#define set_type_val(t, v) (((*((v_type*)t.type_value))=(*((v_type*)(&(v)))))?t:t)
//#define set_type_ptr(t, p) ((t.pointer=p)?t:t)
#define set_type_val(t, v) ((*((v_type*)t.type_value))=(*((v_type*)(&(v)))))
#define set_type_ptr(t, p) (t.pointer=p)
#define vtype_xtype(vt,t) (*((vt*)t.type_value))
#define vtype_int(t) vtype_xtype(v_type, t)
#define vtype_float(t) vtype_xtype(floating_type, t)
#define vtype_pointer(t) (t.pointer)

#define xtype_vtype(x_type, v)  \
   ({                           \
      type_value_t tv;          \
      x_type value = v;         \
      set_type_val(tv, value);  \
      tv;                       \
   })
#define int_vtype(v) xtype_vtype(v_type, v)
#define float_vtype(v) xtype_vtype(floating_type, v)
#define pointer_vtype(v)       \
   ({                          \
      type_value_t tv;         \
      void* p = v;             \
      set_type_ptr(tv, p);     \
      tv;                      \
   }) 
#define vtype_equl(t1, t2) (((*((v_type*)t1.type_value))^(*((v_type*)t2.type_value)))?1:0)
#define vtype_cmp_on_int(t1, t2) ((vtype_equl(t1, t2))?(((vtype_int(t1)>vtype_int(t2)))?1:-1):0)
#define vtype_cmp_on_flt(t1, t2) ((vtype_equl(t1, t2))?((vtype_float(t1)>vtype_float(t2))?1:-1):0)
#define vtype_cmp_on_ptr(t1, t2) (!(vtype_pointer(t1)==vtype_pointer(t2)))

#define bad_vtype int_vtype(~((v_type)0))

typedef union _type_value{
   void* pointer;
   unsigned char type_value[sizeof(v_type)];
} type_value_t;

#endif