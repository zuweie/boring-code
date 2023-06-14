/*
 * @Author: your name
 * @Date: 2021-10-21 15:16:31
 * @LastEditTime: 2023-06-14 13:11:11
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/Entity.h
 */
#ifndef __ENTITY_H__
#define __ENTITY_H__
#include <stdarg.h>
#include "base/type_value/__type_value_def.h"
#include "ty.h"

#define ROUND_UP_4(x) (((x) + 4 -1) & ~(4-1))

// ef => entity field 的缩写。这里设计 entity 的 body 中可以放 32 个单体数据。
// ef_(x) 只是读写第几个元素。

#define MAX_ENT_FIELD_NUM 32
#define ef_(x) ((unsigned long)1<<x)

#define ef_all (( (unsigned long) 1<<31) | ~(-( (unsigned long) 1<<31)))
#define ef_keys ((unsigned long)1<<33)
#define ef_values ((unsigned long)1<<34)

#define ef_key 0
#define ef_value 1
 
// 定义将第几个entity 的字段取出，并还原成相应的数据。
#define ef_pT(pe, i) ((pe)->block[i])
#define ef_char(pe, i) T_char(ef_pT(pe, i))
#define ef_uchar(pe, i) T_uchar(ef_pT(pe, i))
#define ef_int(pe, i) T_int(ef_pT(pe, i))
#define ef_uint(pe, i) T_uint(ef_pT(pe, i))
#define ef_long(pe, i) T_long(ef_pT(pe, i))
#define ef_ulong(pe, i) T_ulong(ef_pT(pe, i))
#define ef_float(pe, i) T_float(ef_pT(pe, i))
#define ef_double(pe, i) T_double(ef_pT(pe, i))
#define ef_ptr(pe, i) T_prt(ef_pT(pe, i))
#define ef_str(pe, i) T_str(ef_pT(pe, i))
#define ef_x(pe, i, type) T_type(ef_pT(pe, i), type)



typedef struct __entity_template {
    T_clazz type_class;
    int field_num;
    int value_idx;
    int* field_types;
} entity_template_t;

typedef struct __entity {
    entity_template_t* tpl;
    T** block;
} entity_t;

int cmp_entity(T*, T*);
int hash_entity(T*, int);
int setup_entity(T*, T*, unsigned char old_block);
int vargs_reader_entity(va_list, T*, int);

entity_t* entity_create(entity_template_t* etpl);
entity_t* entity_cpy(entity_t* src);
void entity_format_data_block(entity_t*, unsigned long);

// 用于计算包含 entity_template_t 实体 的 entity 的大小
int entity_tpl_cal_body_size(entity_template_t*, unsigned long);
// 用于格式化包含 entity_template_t 的 entity
void entity_tpl_format_body(T*, entity_template_t*, unsigned long);

// 用于计算包含 entity_template_t 指针的 entity 的大小
int entity_cal_body_size(entity_template_t*, unsigned long);
// 用于格式化包含 entity_template_t 指针的 entity 的大小
void entity_format_body(T*, entity_template_t*, unsigned long);

void entity_cpy_block_data(entity_t*, entity_t*, unsigned long);

int entity_read_from_vargs(entity_t*,  va_list, unsigned long);
int entity_write_to_vargs(entity_t*, va_list, unsigned long);



int entity_release(entity_t*);
#endif