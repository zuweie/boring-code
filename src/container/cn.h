/*
 * @Author: your name
 * @Date: 2021-10-21 11:59:07
 * @LastEditTime: 2021-11-09 10:41:58
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/Cn.h
 */
#ifndef __CN_H__
#define __CN_H__
#include "base/type_value/__built_in_type_adapters.h"
#include "base/__container.h"
#include "it.h"
#include "ty.h"

#define CAPACITY_NUMBER 1024

#define CN_DEFINE_LOCAL_ENTITY(cn, ent_name, accessor) \
    entity_template_t* marco_##ent_name##_tpl = CN_ty_clazz(cn); \
    int marco_##ent_name##_local_entity_body_size = entity_cal_body_size(marco_##ent_name##_tpl, accessor); \
    T marco_##ent_name##_ent_body[marco_##ent_name##_local_entity_body_size]; \
    entity_format_body(marco_##ent_name##_ent_body, marco_##ent_name##_tpl, accessor); \
    entity_t* ent_name = marco_##ent_name##_ent_body

#define CN_READ_ENTITY_VARGS(cn, ent_name, valist, accessor, context) \
    CN_DEFINE_LOCAL_ENTITY(cn, ent_name, accessor); \
    T_vargs_read(CN_ty_clazz(cn))(valist, ent_name, context)

#define CN_READ_SINGLE_VALUE_VARGS(cn, t_name, valist, context) \
    T t_name[T_size(CN_ty_clazz(cn))]; \
    T_vargs_read(CN_ty_clazz(cn))(valist, t_name, context)

enum {
    // base container type 
    VECTOR = (unsigned long) 1 << 27,
    LIST   = (unsigned long) 1 << 26,
    TREE_SET = (unsigned long) 1 << 25,
    HASH_SET = (unsigned long) 1 << 24,

    // build options
    multi_key = (unsigned long) 1 << 15,
    use_entity = (unsigned long) 1 << 14,
    customized_entity = (unsigned long) 1 << 13,
    customized_compare = (unsigned long) 1 << 12,
    customized_hasher = (unsigned long) 1 << 11,
    customized_setup = (unsigned long) 1 << 10,
    customized_vargs_reader = (unsigned long) 1 << 9,
};  

enum {
    // combine container
    TREE_MAP = TREE_SET | use_entity,
    HASH_MAP = HASH_SET | use_entity,
    MULTI_TREE_SET = TREE_SET | multi_key,
    MULTI_TREE_MAP = TREE_SET | use_entity | multi_key,
    MULTI_HASH_SET = HASH_SET | multi_key,
    MULTI_HASH_MAP = HASH_SET | use_entity | multi_key,
};


// error code
enum {
    err_ok = 0,
    err_no_found = -1,
    err_empty = -2,
    err_out_of_capcity = -10,
    err_unsupported_type = -11,
    err_unsupported_eng = -12,
    err_out_of_max_ent_field = -13,
    err_invalid_ent_keys_number = -14,
    err_unsupported_method = -15,
    err_invalid_pos = -16,
    err_merge = -17
};

enum {
    using_at_add = 1,
    using_at_find,
};
typedef struct __It It;
typedef int CN;
typedef struct __cn cn_t;

struct __cn {
    container_t* eng;
    unsigned long build_code;
    T_clazz* type_clazz;
    unsigned char is_forward;
};

CN CN_create(unsigned long build_code, ...);
CN CN_finalize(CN cn, int(*cleanup)(T*));
T_clazz* CN_ty_clazz(CN);

It CN_head(CN cn);
It CN_tail(CN cn);
It CN_first(CN cn);
It CN_last(CN cn);
  
int CN_reverse(CN cn);
int CN_extract(CN, T*, int (*cmp)(T*, T*));
int CN_sort(CN, int (*cmp)(T*, T*));
int CN_size(CN);
int CN_has(CN, ...);
int CN_to_unique(CN);
int CN_merge(CN, CN);

int CN_add(CN, ...);
int CN_add_at(CN, It, ...);

int CN_remove(CN, T*);
int CN_remove_at(CN, It, T*);

It CN_find(CN, ...);
It CN_find_at(CN, It, ...);

int CN_del(CN, ...);
int CN_set(CN, ...);
T* CN_get(CN, ...);



#endif