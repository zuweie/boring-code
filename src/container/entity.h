/*
 * @Author: your name
 * @Date: 2021-10-21 15:16:31
 * @LastEditTime: 2021-10-24 22:51:21
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/Entity.h
 */
#ifndef __ENTITY_H__
#define __ENTITY_H__
#include <stdarg.h>
#include "ty.h"

#define ROUND_UP_4(x) (((x) + 4 -1) & ~(4-1))
#define MAX_ENT_FIELD_NUM 32
#define ef_(x) ((unsigned long)1<<x)
#define ef_0 ((unsigned long)1)
#define ef_1 ef_(1)
#define ef_2 ef_(2)
#define ef_3 ef_(3)
#define ef_4 ef_(4)
#define ef_5 ef_(5)
#define ef_6 ef_(6)
#define ef_7 ef_(7)
#define ef_8 ef_(8)
#define ef_9 ef_(9)
#define ef_10 ef_(10)
#define ef_11 ef_(11)
#define ef_12 ef_(12)
#define ef_13 ef_(13)
#define ef_14 ef_(14)
#define ef_15 ef_(15)
#define ef_16 ef_(16)
#define ef_17 ef_(17)
#define ef_18 ef_(18)
#define ef_19 ef_(19)
#define ef_20 ef_(20)
#define ef_21 ef_(21)
#define ef_22 ef_(22)
#define ef_23 ef_(23)
#define ef_24 ef_(24)
#define ef_25 ef_(25)
#define ef_26 ef_(26)
#define ef_27 ef_(27)
#define ef_28 ef_(28)
#define ef_29 ef_(29)
#define ef_30 ef_(30)
#define ef_31 ef_(31)
#define ef_none 0

#define ef_all (( (unsigned long) 1<<31) | ~(-( (unsigned long) 1<<31)))
#define ef_keys ((unsigned long)1<<33)
#define ef_values ((unsigned long)1<<34)

#define ef_pT(pe, i) ((pe)->block[i])
#define ef_char(pe, i) T_char(ef_pT(pe, i))
#define ef_uchar(pe, i) T_uchar(ef_pT(pe, i))
#define ef_short(pe, i) T_short(ef_pT(pe, i))
#define ef_ushort(pe ,i) T_ushort(ef_pT(pe, i))
#define ef_int(pe, i) T_int(ef_pT(pe, i))
#define ef_uint(pe, i) T_uint(ef_pT(pe, i))
#define ef_long(pe, i) T_long(ef_pT(pe, i))
#define ef_ulong(pe, i) T_ulong(ef_pT(pe, i))
#define ef_float(pe, i) T_float(ef_pT(pe, i))
#define ef_double(pe, i) T_double(ef_pT(pe, i))
#define ef_ptr(pe, i) T_prt(ef_pT(pe, i))
#define ef_str(pe, i) T_str(ef_pT(pe, i))


typedef struct __entity_template {
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
int conflict_fix_entity(T*, T*);
int setup_entity(T*, T*);

entity_t* entity_create(entity_template_t* etpl);
entity_t* entity_cpy(entity_t* src);
void entity_format_data_block(entity_t*, unsigned long);
void entity_format_independent_entity_body(T*, entity_template_t*, unsigned long);
int entity_tpl_cal_independent_entity_body_size(entity_template_t*, unsigned long);
void entity_cpy_block(entity_t*, entity_t*, unsigned long);

int entity_read_from_vargs(entity_t*,  va_list, unsigned long);
int entity_write_to_vargs(entity_t*, va_list, unsigned long);



int entity_release(entity_t*);
#endif