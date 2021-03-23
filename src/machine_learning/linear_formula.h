/*
 * @Author: your name
 * @Date: 2021-03-23 10:42:20
 * @LastEditTime: 2021-03-23 11:36:22
 * @LastEditors: Please set LastEditors
 * @Description: 线性方程
 * @FilePath: /boring-code/src/machine_learning/linear_formula.h
 */

#ifndef _LINEAR_FORMULA_H_
#define _LINEAR_FORMULA_H_

typedef struct _u_array u_array_t;

typedef struct _linear_formula {
    unsigned int coefficient_n;
    double* coefficients;
    double  Y;
} linear_formula_t;

linear_formula_t Linear_formula_create_with_coe_dots(double, unsigned int,  ...);
linear_formula_t Linear_formula_create_with_coe_arr(double, unsigned int, double*);
int Linear_formula_solve(linear_formula_t*[], int formula_n, u_array_t* solution);
void Linear_formula_destroy(linear_formula_t* formula);

#define _LinearFormula(_Y, coe_n, ...) Linear_formula_create_with_coe_dots(_Y, coe_n, __VA_ARGS__)
#define LinearFormula_(pformula) Linear_formula_destroy(pformula)

#endif

