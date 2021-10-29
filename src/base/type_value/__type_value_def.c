/*
 * @Author: your name
 * @Date: 2021-10-07 20:09:36
 * @LastEditTime: 2021-10-29 16:49:41
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/type_value/type_value_def.c
 */
#include <stdlib.h>
#include "__built_in_type_adapters.h"
#include "__type_value_def.h"

static T_adapter _T_ADAPTERS[MAX_T_DEF_SLOT_SIZE][4] = 
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

static T_def _T_DEFS[MAX_T_DEF_SLOT_SIZE] = 
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
    {0, 0, NULL, NULL},
    {0, 0, NULL, NULL},
    {0, 0, NULL, NULL},
    {0, 0, NULL, NULL},
    {0, 0, NULL, NULL},
};


int T_def_reg(int T_size, 
    int (*cmp)(type_value_t*, type_value_t*, int), 
    int(*hasher)(type_value_t*, int, int), 
    int (*setup)(type_value_t*, type_value_t*), 
    int(*vargs_read)(va_list, type_value_t*, int)
)
{
    for (int i=1; i<MAX_T_DEF_SLOT_SIZE; ++i) {
        T_def* _def= T_def_get(i);
        if (_def->ty_id == 0) {
            _def->ty_id = i;
            _def->ty_size = T_size;
        }
        return i;
    }
    return -1;
}

int T_def_unreg(int T_id) 
{
    _T_DEFS[T_id].T_id = 0;
}

int T_def_is_reg(int T_id) 
{
    return (T_id < 1 || T_id > MAX_T_DEF_SLOT_SIZE)? 0 : _T_DEFS[T_id].T_id > 0;
}

T_def* T_def_get(int T_id)
{
    return &_T_DEFS[T_id];
}