/*
 * @Author: your name
 * @Date: 2021-10-21 15:16:26
 * @LastEditTime: 2021-10-23 20:03:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/Entity.c
 */
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "entity.h"
#include "base/type_value/__type_value_def.h"

// 单纯计算 block 中 data 的 size 的大小。
static int __entity_tpl_cal_block_data_size(entity_template_t* etpl, unsigned long accessor)
{ 
    int cal_field_num = (accessor & ef_keys) ? etpl->value_idx : etpl->field_num;
    int keys_data_block_size = 0;
    for (int i=0; i<cal_field_num; ++i) { 
        T_def *_def = T_def_get(etpl->field_types[i]); 
        keys_data_block_size += ROUND_UP_4(_def->ty_size); 
    } 
    return keys_data_block_size; 
}

// 计算整个 block 的 size 的大小。
static int __entity_tpl_cal_block_size(entity_template_t* etpl, unsigned long accessor) 
{ 
    int cal_field_num = (accessor & ef_keys) ? etpl->value_idx : etpl->field_num;
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
static int __entity_io_vargs(entity_t* ent, unsigned long accessor, va_list valist, unsigned char in) 
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
            T_def* _def = T_def_get(ent->tpl->field_types[i]);
            if (in) {
                _def->ty_adapter.read_vargs(valist, pT_i);
            } else {
                _def->ty_adapter.write_vargs(valist, pT_i);
            }
        }
    }
    return 0;
}

int entity_tpl_cal_independent_entity_body_size(entity_template_t* etpl, unsigned long accessor)
{
    int independent_entity_size = sizeof (entity_t);
    independent_entity_size += __entity_tpl_cal_block_size(etpl, accessor);
    independent_entity_size += sizeof (entity_template_t);
    independent_entity_size += sizeof(int) * (accessor & ef_keys ? etpl->value_idx : etpl->field_num);
    return independent_entity_size;
}

int cmp_entity(T* t1, T* t2)
{
    entity_t* e1 = (entity_t*) t1;
    entity_t* e2 = (entity_t*) t2;
    int result;
    T_def* _def;
    for (int i=0; i<e1->tpl->value_idx; ++i) {
        _def = T_def_get(e1->tpl->field_types[i]);
        result = _def->ty_cmp(e1->block[i], e2->block[i]);
        if (result != 0) return result;
    }
    return result;
}

int hash_entity(T* t, int slot_size)
{
    entity_t* ent = (entity_t*)t;
    unsigned int hash_code = 0;
    T_def* _def;
    for (int i=0; i<ent->tpl->value_idx; ++i) {
        _def = T_def_get(ent->tpl->field_types[i]);
        hash_code += _def->ty_hasher(ent->block[i], slot_size);
    }
    return hash_code % slot_size;
}

int conflict_fix_entity(T* t1, T* t2)
{
    entity_t* dest = t1;
    entity_t* src  = t2;
    T_def* _def;
    for (int i=dest->tpl->value_idx; i<dest->tpl->field_num; ++i) {
        _def = T_def_get(dest->tpl->field_types[i]);
        _def->ty_adapter.bit_cpy(dest->block[i], src->block[i]);
    }
}

int setup_entity(T* t1, T* t2)
{
    entity_t* ent1 = (entity_t*) t1;
    entity_t* ent2 = (entity_t*) t2;
    entity_t* cpy = entity_cpy(t2);
    ent1 = cpy;
}

entity_t* entity_create(entity_template_t* etpl)
{
    int total_size = 0;
    for (int i=0; i<etpl->field_num; ++i) {
        T_def* _def = T_def_get(etpl->field_types[i]);
        total_size += ROUND_UP_4(_def->ty_size);
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
    int data_block_size = __entity_tpl_cal_block_data_size(src->tpl, ef_all);
    memcpy(cpy->block[0], src->block[0], data_block_size);
    return cpy;
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
        T_def* _def = T_def_get(ent->tpl->field_types[i]);
        fields_ptr[i] = data_base_ptr + offset;
        offset += ROUND_UP_4(_def->ty_size);
        data_size += ROUND_UP_4(_def->ty_size);
    }
    memset(data_base_ptr, 0, data_size);
}

void entity_format_independent_entity_body(T* body, entity_template_t* etpl, unsigned long accessor)
{
    entity_t* ent = (entity_t*)body;
    char* base = body;
    int offset = sizeof(entity_t);
    ent->block = base + offset;

    offset += __entity_tpl_cal_block_size(etpl, accessor);
    ent->tpl = base + offset;

    ent->tpl->field_num = etpl->field_num;
    ent->tpl->value_idx = etpl->value_idx;

    for (int i=0; i<(accessor&ef_keys? etpl->value_idx : etpl->field_num); ++i) {
        ent->tpl->field_types = etpl->field_types[i];
    }
    entity_format_data_block(ent, accessor);
}



int entity_set(entity_t* ent, unsigned long accessor, ...)
{

}

int entity_get(entity_t* ent, unsigned long accessor, ...)
{

}

int entity_read_from_vargs(entity_t* ent, unsigned long accessor, va_list valist)
{

}

int entity_write_to_vargs(entity_t* ent, unsigned long accessor, va_list valist)
{

}

int entity_set_keys(entity_t* ent, ...)
{

}

int entity_set_values(entity_t* ent, ...)
{

}

int entity_get_keys(entity_t* ent, ...)
{

}

int entity_get_value(entity_t* ent, ...)
{

}

int entity_release(entity_t* ent)
{

}