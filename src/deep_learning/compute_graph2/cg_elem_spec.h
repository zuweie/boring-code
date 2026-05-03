/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-05-01 15:35:16
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-05-01 20:42:44
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_elem_spec.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_ELEM_SPEC_H__
#define __CG_ELEM_SPEC_H__
#include <string.h>
#include "cg_ref.h"

#define CG_ELEM_OPTS 32

#define CALL_ELEM_OPT(spec, opt, dest, elem1, elem2) ((spec)->opts[(opt)])(dest, elem1, elem2)

enum elem_opt {
    elem_opt_add = 0,
    elem_opt_subtract,
    elem_opt_multiply,
    elem_opt_divide
};

typedef struct cg_elem_spec {

    int eleme_size;
    int (*opts[CG_ELEM_OPTS])(cg_ref_t, cg_ref_t, cg_ref_t);

} cg_elem_spec_t;

static inline int cg_elem_spec_init(cg_elem_spec_t* elem_spec, int size)
{
    elem_spec->eleme_size = size;
    memset(elem_spec->opts, 0, sizeof(elem_spec->opts));
    return 0;
}

static inline int cg_elem_spec_opt_register(cg_elem_spec_t* elem_spec, elem_opt opt, int(*opt_imp)(cg_ref_t, cg_ref_t, cg_ref_t)) 
{
    if (opt < CG_ELEM_OPTS) {

        elem_spec->opts[opt] = opt_imp;
        return 0;
    } 
    return -1;
}

#endif