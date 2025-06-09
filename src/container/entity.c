/*
 * @Author: your name
 * @Date: 2021-10-21 15:16:26
 * @LastEditTime: 2025-06-09 11:50:56
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/Entity.c
 */
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "Entity.h"
#include "base/type_value/__type_value_def.h"

// 单纯计算 block 中 data 的 size 的大小。
static int __entity_tpl_cal_block_data_size(entity_template_t* etpl, unsigned long accessor)
{ 
    //int cal_field_num = (accessor & ef_keys) ? etpl->value_idx : (accessor & ef_values) ? (etpl->field_num  - etpl->value_idx) : etpl->field_num;
    int cal_field_num;
    int cal_field_start;
    if (accessor & ef_keys) {
        cal_field_num = etpl->value_idx;
        cal_field_start = 0;
    } else if (accessor & ef_values) {
        cal_field_num = etpl->field_num - etpl->value_idx + 1;
        cal_field_start = etpl->value_idx;
    } else {
        cal_field_num = etpl->field_num;
        cal_field_start = 0;
    }
    int data_block_size = 0;
    for (int i = cal_field_start; i<cal_field_num; ++i) { 
        T_def _def = T_def_get(etpl->field_types[i]); 
        data_block_size += ROUND_UP_4(_def.ty_size); 
    } 
    return data_block_size; 
}

// 计算整个 block 的 size 的大小。
static int __entity_tpl_cal_block_size(entity_template_t* etpl, unsigned long accessor) 
{ 
    int cal_field_num = (accessor & ef_keys) ? etpl->value_idx : (accessor & ef_values) ? etpl->field_num - etpl->value_idx : etpl->field_num;
    int block_size = sizeof(T*) * cal_field_num;
    block_size += __entity_tpl_cal_block_data_size(etpl, accessor);
    return block_size; 
}

static inline unsigned long __entity_keys_accessor(entity_t* ent)
{
    unsigned long accessor = 0;
    for (int i=0; i<ent->tpl->value_idx; ++i) {
        accessor |= ef_(i);
    }
    return accessor;
}
static inline unsigned long __entity_values_accessor(entity_t* ent) 
{
    unsigned long accessor = 0;
    for (int i=ent->tpl->value_idx; i<ent->tpl->field_num; ++i) {
        accessor |= ef_(i); 
    }
    return accessor;
}
static int __entity_read_vargs(entity_t* ent, unsigned long accessor, va_list valist, int context) 
{
    unsigned _accessor = 0;
    if (accessor & ef_keys) {
        _accessor = __entity_keys_accessor(ent);
    } else if (accessor & ef_values) {
        _accessor = __entity_values_accessor(ent);
    } else {
        _accessor = accessor;
    }

    for (int i=0; i<ent->tpl->field_num; ++i) {
        if ( _accessor & ef_(i) ) {
            T* pT_i = ent->block[i];
            T_adapter _adapter = T_adapter_get(ent->tpl->field_types[i], e_vargs);
            ((adapter_vargs_reader)(_adapter))(valist, pT_i, context);
        }
    }
    return 0;
}



int cmp_entity(T* t1, T* t2)
{
    entity_t* e1 = *((entity_t**) t1);
    entity_t* e2 = *((entity_t**) t2);
    int result;
    T_def _def;
    for (int i=0; i<e1->tpl->value_idx; ++i) {
        T_adapter _adapter = T_adapter_get(e1->tpl->field_types[i], e_cmp);
        result = ((adapter_cmp)(_adapter))(e1->block[i], e2->block[i]);
        if (result != 0) return result;
    }
    return result;
}

int hash_entity(T* t, int slot_size)
{
    entity_t* ent = *((entity_t**)t);
    unsigned long hash_code = 0;
    T_def* _def;
    for (int i=0; i<ent->tpl->value_idx; ++i) {
        T_adapter _adapter = T_adapter_get(ent->tpl->field_types[i], e_hash);
        hash_code += ((adapter_hasher)(_adapter))(ent->block[i], slot_size);
    }
    return hash_code % slot_size;
}

int setup_entity(T* t1, T* t2, unsigned char old_block)
{
    if (old_block) {
        entity_t* dest = *((entity_t**)t1);
        entity_t* src  = *((entity_t**)t2);
        entity_cpy_block_data(dest, src, ef_values);
    } else {
        entity_t* ent2 = *((entity_t**) t2);
        entity_t* cpy = entity_cpy(ent2);
        *((entity_t**)t1) = cpy;
    }
    return 0;
}

int vargs_reader_entity(va_list valist, T* ent, int context) 
{
    if (context == 1) {
        // read for insert
        __entity_read_vargs(ent, ef_all, valist, context);
    } else {
        // read for search
        __entity_read_vargs(ent, ef_keys, valist, context);
    }
    return 0;
}

entity_t* entity_create(entity_template_t* etpl)
{
    int total_size = 0;
    for (int i=0; i<etpl->field_num; ++i) {
        T_def _def = T_def_get(etpl->field_types[i]);
        total_size += ROUND_UP_4(_def.ty_size);
    }
    total_size += etpl->field_num * sizeof(T*);
    total_size += sizeof(entity_t);

    entity_t* ent = (entity_t*) malloc(total_size);
    ent->tpl = etpl;
    ent->block = &ent[1];
    entity_format_data_block(ent, ef_all);
    return ent;
}

entity_t* entity_cpy(entity_t* src)
{
    entity_t* cpy = entity_create(src->tpl);
    entity_cpy_block_data(cpy, src, ef_all);
    return cpy;
}
void entity_cpy_block_data(entity_t* dest, entity_t* src, unsigned long accessor) 
{
    int data_block_size = __entity_tpl_cal_block_data_size(dest->tpl, accessor);
    int block_cpy_idx = (accessor & ef_keys) ? 0 : (accessor & ef_values) ? dest->tpl->value_idx : 0;
    memcpy(dest->block[block_cpy_idx], src->block[block_cpy_idx], data_block_size);
}

void entity_format_data_block(entity_t* ent, unsigned long accessor)
{
    int field_num = 0;
    if (accessor & ef_keys) {
        field_num = ent->tpl->value_idx;
    } else {
        field_num = ent->tpl->field_num;
    }

    T** fields_ptr = ent->block;
    T* data_base_ptr = &fields_ptr[field_num];
    int offset = 0;
    int data_size = 0;
    for (int i=0; i<field_num; ++i) {
        T_def _def = T_def_get(ent->tpl->field_types[i]);
        fields_ptr[i] = data_base_ptr + offset;
        offset += ROUND_UP_4(_def.ty_size);
        data_size += ROUND_UP_4(_def.ty_size);
    }
    memset(data_base_ptr, 0, data_size);
}

int entity_tpl_cal_body_size(entity_template_t* etpl, unsigned long accessor)
{
    int independent_entity_size = sizeof (entity_t);
    independent_entity_size += __entity_tpl_cal_block_size(etpl, accessor);
    independent_entity_size += sizeof (entity_template_t);
    independent_entity_size += sizeof(int) * (accessor & ef_keys ? etpl->value_idx : etpl->field_num);
    return independent_entity_size;
}

void entity_tpl_format_body(T* body, entity_template_t* etpl, unsigned long accessor)
{
    entity_t* ent = (entity_t*)body;
    char* base = body;
    int offset = sizeof(entity_t);
    ent->block = (char*)(base + offset);

    offset += __entity_tpl_cal_block_size(etpl, accessor);
    ent->tpl = (char*)(base + offset);

    ent->tpl->field_num = etpl->field_num;
    ent->tpl->value_idx = etpl->value_idx;
    ent->tpl->type_class = etpl->type_class;

    offset += sizeof(entity_template_t);
    ent->tpl->field_types = (char*)(base + offset);

    for (int i=0; i<(accessor&ef_keys? etpl->value_idx : etpl->field_num); ++i) {
        ent->tpl->field_types[i] = etpl->field_types[i];
    }
    entity_format_data_block(ent, accessor);
}

// 用于计算包含 entity_template_t 指针的 entity 的大小
int entity_cal_body_size(entity_template_t* etpl, unsigned long accessor)
{
    int entity_size = sizeof (entity_t);
    entity_size += __entity_tpl_cal_block_size(etpl, accessor);
    return entity_size;
}
// 用于格式化包含 entity_template_t 指针的 entity 的大小
void entity_format_body(T* body, entity_template_t* etpl, unsigned long accessor)
{
    entity_t* ent = (entity_t*)body;
    char* base = body;
    int offset = sizeof(entity_t);
    ent->block = (char*)(base+offset);
    ent->tpl = etpl;
    entity_format_data_block(ent, accessor);
}

int entity_release(entity_t* ent)
{
    free(ent);
    return 0;
}