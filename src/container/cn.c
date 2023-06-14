/*
 * @Author: your name
 * @Date: 2021-10-21 11:58:55
 * @LastEditTime: 2023-06-14 12:33:32
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/cn.c
 */
#include <stdlib.h>
#include <stdarg.h>
#include "base/__container.h"
#include "base/__iterator.h"
#include "base/type_value/__type_value_def.h"
#include "base/operate/__heap_sort.h"
#include "base/operate/__quick_sort.h"
#include "base/operate/__wring.h"
#include "base/__vector.h"
#include "base/__list.h"
#include "base/__hash.h"
#include "base/__rb_tree.h"
#include "entity.h"
#include "it.h"
#include "cn.h"

#define CN_(i) ((cn_t*)__CONTAINERS[i])
#define SET_(i, ptr) (__CONTAINERS[i] = ptr)
// private data
static char* __CONTAINERS[CAPACITY_NUMBER] = {NULL};

// private function

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

        if (It_is_tail(at) || It_is_head(at)) 
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
    if (CN_(cn)->build_code & VECTOR && It_is_head(at)) 
        return err_invalid_pos;

    int err = err_ok;

    if (CN_(cn)->build_code & use_entity) {
        CN_READ_ENTITY_VARGS(cn, ent, valist, ef_all, using_at_add);
        container_insert(CN_(cn)->eng, at._iter, &ent);
    } else {
        CN_READ_SINGLE_VALUE_VARGS(cn, tv, valist, using_at_add);
        container_insert(CN_(cn)->eng, at._iter, tv);
    }
    return err;
}
static It __cn_find_at(CN cn, It at, va_list valist) 
{   
    iterator_t it;
    if (CN_(cn)->build_code & use_entity) {
        CN_READ_ENTITY_VARGS(cn, ent, valist, ef_keys, using_at_find);
        it = container_search(CN_(cn)->eng, at._iter, &ent, NULL);
    } else {
        CN_READ_SINGLE_VALUE_VARGS(cn, tv, valist, using_at_find);
        it = container_search(CN_(cn)->eng, at._iter, tv, NULL);
    }
    return It(it, CN_(cn));
}

// public function :
It CN_head(CN cn)
{
    iterator_t head = CN_(cn)->is_forward ? container_head(CN_(cn)->eng) : container_tail(CN_(cn)->eng);
    return It(head, CN_(cn));
}
It CN_tail(CN cn)
{
    iterator_t tail = CN_(cn)->is_forward ? container_tail(CN_(cn)->eng) : container_head(CN_(cn)->eng);
    return It(tail, CN_(cn));
}
It CN_first(CN cn)
{
    iterator_t first = CN_(cn)->is_forward ? container_first(CN_(cn)->eng) : container_last(CN_(cn)->eng);
    return It(first, CN_(cn));
}

It CN_last(CN cn)
{
    iterator_t last = CN_(cn)->is_forward ? container_last(CN_(cn)->eng) : container_first(CN_(cn)->eng);
    return It(last, CN_(cn));
}


CN CN_create(unsigned long build_code, ...)
{
    CN cn = __get_empty_slot();
    int err = err_ok;
    int i;

    va_list valist;
    va_start(valist, build_code);

    cn_t* cn_ptr         = NULL;
    T_clazz* type_clazz  = NULL;

    if (cn) {
        container_t* eng_ptr = NULL;

        // void* cmp_func = NULL;
        // void* hash_func = NULL;
        // void* setup_func = NULL;
        // void* vargs_reader_func = NULL;

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
                entity_template_t* etpl_ptr = (entity_template_t*)malloc(sizeof(entity_template_t) + filed_num * sizeof(int));
                type_clazz = etpl_ptr;

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
                entity_template_t* etpl_ptr = (entity_template_t*)malloc(sizeof(entity_template_t) + 2 * sizeof(int));
                type_clazz = etpl_ptr;

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
            //初始化 type_clazz，entity 本质就是一个 pointer 类型
            type_clazz->_def = T_def_get(ptr_t);


        } else {
            // 使用单值
            int type = va_arg(valist, int);
            if (!T_def_is_reg(type)) {
                err = err_unsupported_type;
                goto BUILD_FAIL;
            }
            type_clazz = (T_clazz*)malloc(sizeof(T_clazz));
            type_clazz->_def = T_def_get(type);
        }

        // 2 初始化函数行为。
        if (build_code & customized_compare) {
            type_clazz->_adapter[e_cmp] = (T_adapter)va_arg(valist, T_adapter);
        } else if (build_code & use_entity) {
            type_clazz->_adapter[e_cmp] = (T_adapter)&cmp_entity;
        } else {
            type_clazz->_adapter[e_cmp] = T_adapter_get(type_clazz->_def.ty_id, e_cmp);
        }

        if (build_code & customized_hasher) {
            type_clazz->_adapter[e_hash] = (T_adapter)va_arg(valist, T_adapter);
        } else if (build_code & use_entity) {
            type_clazz->_adapter[e_hash] = (T_adapter)&hash_entity;
        } else {
            type_clazz->_adapter[e_hash] = T_adapter_get(type_clazz->_def.ty_id, e_hash);
        }

        if (build_code & customized_setup) {
            type_clazz->_adapter[e_setup] = (T_adapter)va_arg(valist, T_adapter);
        } else if (build_code & use_entity) {
            type_clazz->_adapter[e_setup] = (T_adapter)&setup_entity;
        } else {
            type_clazz->_adapter[e_setup] = T_adapter_get(type_clazz->_def.ty_id, e_setup);
        }
        
        if (build_code & customized_vargs_reader) {
            type_clazz->_adapter[e_vargs] = (T_adapter)va_arg(valist, T_adapter);
        } else if (build_code & use_entity) {
            type_clazz->_adapter[e_vargs] = (T_adapter)&vargs_reader_entity;
        } else {
            type_clazz->_adapter[e_vargs] = T_adapter_get(type_clazz->_def.ty_id, e_vargs);
        }
        
        // 要认真开始 建造容器了
        cn_ptr = (cn_t*)malloc(sizeof(cn_t));
        T_def* _def;
        if (build_code & VECTOR) {
            eng_ptr = container_create(vector, type_clazz);
        } else if (build_code & LIST) {
            eng_ptr = container_create(list, type_clazz);
        } else if (build_code & TREE_SET) {
            unsigned char multi = build_code & multi_key ? 1 : 0;
            eng_ptr = container_create(rb_tree, type_clazz, multi);
        } else if (build_code & HASH_SET) {
            unsigned char multi = build_code & multi_key ? 1 : 0;
            eng_ptr = container_create(hash, type_clazz, 1024, multi);
        }
        
        cn_ptr->build_code = build_code;
        cn_ptr->eng = eng_ptr;
        cn_ptr->type_clazz = type_clazz;
        cn_ptr->is_forward = 1;

    } else {
        err = err_out_of_capcity;
    }
    BUILD_FAIL:
    va_end(valist);
    if (err != err_ok) {
        if (type_clazz) free(type_clazz);
        if (cn_ptr) free(cn_ptr);
        return err;
    } else {
        SET_(cn, cn_ptr);
        return cn;
    }
}
CN CN_finalize(CN cn, int(*cleanup)(T*))
{
    // // TODO : release the container;
    T_clazz* _class = CN_ty_clazz(cn);
    

    // 若是有 cleanup 函数，或者此容器用的是 entity 则需要遍历一次进行处理。
    
    if ( (CN_(cn)->build_code & use_entity) || cleanup) {

        for (It first=CN_first(cn); !It_equal(first, CN_tail(cn)); It_next(first)) {

            // 若是有 cleanup 函数，即调用 cleanup 函数
            if (cleanup) {
                cleanup(first._iter.reference);
            }

            // 完了就释放 entity 的内存。
            if (CN_(cn)->build_code & use_entity) {
                entity_t* ent = It_ptr(first);
                entity_release(ent);
            }
        }
    }

    if (CN_(cn)->build_code & VECTOR) {
        container_destroy(vector, CN_(cn)->eng);
    } else if (CN_(cn)->build_code & LIST) {
        container_destroy(list, CN_(cn)->eng);
    } else if (CN_(cn)->build_code & HASH_SET) {
        container_destroy(hash, CN_(cn)->eng);
    } else if (CN_(cn)->build_code & TREE_SET) {
        container_destroy(rb_tree, CN_(cn)->eng);
    }

    free(_class);
    free(CN_(cn));
    SET_(cn, NULL);
    return err_ok;
}

T_clazz* CN_ty_clazz(CN cn) 
{
    return CN_(cn)->type_clazz;
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
int CN_add_at(CN cn, It at, ...)
{
    va_list valist;
    va_start(valist, at);
    int err = __cn_add_at(cn, at, valist);
    va_end(valist);
    return err;
}
int CN_remove(CN cn, T* rdata)
{
    return __cn_remove_at(cn, CN_last(cn), rdata);
}

int CN_remove_at(CN cn, It at, T* rdata) 
{
    return __cn_remove_at(cn, at, rdata);
}

It CN_find(CN cn, ...) 
{
    va_list valist;
    va_start(valist, cn);
    It it = __cn_find_at(cn, ((CN_(cn)->build_code & HASH_SET || CN_(cn)->build_code &TREE_SET)?It_null:CN_first(cn)), valist);
    va_end(valist);
    return it;
}

It CN_find_at(CN cn, It at, ...) 
{
    va_list valist;
    va_start(valist, at);
    It it = __cn_find_at(cn, at, valist);
    va_end(valist);
    return it;
}

int CN_reverse(CN cn)
{
    CN_(cn)->is_forward = ! CN_(cn)->is_forward;
    return CN_(cn)->is_forward;
}

int CN_extract(CN cn, T* rdata, int (*cmp)(T*, T*))
{
    int err = err_ok; 
    if (CN_size(cn) > 0) { 
        cmp = cmp ? cmp : T_adapter_get(CN_(cn)->type_clazz->_def.ty_id, e_cmp);
        heap_build_max_heap(CN_(cn)->eng, cmp); 
        iterator_t first = container_first(CN_(cn)->eng);
        iterator_t last  = container_last(CN_(cn)->eng);
        iterator_exchange(first, last);
        err = container_remove(CN_(cn)->eng, container_last(CN_(cn)->eng), rdata);
    } else { 
        err = err_empty; 
    } 
    return err;
}

int CN_sort(CN cn, int (*cmp)(T*, T*))
{
    int err = err_ok;
    cmp = cmp ? cmp : T_adapter_get(CN_(cn)->type_clazz->_def.ty_id, e_cmp);
    if (CN_(cn)->build_code & LIST){
        quick_sort(container_first(CN_(cn)->eng), container_last(CN_(cn)->eng), cmp);
    } else if (CN_(cn)->build_code & VECTOR) {
        heap_sort(CN_(cn)->eng, cmp);
    } else {
        err = err_unsupported_method;
    }
    return err;
}

int CN_size(CN cn) 
{
    return container_size(CN_(cn)->eng);
}

int CN_has(CN cn, ...)
{
    va_list valist;
    va_start(valist, cn);
    It it = __cn_find_at(cn, ((CN_(cn)->build_code & HASH_SET || CN_(cn)->build_code &TREE_SET)?It_null:CN_first(cn)), valist);
    va_end(valist);
    int has = !It_is_tail(it);
    return has;
}

int CN_to_unique(CN cn)
{
    int err;
    int (*cmp)(T*, T*) = T_adapter_get(CN_(cn)->type_clazz->_def.ty_id, e_cmp);//0 ? 0 : CN_(cn)->eng->type_def.cmp;
    err = CN_sort(cn, cmp);
    if (err == err_ok) {
        if (CN_(cn)->build_code & use_entity)
            wring(CN_(cn)->eng, cmp, entity_release);
        else 
            wring(CN_(cn)->eng, cmp, NULL);
    }
    return err;
}

int CN_merge(CN cn1, CN cn2)
{
    int err = err_ok;
    T_clazz* clazz1 = CN_ty_clazz(cn1);
    T_clazz* clazz2 = CN_ty_clazz(cn2);

    if (clazz1->_def.ty_id == clazz1->_def.ty_id) {
        for (iterator_t first2=container_first(CN_(cn2)->eng); !iterator_equal(first2, container_tail(CN_(cn2)->eng)); iterator_next(first2))
        {
            container_insert(CN_(cn1)->eng, container_tail(CN_(cn1)->eng), first2.reference);
        }
    } else {
        err = err_merge;
    }
    return err;
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
            CN_READ_ENTITY_VARGS(cn, rm_keys, valist, ef_keys,0);
            iterator_t it = container_search(CN_(cn)->eng, __null_iterator, &rm_keys, NULL);
            if (!iterator_is_tail(it)) {                
                entity_t* rm;
                container_remove(CN_(cn)->eng, it, &rm);
                entity_release(rm);
            } else {
                err = err_no_found;
            }
            
        } else {
            CN_READ_SINGLE_VALUE_VARGS(cn, rm, valist, 0);
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
            CN_READ_ENTITY_VARGS(cn, ent, valist, ef_all, 1);
            container_insert(CN_(cn)->eng, __null_iterator, &ent);
        }  else {
            CN_READ_SINGLE_VALUE_VARGS(cn, tvalue, valist, 1);
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
            CN_READ_ENTITY_VARGS(cn, ent, valist, ef_keys, 0);
            iterator_t it = container_search(CN_(cn)->eng, __null_iterator, &ent, NULL);
            if (!iterator_is_tail(it)) {
                entity_t* ent = *((entity_t**)it.reference);
                // 
                if (ent->tpl->field_num - ent->tpl->value_idx == 1) {
                    return ent->block[ent->tpl->value_idx];
                } else {
                    return &ent->block[ent->tpl->value_idx];
                }
            }
            
        } else {
            CN_READ_SINGLE_VALUE_VARGS(cn, tv, valist, 0);
            iterator_t it = container_search(CN_(cn)->eng, __null_iterator, tv, NULL);
            if (!iterator_is_tail(it)) {
                return it.reference;
            }
        }

        va_end(valist);
    } 
    return NULL;
}