/*
 * @Author: your name
 * @Date: 2021-10-21 11:58:55
 * @LastEditTime: 2021-10-21 16:26:11
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/cn.c
 */
#include <stdlib.h>
#include <stdarg.h>
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
    entity_template_t* etpl = NULL;
    container_t* container   = NULL;
    cn_t* cn_ptr             = NULL;
    int i;

    va_list valist;
    var_start(valist, build_code);

    if (cn) {
        void* etpl = NULL;
        void* cmp_func = NULL;
        void* hash_func = NULL;

        if (build_code & use_entity) {
            if (build_code & customized_entity) {
                int filed_num = va_arg(valist, int);
                if (filed_num > MAX_ENT_FIELD_NUM) {
                    err = err_out_of_max_entity_field;
                    goto BUILD_FAIL;
                }
            }
        } else {

        }

    } else {
        err = err_out_of_capcity;
    }
    BUILD_FAIL:
    va_end(valist);
    if (err != err_ok) {
        if (etpl) free(etpl);
        if (cn_ptr) free(cn_ptr);
        return err;
    } else {
        SET_(cn, cn_ptr);
        return cn;
    }
}
CN CN_finalize(CN cn, int(*cleanup)(T*));