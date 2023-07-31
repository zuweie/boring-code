#include <math.h>
#include <string.h>
#include <float.h>
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
            label_on_vals[pos]->pool[i] = label->pool[i];
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

/**
 * @brief 尝试找出最佳的分离属性
 * 
 * @param data 
 * @param label 
 * @param candidate_node 剩下可以选择的node
 * @param candidate_node_size 
 * @return int 
 */
static int __find_best_split(matrix2_t* data, matrix2_t* label, int* candidate_nodes, int* candidate_node_size, int* out_index, float* out_gain)
{


    matrix2_t* _Xi = Mat2_create(1,1);

    float gains[*candidate_node_size];

    for(int i=0; i<*candidate_node_size; ++i) {
        int index = candidate_nodes[i];

        Mat2_slice_col_to(data, _Xi, index);

        gains[i] = __calculate_A_gain(_Xi, label);
    }

    
    // 找到最大的哪个信息增益的 index
    int max_index  = 0;
    float max_gain = gains[0];

    for (int j=1; j<*candidate_node_size; ++j) {
        if (max_gain < gains[j]) {
            max_gain = gains[j];
            max_index = j;
        }
    }

    // 找到最大那个 index 后把那个 index 从 candidate_nodes 中删除。

    for(int i=max_index+1; i<*candidate_node_size; ++i) {
        candidate_nodes[i-1] = candidate_nodes[i];
    }
    (*candidate_node_size)--;

    *out_index = max_index;
    *out_gain  = max_gain;
    Mat2_destroy(_Xi);

    return 0;
}
static int __build_classification_leaf(cart_node_t** node_ref, vfloat_t _predict) 
{
    *node_ref = malloc(sizeof(cart_node_t));
    (*node_ref)->sub_counting = NULL;
    (*node_ref)->sub_nodes    = NULL;
    (*node_ref)->_predict     = _predict;
    (*node_ref)->_xi          = -1;
    return 0;
}
/**
 * @brief 递归建造决策树
 * 
 * @param data 
 * @param label 
 * @param node 
 * @param candidate_nodes 
 * @param candidate_node_size 
 * @param esp 
 * @return int 
 */
static int __build_classification_node(matrix2_t* data, matrix2_t* label, cart_node_t** node_ref, int* candidate_nodes, int* candidate_node_size, double esp_gain, int least_data) 
{
    // 若没有任何属性可以分割聊，或者剩下的数据量小于最少的数据量，例如小于10条数据，
    // 那么就直检测 label 的类别，选最多的那个类别当作叶子节点的值。

    void* label_counting = NULL;
    counting_Y(label, &label_counting);

    if (*candidate_node_size == 0 
    || data->rows <= least_data
    || CTY_size(label_counting) == 1) {

        // *node_ref = malloc(sizeof(cart_node_t));
        // (*node_ref)->sub_counting = NULL;
        // (*node_ref)->sub_nodes = NULL;
        // (*node_ref)->_predict = counting_get_most_elem(counting);
        // (*node_ref)->_xi = -1;
        //return 0;
        __build_classification_leaf(node_ref, counting_get_most_elem(label_counting));
    } else {

        // 测试每个属性，或者最大信息增益的属性组成节点，分割数据，然后迭代到下一个节点去。
        int out_index;
        float out_gain;
        __find_best_split(data, label, candidate_nodes, candidate_node_size, &out_index, &out_gain);

        if (out_gain < esp_gain) {

            // 最大的 gain 也小于 esp, 那么马上将其变成叶子节点。
            __build_classification_leaf(node_ref, counting_get_most_elem(label_counting));

        } else {

            // 分割数据。然后递归调用此函数。
           
            matrix2_t* _Xi = Mat2_create(1,1);
            Mat2_slice_col_to(_Xi, data, out_index);


            void* xi_data_counting = NULL;
            counting_Y(data, xi_data_counting);

            matrix2_t* sub_datas[CTY_size(xi_data_counting)];
            matrix2_t* sub_labels[CTY_size(xi_data_counting)];
            int sub_index[CTY_size(xi_data_counting)];

            memset(sub_datas, 0x0, CTY_size(xi_data_counting) * sizeof(matrix2_t*));
            memset(sub_labels, 0x0, CTY_size(xi_data_counting) * sizeof(matrix2_t*));

            MAT2_POOL_PTR(data, data_ptr);

            for(int i=0; i<data->rows; ++i) {

                vfloat_t target = data_ptr[i][out_index];
                int pos = counting_get_elem_pos(xi_data_counting, target);

                // 若没有这个矩阵，
                if (sub_datas[pos] == NULL)   
                    sub_datas[pos]  = Mat2_create(CTY_elems_number_ptr(xi_data_counting)[pos], data->cols);

                if (sub_labels[pos] == NULL)  
                    sub_labels[pos] = Mat2_create(CTY_elems_number_ptr(xi_data_counting)[pos], label->cols);



                
            }


            // cart_node_t** sub_nodes = malloc(CTY_size(data_counting) * sizeof(cart_node_t*));
            // memset(sub_nodes, 0x0, CTY_size(data_counting) * sizeof(cart_node_t*));

            // *node_ref = malloc(sizeof(cart_node_t));
            // (*node_ref)->sub_counting = data_counting;
            // (*node_ref)->sub_nodes = sub_nodes;
            // (*node_ref)->_xi = out_index;

            

        }
        
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
