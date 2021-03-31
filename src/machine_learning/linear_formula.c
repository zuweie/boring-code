/*
 * @Author: your name
 * @Date: 2021-03-23 11:10:10
 * @LastEditTime: 2021-03-28 18:53:10
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/linear_formula.c
 */
#include <stdarg.h>
#include <stdlib.h>
#include "matrix/DenseMatrix.h"
#include "linear_formula.h"

linear_formula_t Linear_formula_create_with_coe_dots(double _Y, unsigned int coe_n,  ...)
{
    double* coes = malloc(coe_n * sizeof(double));
    va_list valist;
    va_start(valist, coe_n);
    for (unsigned int i=0; i<coe_n; ++i) {
        coes[i] = va_arg(valist, double);
    }
    va_end(valist);

    linear_formula_t formula = {
        .coefficient_n = coe_n,
        .coefficients = coes,
        .Y = _Y
    };
    return formula;
}

linear_formula_t Linear_formula_create_with_coe_arr(double _Y, unsigned int coe_n, double* coes)
{
    double* coes = malloc( coe_n * sizeof(double) );
    linear_formula_t formula = {
        .coefficient_n = coe_n,
        .coefficients = coes,
        .Y = _Y
    };
    return formula;
}

int Linear_formula_solve(linear_formula_t *formulas[], int formula_n, double X[])
{
    // 第一步 找出最大的那多参数的那个式子。
    // 第二步 把参数都抽出来放入矩阵，组成增广矩阵。
    // 第三步 矩阵进行初等变换，求出所有的解。

    if (max_coes == formula_n) {

        // 把参数装进矩阵，然后准备做
        double (*coe_matrix)[max_coes + 1] = malloc ( (max_coes +1) * formula_n * sizeof (double) );
        for (int j=0; j<formula_n; ++j) {
            linear_formula_t* f = formulas[i];
            for (unsigned k=0; k<max_coes; ++k){
                if (k<f->coefficient_n)
                    coe_matrix[j][k] = f->coefficient[k];
                else 
                    coe_matrix[j][k] = 0.f;
            }
            coe_matrix[max_coes] = f->Y;
        }

        DenseMatrix* augmented_matrix = DenseMatrix_wrap(formula_n, max_coes+1, coe_matrix);

        DenseMatrix_lu(augmented_matrix);
        
        double X[max_co]
        
        DenseMatrix_destroy(augmented_matrix);

    }

    return ret;
}

void Linear_formula_destroy(linear_formula_t* formula)
{
    formula->coefficient_n = 0;
    free(formula->coefficients);
    return;
}
