/*
 * @Author: your name
 * @Date: 2021-10-21 11:58:55
 * @LastEditTime: 2021-10-26 00:05:27
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/cn.c
 */
#include <stdlib.h>
#include <stdarg.h>
#include "base/__container.h"
#include "base/__iterator.h"
#include "base/type_value/__type_value_def.h"
#include "entity.h"
#include "it.h"
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

static int __cn_remove_at(CN cn, It at, T* rdata) 
{
    int err = err_ok;
    if (CN_size(cn) != 0) {

        if (It_is_tail(at)) 
            return err_invalid_pos;

        if (CN_(cn)->build_code & use_entity) {
            entity_t* ent;
            container_remove(CN_(cn)->eng, at._iter, &ent);
            if (rdata) entity_cpy_block_data(rdata, ent, ef_all);
            entity_release(ent);
        } else {
            container_remove(CN_(cn)->eng, at._iter, rdata);
        }
    } else {
        err = err_empty;
    }
    return err;
}
static int __cn_add_at(CN cn, It at, va_list valist)
{
    if (It_is_head(at)) 
        return err_invalid_pos;

    int err = err_ok;

    if (CN_(cn)->build_code & use_entity) {
        CN_READ_ENTITY_VARGS(cn, ent, valist, ef_all);
        container_insert(CN_(cn)->eng, at._iter, ent);
    } else {
        CN_READ_SINGLE_VALUE_VARGS(cn, tv, valist);
        container_insert(CN_(cn)->eng, at._iter, tv);
    }
    return err;
}
static It __cn_find_at(CN cn, It at, va_list valist) 
{   
    iterator_t it;
    if (CN_(cn)->build_code & use_entity) {
        CN_READ_ENTITY_VARGS(cn, ent, valist, ef_keys);
        it = container_search(CN_(cn)->eng, at._iter, ent, NULL);
    } else {
        CN_READ_SINGLE_VALUE_VARGS(cn, tv, valist);
        it = container_search(CN_(cn), at._iter, tv, NULL);
    }
    return It(it, cn);
}

// public function :
It CN_head(CN cn)
{
    iterator_t head = container_head(CN_(cn)->eng);
    return It(head, cn);
}
It CN_tail(CN cn)
{
    iterator_t tail = container_tail(CN_(cn)->eng);
    return It(tail, cn);
}
It CN_first(CN cn)
{
    iterator_t first = container_first(CN_(cn)->eng);
    return It(first, cn);
}

It CN_last(CN cn)
{
    iterator_t last = container_last(CN_(cn)->eng);
    return It(last, cn);
}

CN CN_create(unsigned long build_code, ...)
{
    CN cn = __get_empty_slot();
    int err = err_ok;
    int i;

    va_list valist;
    var_start(valist, build_code);

    cn_t* cn_ptr                = NULL;
    entity_template_t* etpl_ptr = NULL;

    if (cn) {
        void* type_info      = NULL;
        container_t* eng_ptr = NULL;

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
                    err = err_out_of_max_ent_field;
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
            conflict_fix_func = (void*)&conflict_fix_entity;
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
            unsigned char multi = build_code & multi_key;
            if (build_code & use_entity) {
                _def = T_def_get(ptr_t);
            } else {
                _def = T_def_get((int)type_info);
            }
            eng = container_create(rb_tree, multi, setup_func, conflict_fix_func);
        } else if (build_code & HASH_SET) {
            unsigned char multi = build_code & multi_key;
            if (build_code & use_entity) {
                _def = T_def_get(ptr_t);
            } else {
                _def = T_def_get((int)type_info);
            }
            eng = container_create(hash, multi, setup_func, conficlt_fix_func);
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
CN CN_finalize(CN cn, int(*cleanup)(T*))
{
    // TODO : release the container;
}

void* CN_type_info(CN cn) 
{
    return CN_(cn)->type_info;
}

T_def* CN_type_def(CN cn) 
{
    if (! (CN_(cn)->build_code & use_entity)) {
        return T_get_def( (int)(CN_(cn)->type_info));
    } else {
        return NULL;
    }
}
// sequence function
int CN_add(CN cn, ...) 
{
    va_list valist;
    va_start(valist, cn);
    int err = __cn_add_at(cn, CN_tail(cn), valist);
    va_end(valist);
    return err;
}
int CN_remove(CN cn, T* rdata)
{
    return __cn_remove_at(cn, CN_last(cn), rdata);
}

int CN_remove_at(CN cn, It it, T* rdata) 
{
    return __cn_remove_at(cn, it, rdata);
}

It CN_find(CN cn, ...) 
{
    va_list valist;
    va_start(valist, cn);
    It it = __cn_find_at(cn, CN_first(cn), valist);
    va_end(valist);
    return it;
}

It CN_find_at(CN cn, It pos, ...) 
{
    va_list valist;
    va_start(valist, pos);
    It it = __cn_find_at(cn, pos, valist);
    va_end(valist);
    return it;
}

// mapping function
int CN_del(CN cn, ...)
{
    int err = err_ok;
    if (CN_(cn)->build_code & TREE_SET || CN_(cn)->build_code & HASH_SET) {
        va_list valist;
        va_start(valist, cn);
        if (CN_(cn)->build_code & use_entity) {
            // 这里是一个map
            CN_READ_ENTITY_VARGS(cn, rm_keys, valist, ef_keys);
            iterator_t it = container_search(CN_(cn)->eng, __null_iterator, rm_keys, NULL);
            if (!iterator_is_tail(it)) {
                entity_t* rm;
                container_remove(CN_(cn)->eng, it, &rm);
                entity_release(rm);
            } else {
                err = err_no_found;
            }
            
        } else {
            CN_READ_SINGLE_VALUE_VARGS(cn, rm, valist);
            iterator_t it = container_search(CN_(cn)->eng, __null_iterator, rm, NULL);
            if (!iterator_is_tail(it)) {
                container_remove(CN_(cn)->eng, it, NULL);
            } else {
                err = err_no_found;
            }
        }
        va_end(valist);
        return err;
    } else {
        err = err_unsupported_method;
    }
    return err;
}
int CN_set(CN cn, ...)
{
    int err = err_ok;
    if (CN_(cn)->build_code & TREE_SET || CN_(cn)->build_code & HASH_SET ) {
        va_list valist;
        va_start(valist, cn);

        if (CN_(cn)->build_code & use_entity) {
            CN_READ_ENTITY_VARGS(cn, ent, valist, ef_all);
            container_insert(CN_(cn)->eng, __null_iterator, ent);
        }  else {
            CN_READ_SINGLE_VALUE_VARGS(cn, tvalue, valist);
            container_insert(CN_(cn)->eng, __null_iterator, tvalue);
        }
        va_end(valist);
    } else {
        err = err_unsupported_method;
    }

    return err;
}
T* CN_get(CN cn, ...)
{
    int err = err_ok;
    T* ret = NULL;
    if (CN_(cn)->build_code & TREE_SET || CN_(cn)->build_code && HASH_SET) 
    {
        va_list valist;
        va_start(valist, cn);

        if (CN_(cn)->build_code & use_entity) {
            CN_READ_ENTITY_VARGS(cn, ent, valist, ef_keys);
            iterator_t it = container_search(CN_(cn)->eng, __null_iterator, ent, NULL);
            if (!iterator_is_tail(it)) {
                entity_t* ent = it.reference;
                return &ent->block[ent->tpl->value_idx];
            }
            
        } else {
            CN_READ_SINGLE_VALUE_VARGS(cn, tv, valist);
            iterator_t it = container_search(CN_(cn)->eng, __null_iterator, tv, NULL);
            if (!iterator_is_tail(it)) {
                return it.reference;
            }
        }

        va_end(valist);
    } 
    return NULL;
}