#include <math.h>
#include <string.h>
#include "counting.h"
#include "decision_tree.h"

/**
 * @brief 计算基尼纯度
 * 
 * @param _Xi 
 * @param _Y 
 * @return double 
 */
static double __calculate_gini(matrix2_t* label_on_val)
{
    void* counting = NULL;
    counting_Y(label_on_val, &counting);

    int  cty_size              = CTY_size(counting);
    int* elems_number_ptr  = CTY_elems_number_ptr(counting);

    int N = MAT2_ELEM_SIZE(label_on_val);
    double gini = 1.f;

    for (int i=0; i<cty_size; ++i) {
        double pro = (double)elems_number_ptr[i] / (double)N;
        gini -= (pro * pro);
    }
    free(counting);

    return gini;
}

/**
 * @brief 计算特征A的基尼系数
 * 
 * @param _Xi 
 * @param label 
 * @return double 
 */
static double __calculate_A_gini_index(matrix2_t* _Xi, matrix2_t* label) 
{

    void* counting = NULL;
    counting_Y(_Xi, &counting);

    int val_type_size       = CTY_size(counting);
    int val_type_number_ptr = CTY_elems_number_ptr(counting);

    int _Xi_size      = MAT2_ELEM_SIZE(_Xi);

    matrix2_t* label_on_vals[val_type_size];
    int        label_on_vals_index[val_type_size];

    memset(label_on_vals, 0x0, sizeof(label_on_vals));
    memset(label_on_vals_index, 0x0, sizeof(label_on_vals_index));


    for (int i=0; i<_Xi_size; ++i) {
        
        vfloat_t _Xi_val = _Xi->pool[i];
        
        int pos = counting_get_elem_pos(counting, _Xi_val);

        if (pos >= 0) {
            if (label_on_vals[pos] == NULL) {
                label_on_vals[pos] = Mat2_create(val_type_number_ptr[pos], 1);
            }

            int lov_index = label_on_vals_index[pos];
            label_on_vals[pos]->pool[lov_index] = label->pool[i];
            label_on_vals_index[pos]++;

        }
        

    }

    double GINI_INDEX = 0.f;

    for (int j=0; j<val_type_size; ++i) 
    {
        GINI_INDEX += ((double)val_type_number_ptr[j] / (double)_Xi_size) * __calculate_gini(label_on_vals[j]);

        Mat2_destroy(label_on_vals[j]);
    }

    // free matrix
    return GINI_INDEX;

}

/**
 * @brief 计算信息增益
 * 
 * @param data 
 * @param label 
 * @return double 
 */
static double __calculate_entropy(matrix2_t* label_on_val)
{
    void* counting = NULL;
    counting_Y(label_on_val, &counting);
    int cty_size             = CTY_size(counting);
    int cty_elems_number_ptr = CTY_elems_number_ptr(counting);
    int label_size           = MAT2_ELEM_SIZE(label_on_val);

    double entropy = 0.f;

    for (int i =0; i<cty_size; ++i) {
        double pro = (double)cty_elems_number_ptr[i] / (double)label_size;
        entropy += -1 * pro * log2(pro);
    }
    free(counting);
    return entropy;
}

static double __calculate_A_gain(matrix2_t* _Xi, matrix2_t* label)
{
    void* counting = NULL;
    counting_Y(_Xi, &counting);

    int val_type_size        = CTY_size(counting);
    int* val_type_number_ptr = CTY_elems_number_ptr(counting);

    int _Xi_size = MAT2_ELEM_SIZE(_Xi);

    matrix2_t* label_on_vals[val_type_size];
    int        label_on_vals_index[val_type_size];

    memset(label_on_vals, 0x0, sizeof(label_on_vals) );  
    memset(label_on_vals_index, 0x0, sizeof(label_on_vals_index) );

    for (int i=0; i<_Xi_size; ++i) {

        vfloat_t _Xi_val = _Xi->pool[i];

        int pos = counting_get_elem_pos(counting, _Xi_val);
        if (pos >= 0) {
            if (label_on_vals[pos] == NULL) {
                label_on_vals[pos] = Mat2_create(val_type_number_ptr[pos], 1);
            }

            int lov_index = label_on_vals_index[pos];
            label_on_vals[pos]->pool = label->pool[i];
            label_on_vals_index[pos]++;
        }
    }

    // 先算总的信息熵
    double GAIN = __calculate_entropy(label);

    for (int j=0; j<val_type_size; ++j) {

        GAIN -= ((double)val_type_number_ptr[j] / (double)_Xi_size) * __calculate_entropy(label_on_vals[j]);
        
        Mat2_destroy(label_on_vals[j]);
    }

    return GAIN;
}

static int __build_classification_tree(matrix2_t* data, matrix2_t* label, cart_node_t* _tree) 
{

    // TODO: 1 遍历每一个熟悉，找出最时候的的分点。
 
    matrix2_t* Xi = Mat2_create(1,1);

    for (int i=0; i<data->cols; ++i) {

        // 截取每一个属性
        Mat2_slice_col_to(Xi, data, i);
        
        
        

        


    }

}

/**
 * @brief 使用 CART 算法训练决策树。 
 * 
 * @param data 训练数据
 * @param label 数据标签
 * @param _tree 输出的决策树的根节点。
 * @return int 返回结果。
 */
int decision_tree_classification_train(matrix2_t* data, matrix2_t* label, cart_node_t** _tree)
{
    // TODO:
    // 1 计算每个种类的信息增益。
    // 2 用增益值最大的作为节点。

    void* Py_counting = NULL;

    
}
