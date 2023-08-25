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
    int* val_type_number_ptr = CTY_elems_number_ptr(counting);

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

    for (int j=0; j<val_type_size; ++j) 
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
    int* cty_elems_number_ptr = CTY_elems_number_ptr(counting);
    int label_size           = MAT2_ELEM_SIZE(label_on_val);

    double entropy = 0.f;

    for (int i =0; i<cty_size; ++i) {
        double pro = (double)cty_elems_number_ptr[i] / (double)label_size;
        entropy += -1 * pro * log2(pro);
    }
    free(counting);
    return entropy;
}

static double __calculate_A_gain(matrix2_t* _Xj, matrix2_t* label)
{
    void* counting = NULL;
    counting_Y(_Xj, &counting);

    int val_type_size        = CTY_size(counting);
    int* val_type_number_ptr = CTY_elems_number_ptr(counting);

    int _Xj_size = MAT2_ELEM_SIZE(_Xj);

    matrix2_t* label_on_vals[val_type_size];
    int        label_on_vals_index[val_type_size];

    memset(label_on_vals, 0x0, sizeof(label_on_vals) );  
    memset(label_on_vals_index, 0x0, sizeof(label_on_vals_index) );

    for (int i=0; i<_Xj_size; ++i) {

        vfloat_t _Xj_val = _Xj->pool[i];

        int pos = counting_get_elem_pos(counting, _Xj_val);
        if (pos >= 0) {
            if (label_on_vals[pos] == NULL) {
                label_on_vals[pos] = Mat2_create(val_type_number_ptr[pos], 1);
            }

            int lov_index = label_on_vals_index[pos];
            label_on_vals[pos]->pool[lov_index] = label->pool[i];
            label_on_vals_index[pos]++;
        }
    }

    // 先算总的信息熵
    double GAIN = __calculate_entropy(label);

    for (int j=0; j<val_type_size; ++j) {

        GAIN -= ((double)val_type_number_ptr[j] / (double)_Xj_size) * __calculate_entropy(label_on_vals[j]);
        
        Mat2_destroy(label_on_vals[j]);
    }

    return GAIN;
}

/**
 * @brief 尝试找出最佳的分离属性, 非常关键的函数
 * 
 * @param data 
 * @param label 
 * @param candidate_node 剩下可以选择的node
 * @param candidate_node_size 
 * @return int 
 */
static int __find_best_split(matrix2_t* sub_data, matrix2_t* sub_label, int* Ags, int* Ags_size, double esp, int* out_Ag, double* out_gain)
{
    matrix2_t* _Xj = Mat2_create(1,1);

    double A_gain   = FLT_MIN;
    int    Ag       = 0;
    int    Ag_index = 0;

    for(int i=0; i<*Ags_size; ++i) {
        int index = Ags[i];

        Mat2_slice_col_to(_Xj, sub_data, index);

        double gain = __calculate_A_gain(_Xj, sub_label);

        if (A_gain < gain)  {
            A_gain   = gain;
            Ag       = index;
            Ag_index = i;
        }

    }

    Mat2_destroy(_Xj);

    if (A_gain > esp) {

        // 找到最佳的 Ag 后，把它的 index 从 Ags 数组中删除。

        *out_Ag   = Ag;
        *out_gain = A_gain;

        // 把这个天选的 Ag 删除，在后续的
        for(int i=Ag_index+1; i<*Ags_size; ++i) {
            Ags[i-1] = Ags[i];
        }
        (*Ags_size)--;

        return 0;

    } 
    return -1;

}
static int __build_classification_leaf(dct_node_t** node_ref, vfloat_t _predict) 
{
    *node_ref = malloc(sizeof(dct_node_t));
    (*node_ref)->router      = NULL;
    (*node_ref)->sub_nodes   = NULL;
    (*node_ref)->_predict    = _predict;
    (*node_ref)->attr_index  = -1;
    return 0;
}

static int __is_leaf(dct_node_t* node) 
{
    return node->sub_nodes == NULL || node->router == NULL || node->attr_index == -1;
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
static int __build_classification_node(matrix2_t* data, matrix2_t* label,  dct_node_t** node_ref, int* Ags, int Ags_size, int level_label, int* tree_level, double gain_esp, int least_limit, int level_limit, void (*progress)(char*, unsigned long, unsigned long)) 
{
    // 若没有任何属性可以分割聊，或者剩下的数据量小于最少的数据量，例如小于10条数据，
    // 那么就直检测 label 的类别，选最多的那个类别当作叶子节点的值。

    if (progress) progress("248.创建节点", Ags_size, data->rows);

    void* label_counting = NULL;

    counting_Y(label, &label_counting);

    if (*tree_level < level_label) 
        *tree_level = level_label;

    /* 以下条件结束生成：
        1 首选节点为空，
        2 数据量小于最少的数据要求。
        3 树的层数大于最大限制。
    */
    if (Ags_size   == 0 
    || data->rows  <= least_limit
    || level_label >= level_limit
    || CTY_size(label_counting) == 1) {

        /** 满足建造叶子的条件马上建造叶子 */
        vfloat_t predict = counting_max_frequency(label_counting);

        //if (progress) progress("261.创建叶节点...", Ags_size, predict);  

        __build_classification_leaf(node_ref, predict);

    } else {

        // 测试每个属性，或者最大信息增益的属性组成节点，分割数据，然后迭代到下一个节点去。
        int   out_Ag;
        double out_gain;
        
        if (__find_best_split(data, label, Ags, &Ags_size, gain_esp,  &out_Ag, &out_gain) < 0) {

            // 最大的 gain 也小于 esp, 那么马上将其变成叶子节点。
            vfloat_t predict = counting_max_frequency(label_counting);

            //if (progress) progress("276.创建叶节点...", Ags_size, predict);  

            __build_classification_leaf(node_ref, predict);

        } else {

            // 分割数据。然后递归调用此函数。
            void* router = NULL;
            matrix2_t* _Xj = Mat2_create(1,1);
            Mat2_slice_col_to(_Xj, data, out_Ag);

            counting_Y(_Xj, &router);

            int group_size;
            matrix2_t** group_x;
            matrix2_t** group_y;

            // TODO: 做一个优化，当选取出来的东东只有一个 group，则屏蔽这个 Ag 选择下一个 Ag。
            counting_XY_group_by_x(data, label, out_Ag, &group_x, &group_y, &group_size);

            //if (progress) progress("295.创建节点中...", Ags_size, data->rows);   

            *node_ref = malloc(sizeof(dct_node_t)); 
            // 多申请一个空位用于做遍历的止点   
            (*node_ref)->sub_nodes  = malloc((group_size+1) * sizeof(dct_node_t*));
            (*node_ref)->_predict   = 0.f;
            (*node_ref)->router     = router;
            (*node_ref)->attr_index = out_Ag;
            memset((*node_ref)->sub_nodes, 0x0, (group_size+1) * sizeof(dct_node_t*));

            // 此处要建立多个 Ags 数组，以供各个子节点分拣数据使用
            //int Ags_group[group_size][Ags_size];
            int (*Ags_group)[Ags_size] = malloc(group_size * Ags_size * sizeof(int));

            // 可用得 Ag 信息 copy 到 Ags 数组中。
            for (int i=0; i<group_size; ++i) {
                memcpy(Ags_group[i], Ags, Ags_size * sizeof(Ags[0]));
            }

            for (int i=0; i<group_size; ++i) {
                matrix2_t* sub_data  = group_x[i];
                matrix2_t* sub_label = group_y[i];

                __build_classification_node( \
                    sub_data, sub_label, \
                    &((*node_ref)->sub_nodes[i]), \
                    Ags_group[i], \
                    Ags_size, \
                    (level_label+1), \
                    tree_level,  \
                    gain_esp, \
                    least_limit, \
                    level_limit, \
                    progress \
                );

                // 用完了就释放矩阵。
                Mat2_destroy(sub_data);
                Mat2_destroy(sub_label);
            }
            free(Ags_group);
            free(group_x);
            free(group_y);
        }
    }
    free(label_counting);
    return 0;
}

static int __do_classification_predict(matrix2_t* _X, dct_node_t* _tree, vfloat_t* predict) 
{
    if (__is_leaf(_tree)) {
        *predict = _tree->_predict;
    } else {

        vfloat_t target = _X->pool[_tree->attr_index];
        int router = counting_get_elem_pos(_tree->router, target);
        
        if(router == -1) {
            // 要是缺失了这个属性，使用多数表决法：找到某一个属性值最多的那个破玩意儿。把这个缺失的属性当作这个属性。

            target = counting_max_frequency(_tree->router);
            router = counting_get_elem_pos(_tree->router, target);
        }

        return __do_classification_predict(_X, _tree->sub_nodes[router], predict);

    }

    return 0;
}


static int __do_release_node(dct_node_t* _tree)
{
    if (!__is_leaf(_tree) ){
        free(_tree->router);
        for (int i=0;_tree->sub_nodes[i] != NULL; ++i) {
            __do_release_node(_tree->sub_nodes[i]);
        }
    }
    free(_tree);
    return 0;
}


/**
 * @brief 使用 CART 算法训练决策树。 
 * 
 * @param data 训练数据
 * @param label 数据标签
 * @param _tree 输出的决策树的根节点。
 * @return int 返回结果。
 */
int decision_tree_classification_train(matrix2_t* data, matrix2_t* label, dc_tree_t* tree, dct_term_t* term, void (*progress)(char*, unsigned long, unsigned long))
{
    // TODO:
    // 1 计算每个种类的信息增益。
    // 2 用增益值最大的作为节点。

    // 信息增益小于 1e-1 就结叶
    //double gain_esp   = 1e-1;
    // 数据量小于数据量 1/1000 否则
    //int    data_least = 100;

    //cart_node_t* root = NULL;

    int Ags[data->cols];

    for (int i=0; i<data->cols; ++i) 
        Ags[i] = i;

    int Ags_size = data->cols;

    tree->level     = 0;
    tree->root      = NULL;
    int level_label = 1;
    __build_classification_node(data, label, &(tree->root), Ags, Ags_size, level_label, &tree->level, term->gain_esp, term->least_limit, term->level_limit, progress);
    return 0;
    
}


/**
 * @brief 做预测
 * 
 * @param _x 
 * @param _tree 
 * @param predict 
 * @return int 
 */
int decision_tree_classification_predict(matrix2_t* _X, dc_tree_t* _tree, vfloat_t* predict)
{
    return __do_classification_predict(_X, _tree->root, predict);
}

/**
 * @brief 释放 decision tree
 * 
 * @param _tree 
 * @return int 
 */
int decision_tree_release(dc_tree_t* _tree)
{
    return __do_release_node(_tree->root);
}

