/*
 * @Author: your name
 * @Date: 2021-10-07 20:08:54
 * @LastEditTime: 2023-03-07 10:53:32
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/type_value/type_def.h
 */
#ifndef __TYPE_VALUE_DEF_H__
#define __TYPE_VALUE_DEF_H__
#include <stdarg.h>
#include "__type_value.h"


#define MAX_T_DEF_SLOT_SIZE 16
#define T_size(clazz_ptr) ((clazz_ptr)->_def.ty_size)
#define T_cmp(clazz_ptr) ((adapter_cmp)((clazz_ptr)->_adapter[e_cmp]))
#define T_hash(clazz_ptr) ((adapter_hasher)((clazz_ptr)->_adapter[e_hash]))
#define T_setup(clazz_ptr) ( (adapter_setup)((clazz_ptr)->_adapter[e_setup]))
#define T_vargs_read(clazz_ptr) ((adapter_vargs_reader) ((clazz_ptr)->_adapter[e_vargs]))

/**以上宏可以做出改进版本**/
/**
 * 没时间了，以后再弄吧
 * #define T_size(container_ptr) ((container_ptr)->type_clazz->_def.ty_size)
 * #define T_cmp(container_ptr, t1_ptr, t2_ptr) ((adapter_cmp) ((container_ptr)->type_clazz->_adapter[e_cmp]))(t1_ptr, t2_ptr)
 * #define T_hash(container_ptr, t1_ptr, int)  ((adapter_hasher) ((container_ptr)->type_clazz->adapter[e_hash]))(t1_ptr, int)
 * 
*/


typedef enum {
    char_t=1, 
    uchar_t, 
    int_t, 
    uint_t, 
    long_t, 
    ulong_t, 
    fl_t, 
    db_t, 
    str_t, 
    ptr_t
} built_in_type_t;

typedef enum {
    e_cmp=0,
    e_hash,
    e_setup,
    e_vargs,
} adapter_t;

typedef void (*T_adapter)(void);
typedef int (*adapter_cmp) (type_value_t*, type_value_t*);
typedef int (*adapter_hasher) (type_value_t*, int);
typedef int (*adapter_setup) (type_value_t*, type_value_t*, unsigned char);
typedef int (*adapter_vargs_reader)(va_list, type_value_t*, int);


typedef struct {
    int ty_id;
    int ty_size;
} T_def;

typedef struct {
    T_def _def;
    T_adapter _adapter[4];
} T_clazz;

int T_def_reg(    
    int T_size,
    adapter_cmp cmp,
    adapter_hasher hahser,
    adapter_setup setup, 
    adapter_vargs_reader vargs_reader
);

int T_def_unreg(int T_id);
int T_def_is_reg(int T_id);
T_def T_def_get(int T_id);
T_adapter T_adapter_get(int T_id, adapter_t adapter);
#endif