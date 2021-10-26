/*
 * @Author: your name
 * @Date: 2021-10-21 11:59:07
 * @LastEditTime: 2021-10-25 23:53:56
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/Cn.h
 */
#ifndef __CN_H__
#define __CN_H__
#include "base/type_value/__built_in_type.h"
#include "base/__container.h"
#include "it.h"
#include "ty.h"

#define CAPACITY_NUMBER 1024

#define CN_DEFINE_LOCAL_INDEPENDENT_ENTITY(cn, ent_name, accessor) \
    entity_template_t* marco_##ent_name##_tpl = CN_type_info(cn); \
    int marco_##ent_name##_local_entity_body_size = entity_tpl_cal_independent_entity_body_size(marco_##ent_name##_tpl, accessor); \
    T marco_##ent_name##_ent_body[marco_##ent_name##_local_entity_body_size]; \
    entity_format_independent_entity_body(marco_##ent_name##_ent_body, marco_##ent_name##_tpl, accessor); \
    entity_t* ent_name = marco_##ent_name##_ent_body

#define CN_READ_ENTITY_VARGS(cn, ent_name, valist, accessor) \
    CN_DEFINE_LOCAL_INDEPENDENT_ENTITY(cn, ent_name, accessor); \
    entity_read_from_vargs(ent_name,  valist, accessor)

#define CN_READ_SINGLE_VALUE_VARGS(cn, t_name, valist) \
    T_def* marco_##t_name##_def = CN_type_def(cn); \
    T t_name[marco_##t_name##_def->ty_size]; \
    marco_##t_name##_def->ty_adapter.read_vargs(valist, t_name)

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
    err_invalid_pos = -16
};

typedef int CN;
typedef struct __cn cn_t;

struct __cn {
    container_t* eng;
    unsigned long build_code;
    void* type_info;
    unsigned char is_forward;
};

CN CN_create(unsigned long build_code, ...);
CN CN_finalize(CN cn, int(*cleanup)(T*));

It CN_head(CN cn);
It CN_tail(CN cn);
It CN_first(CN cn);
It CN_last(CN cn);
  
int CN_reverse(CN cn);

int CN_add(CN, ...);
int CN_add_at(CN, It, ...);

int CN_remove(CN, T*);
int CN_remove_at(CN, It, T*);

It CN_find(CN, ...);
It CN_find_at(CN, It, ...);

int CN_del(CN, ...);
int CN_set(CN, ...);
T* CN_get(CN, ...);

void* CN_type_info(CN);
T_def* CN_type_def(CN);

#endif