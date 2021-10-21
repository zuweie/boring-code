/*
 * @Author: your name
 * @Date: 2021-10-07 20:09:36
 * @LastEditTime: 2021-10-21 15:07:53
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/type_value/type_value_def.c
 */
#include <stdlib.h>
#include "__built_in_type.h"
#include "__type_value_def.h"

static T_def _T_DEFS[MAX_T_DEF_SLOT_SIZE] = 
{
    // 0
    {0, 0, NULL, NULL, {NULL, NULL, NULL}},
    // 1 ~ 13
    {char_t, sizeof(char), &cmp_char, &hash_char, {&read_vargs_char, &write_vargs_char, &bit_cpy_char}},
    {uchar_t, sizeof(unsigned char), &cmp_uchar, &hash_uchar, {&read_vargs_uchar, &write_vargs_uchar, &bit_cpy_uchar}},
    {short_t, sizeof(short), &cmp_short, &hash_short, {&read_vargs_short, &write_vargs_short, &bit_cpy_short}},
    {ushort_t, sizeof(unsigned short), &cmp_ushort, &hash_ushort, {&read_vargs_ushort, &write_vargs_ushort, &bit_cpy_ushort}},
    {int_t, sizeof(int), &cmp_int, &hash_int, {&read_vargs_int, &write_vargs_int, &bit_cpy_int}},
    {uint_t, sizeof(unsigned int), &cmp_uint, &hash_uint, {&read_vargs_uint, &write_vargs_uint, &bit_cpy_uint}},
    {long_t, sizeof(long), &cmp_long, &hash_long, {&read_vargs_long, &write_vargs_long, &bit_cpy_long}},
    {ulong_t, sizeof(unsigned long), &cmp_ulong, &hash_ulong, {&read_vargs_ulong, &write_vargs_ulong, &bit_cpy_ulong}},
    {fl_t, sizeof(float), &cmp_flv, &hash_float, {&read_vargs_float, &write_vargs_float, &bit_cpy_float}},
    {db_t, sizeof(double), &cmp_dbl, &hash_double, {&read_vargs_double, &write_vargs_double, &bit_cpy_double}}, 
    {str_t, sizeof(char*), &cmp_str, &hash_str, {&read_vargs_str, &write_vargs_str, &bit_cpy_str}},
    {ptr_t, sizeof(char*), &cmp_ptr, &hash_ptr, {&read_vargs_ptr, &write_vargs_ptr, &bit_cpy_ptr}},

    // 14 ~ 31 保留自用
    {0, 0, NULL, NULL, {NULL, NULL, NULL}},
    {0, 0, NULL, NULL, {NULL, NULL, NULL}},
    {0, 0, NULL, NULL, {NULL, NULL, NULL}},
    {0, 0, NULL, NULL, {NULL, NULL, NULL}},
    {0, 0, NULL, NULL, {NULL, NULL, NULL}},
    {0, 0, NULL, NULL, {NULL, NULL, NULL}},
    {0, 0, NULL, NULL, {NULL, NULL, NULL}},
    {0, 0, NULL, NULL, {NULL, NULL, NULL}},
    {0, 0, NULL, NULL, {NULL, NULL, NULL}},
    {0, 0, NULL, NULL, {NULL, NULL, NULL}},
    {0, 0, NULL, NULL, {NULL, NULL, NULL}},
    {0, 0, NULL, NULL, {NULL, NULL, NULL}},
    {0, 0, NULL, NULL, {NULL, NULL, NULL}},
    {0, 0, NULL, NULL, {NULL, NULL, NULL}},
    {0, 0, NULL, NULL, {NULL, NULL, NULL}},
    {0, 0, NULL, NULL, {NULL, NULL, NULL}},
    {0, 0, NULL, NULL, {NULL, NULL, NULL}},
    {0, 0, NULL, NULL, {NULL, NULL, NULL}},
}

int T_def_reg(int T_size, int (*cmp)(type_value_t*, type_value_t*), int(*hasher)(type_value_t*, int), int (*read_vargs)(va_list, type_value_t*), int (*write_args)(va_list, type_value_t*), int(*bit_cpy)(type_value_t*, type_value_t*))
{
    for (int i=1; i<MAX_T_DEF_SLOT_SIZE; ++i) {
        T_def* _def= T_def_get(i);
        if (_def->ty_id == 0) {
            _def->ty_id = i;
            _def->ty_size = T_size;
            _def->ty_cmp = cmp;
            _def->ty_hasher = hasher;
            _def->ty_adapter.read_vargs = read_vargs;
            _def->ty_adapter.write_vargs = write_args;
            _def->ty_adapter.bit_cpy = bit_cpy;
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