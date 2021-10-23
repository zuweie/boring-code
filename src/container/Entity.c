/*
 * @Author: your name
 * @Date: 2021-10-21 15:16:26
 * @LastEditTime: 2021-10-23 10:43:34
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/Entity.c
 */
#include <stdlib.h>
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

// #define entity_tpl_cal_whole_block_size(ptpl) \
// ({ \
//     int marco_whole_block_size = sizeof(T*) * ((entity_template_t*)(ptpl))->field_num; \
//     marco_whole_block_size += entity_tpl_cal_whole_data_block_size(ptpl); \
//     marco_whole_block_size; \
// })

int cmp_entity(T* t1, T* t2)
{

}

int hash_entity(T* t, int slot_size)
{

}

int conflict_fix_entity(T* t1, T* t2)
{

}

int setup_entity(T* t1, T* t2)
{

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

void entity_format_real_entity_body(T* ent_body, entity_template_t* etpl, unsigned long accessor)
{

}

int entity_tpl_cal_real_entity_body_size(entity_template_t* etpl, unsigned long accessor)
{

}

void entity_cpy_block(entity_t* dest, entity_t* src, unsigned long accessor)
{

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