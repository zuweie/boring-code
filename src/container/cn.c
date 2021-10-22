/*
 * @Author: your name
 * @Date: 2021-10-21 11:58:55
 * @LastEditTime: 2021-10-22 13:28:43
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/cn.c
 */
#include <stdlib.h>
#include <stdarg.h>
#include "base/__container.h"
#include "base/type_value/__type_value_def.h"
#include "entity.h"
#include "cn.h"

#define CN_(i) ((cn_t*)__CONTAINERS[i])
#define SET_(i, ptr) (__CONTAINERS[i] = ptr)
static char* __CONTAINERS[CAPACITY_NUMBER] = {NULL};

static int __get_empty_slot ()
{
    for (int i=1; i<CAPACITY_NUMBER; ++i) {
        if ( CN_(i) == NULL) return i;
    }
    return 0;
}

CN CN_create(unsigned long build_code, ...)
{
    CN cn = __get_empty_slot();
    int err = err_ok;
    int i;

    va_list valist;
    var_start(valist, build_code);

    if (cn) {
        void* type_info      = NULL;
        entity_template_t* etpl_ptr = NULL;
        container_t* eng_ptr = NULL;
        cn_t* cn_ptr         = NULL;

        void* cmp_func = NULL;
        void* hash_func = NULL;
        void* conflict_fix_func = NULL;
        void* setup_func = NULL;

        // 1 初始化容器的数据类型
        if (build_code & use_entity) {
            // 使用复合体，entity
            if (build_code & customized_entity) {
                // 使用自定义的复合体
                int filed_num = va_arg(valist, int);
                if (filed_num > MAX_ENT_FIELD_NUM) {
                    err = err_out_of_max_entity_field;
                    goto BUILD_FAIL;
                }

                int value_index = va_arg(valist, int);
                if (value_index > filed_num && value_index <= 0) {
                    err = err_invalid_ent_keys_number;
                    goto BUILD_FAIL;
                }
                etpl_ptr = (entity_template_t*)malloc(sizeof(entity_template_t) + filed_num * sizeof(int));
                etpl_ptr->field_types = (int*)(&etpl_ptr[1]);

                for (i=0; i<filed_num; ++i) {
                    etpl_ptr->field_types[i] = va_arg(valist, int);
                    if (!T_def_is_reg(etpl_ptr->field_types[i])) {
                        err = err_unsupported_type;
                        goto BUILD_FAIL;
                    }
                }
                etpl_ptr->field_num = filed_num;
                etpl_ptr->value_idx = value_index;
            } else {
                // 使用默认的复合体，也就是一个key, 一个value
                etpl_ptr = (entity_template_t*)malloc(sizeof(entity_template_t) + 2 * sizeof(int));
                etpl_ptr->field_types = (int*)(&etpl_ptr[1]);
                etpl_ptr->field_types[0] = va_arg(valist, int);
                etpl_ptr->field_types[1] = va_arg(valist, int);

                for (i=0; i<2; ++i) {
                    if (!T_def_is_reg(etpl_ptr->field_types[i])) {
                        err = err_unsupported_type;
                        goto BUILD_FAIL;
                    }
                }

                etpl_ptr->field_num = 2;
                etpl_ptr->value_idx = 1;
            }
            type_info = etpl_ptr;
        } else {
            // 使用单值
            int type = va_arg(valist, int);
            if (!T_def_is_reg(type)) {
                err = err_unsupported_type;
                goto BUILD_FAIL;
            }
            type_info = (void*)type;
        }
        // 2 使用自定义的比较函数
        if (build_code & customized_compare) {
            cmp_func = (void*)va_arg(valist, void*);
        } else if (build_code & use_entity) {
            cmp_func = (void*)&cmp_entity;
        } 

        if (build_code & customized_hasher) {
            hash_func = (void*)va_arg(valist, void*);
        } else if (build_code & use_entity) {
            hash_func = (void*)&hash_entity;
        }

        if ((build_code & TREE_SET || build_code & HASH_SET) && ( build_code & use_entity) {
            conflict_fix_func = (void*)&conflict_entity;
            setup_func        = (void*)&setup_entity;
        }
        
        // 要认真开始 建造容器了
        cn_ptr = (cn_t*)malloc(sizeof(cn_t));
        T_def* _def;
        if (build_code & VECTOR) {
            if (build_code & use_entity) {
                _def = T_def_get(ptr_t);
            } else {
                _def = T_def_get((int)type_info);
            }
            eng_ptr = container_create(vector, _def);
        } else if (build_code & LIST) {
            if (build_code & use_entity) {
                _def = T_def_get(ptr_t);                
            } else {
                _def = T_def_get((int)type_info);
            }
            if (cmp_func) _def->ty_cmp = cmp_func;
            eng_ptr = container_create(list, _def);
        } else if (build_code & TREE_SET) {
            err = err_unsupported_eng;
            goto BUILD_FAIL;
        } else if (build_code & HASH_SET) {
            err = err_unsupported_eng;
            goto BUILD_FAIL;
        }
        
        cn_ptr->build_code = build_code;
        cn_ptr->eng = eng_ptr;
        cn_ptr->type_info = type_info;
        cn_ptr->is_forwag = 1;

    } else {
        err = err_out_of_capcity;
    }
    BUILD_FAIL:
    va_end(valist);
    if (err != err_ok) {
        if (etpl_ptr) free(etpl_ptr);
        if (cn_ptr) free(cn_ptr);
        return err;
    } else {
        SET_(cn, cn_ptr);
        return cn;
    }
}
CN CN_finalize(CN cn, int(*cleanup)(T*));