/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-05-01 20:32:12
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-05-01 20:53:17
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_float.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "cg_ref.h"
#include "cg_elem_spec.h"
#include "cg_float.h"

static cg_elem_spec_t cg_float;
static cg_elem_spec_t* spec = NULL;

static int __float_add(cg_ref_t dest, cg_ref_t elem1, cg_ref_t elem2)
{
    float* res  = (float*) dest;
    float* opt1 = (float*) elem1;
    float* opt2 = (float*) elem2;
    *res = *opt1 + *opt2;

    return 0;
}

static int __float_subtract(cg_ref_t dest, cg_ref_t elem1, cg_ref_t elem2)
{
    float* res  = (float*) dest;
    float* opt1 = (float*) elem1;
    float* opt2 = (float*) elem2;
    *res = *opt1 - *opt2;
    return 0;
}

static int __float_mulitply(cg_ref_t dest, cg_ref_t elem1, cg_ref_t elem2)
{
    float* res  = (float*) dest;
    float* opt1 = (float*) opt1;
    float* opt2 = (float*) opt2;
    *res = *opt1 * *opt2;
    return 0;
}

static int __float_divide(cg_ref_t dest, cg_ref_t elem1, cg_ref_t elem2)
{
    float* res  = (float*) dest;
    float* opt1 = (float*) elem1;
    float* opt2 = (float*) elem2;
    *res = *opt1 / *opt2;
    return 0;
}

cg_elem_spec_t* cg_float_spec() 
{
    if (!spec) {
        spec = &cg_float;
        cg_elem_spec_init(spec, sizeof(float));
        cg_elem_spec_opt_register(spec, elem_opt_add,      __float_add);
        cg_elem_spec_opt_register(spec, elem_opt_subtract, __float_subtract);
        cg_elem_spec_opt_register(spec, elem_opt_multiply, __float_mulitply);
        cg_elem_spec_opt_register(spec, elem_opt_divide,   __float_divide);
    }
    return spec;
}