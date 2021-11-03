/*
 * @Author: your name
 * @Date: 2021-10-07 20:09:36
 * @LastEditTime: 2021-11-03 15:45:10
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/type_value/type_value_def.c
 */
#include <stdlib.h>
#include <string.h>
#include "__built_in_type_adapters.h"
#include "__type_value_def.h"

static T_adapter __T_ADAPTERS_TAB[MAX_T_DEF_SLOT_SIZE][4] = 
{
    {0,0,0,0},
    {&cmp_char, &hash_char, &setup_char, &vargs_char},
    {&cmp_uchar, &hash_uchar, &setup_uchar, &vargs_uchar},
    {&cmp_int, &hash_int, &setup_int, &vargs_int},
    {&cmp_uint, &hash_uint, &setup_uint, &vargs_uint},
    {&cmp_long, &hash_long, &setup_long, &vargs_long},
    {&cmp_ulong, &hash_ulong, &setup_long, &vargs_long},
    {&cmp_float, &hash_float, &setup_float, &vargs_float},
    {&cmp_double, &hash_double, &setup_double, &vargs_double},
    {&cmp_str, &hash_str, &setup_str, &setup_str},
    {&cmp_ptr, &hash_ptr, &setup_ptr, &vargs_ptr},

    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

static T_def __T_DEFS_BASE[MAX_T_DEF_SLOT_SIZE] = 
{
    // 0
    {0, 0},
    // 1 ~ 10
    {char_t, sizeof(char)},
    {uchar_t, sizeof(unsigned char)},
    {int_t, sizeof(int)},
    {uint_t, sizeof(unsigned int)},
    {long_t, sizeof(long)},
    {ulong_t, sizeof(unsigned long)},
    {fl_t, sizeof(float)},
    {db_t, sizeof(double)}, 
    {str_t, sizeof(char*)},
    {ptr_t, sizeof(char*)},

    // 11 ~ 15 保留备用
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
};


int T_def_reg(
    int T_size, 
    adapter_cmp cmp,
    adapter_hasher hasher,
    adapter_setup setup,
    adapter_vargs_reader vargs_reader
)
{
    for (int i=1; i<MAX_T_DEF_SLOT_SIZE; ++i) {
        T_def* _def = &__T_DEFS_BASE[i];
        if (_def->ty_id == 0) {
            _def->ty_id = i;
            _def->ty_size = T_size;
        }
        __T_ADAPTERS_TAB[i][e_cmp] = cmp;
        __T_ADAPTERS_TAB[i][e_hash] = hasher;
        __T_ADAPTERS_TAB[i][e_setup] = setup;
        __T_ADAPTERS_TAB[i][e_vargs] = vargs_reader;
        return i;
    }
    return -1;
}

int T_def_unreg(int T_id) 
{
    __T_DEFS_BASE[T_id].ty_id = 0;
    __T_DEFS_BASE[T_id].ty_size = 0;
    memset(__T_ADAPTERS_TAB[T_id], 0, 4 * sizeof(T_adapter));
}

int T_def_is_reg(int T_id) 
{
    return (T_id < 1 || T_id > MAX_T_DEF_SLOT_SIZE)? 0 : __T_DEFS_BASE[T_id].ty_id > 0;
}

T_def T_def_get(int T_id) 
{
    return __T_DEFS_BASE[T_id];
}

T_adapter T_adapter_get(int T_id, adapter_t adapter) 
{
    return __T_ADAPTERS_TAB[T_id][adapter];
}
