/*
 * @Author: your name
 * @Date: 2022-01-17 11:00:34
 * @LastEditTime: 2022-02-03 14:22:29
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /boring-code/src/machine_learning/desicion_tree.c
 */
#include "ultra_array/ultra_array.h"
#include "decision_tree.h"

int find_best_split(u_array_t* _X, int* attr_list) {
     
}

int cart_training(u_array_t* X, u_array_t* Y, car_tree_t* tree, unsigned char is_classifier)
{
    // TODO:
    // 1 检查维数是否正确。
    if (UA_shape_axis(X, 0) != UA_length(Y)) return -1;
    int i, j, k;
    int attr_list_num = UA_shape_axis(X, 1);
    int attr_list[attr_list_num];
    // 
    for (i=0; i<attr_list_num; ++i) {
        attr_list[i] = i;
    }

    int best_split_id = find_best_split(X, attr_list);

    return 0;
}
int cart_finalize(car_tree_t* tree);
int cart_predict(u_array_t* sample, car_tree_t* tree);