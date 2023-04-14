/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-04-04 10:06:22
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-04-13 16:52:24
 * @FilePath: /boring-code/src/machine_learning2/knn.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include "container/cn.h"
#include "knn.h"

/**
 * @brief 用于排序矩阵中元素，输入到 quick sort 中的数组是一个这样的数组 [{float,size_t}, {float, size_t}, ....], float 是矩阵中 element 的值，size_t 是他们在矩阵中的索引。
 * 
 * @param _a 是一个 float + size_t 大小的内存块。[float, size_t],使用 float 的值来排序
 * @param _b 是一个 float + size_t 大小的内存块。[float, size_t],使用 float 的值来排序
 * @return int 两个元素的大小。
 */
static int __cmp_mat_elem(const void* _a, const void* _b) 
{
    
}

/**
 * @brief 建立 Kd tree。
 * 
 * @param root 
 * @param parent 
 * @param hanging_node_indexs 
 * @param n_hanging_node 
 * @param _X 
 * @param x_dimens_forward 
 * @return int 
 */
static int __knn_do_building_tree(kd_tree_node_t** root, kd_tree_node_t* parent, int* hanging_node_indexs, int n_hanging_node, matrix2_t* _X, int x_dimens_forward)
{
    if (n_hanging_node > 0) {

        // 有东西需要挂在，于是开始整活。
        *root = malloc(sizeof(kd_tree_node_t));
        (*root)->parent = parent;
        (*root)->left   = NULL;
        (*root)->right  = NULL;

        if (n_hanging_node == 1) {
            /* 只剩一个赋值完毕，赶紧滚蛋。*/
            int col = x_dimens_forward % _X->cols;
            (*root)->x_index      = hanging_node_indexs[0];
            (*root)->middle_value = Mat2_get(_X, hanging_node_indexs[0], col);

        } else {
            
            struct sort_node {
                vfloat_t v;
                sizt_t   i;
            }

            struct sort_node * sort_place = malloc(n_hanging_node * sizeof(struct sort_node));
            int col = x_dimens_forward % _X->cols;
        
            for (int i=0; i<n_hanging_node; ++i) {

                // 填入需要排序的数据，然后使用 quick sort 开始排序。
                sort_place[i].v = Mat2_get(mat, hanging_node_indexs[i], col);
                sort_place[i].i = hanging_node_indexs[i];
            }

            qsort(sort_place, n_hanging_node, sizeof(struct sort_node), __cmp_mat_elem);
            free(sort_place);

            // 找出中位数， 与中间节点索引，赋值给 root。
            vfloat_t middle_value =  (n_hanging_node % 2)? sort_place[(n_hanging_node / 2) + 1].v : (sort_place[(n_hanging_node / 2)-1].v + sort_place[(n_hanging_node/2)].v)/2
            int      middle_index =  (n_hanging_node % 2)? sort_place[(n_hanging_node / 2) + 1].i : sort_place[(n_hanging_node / 2)].i;
            
            (*root)->x_index      = middle_index;
            (*root)->middle_value = middle_value;

            
            int left_indexs[middle_index];
            for (int j=0; j<middle_index; ++j) {
                left_indexs[i] = sort_place[i].i;
            }

            __knn_do_building_tree((*root)->left, (*root), left_indexs, middle_index, _X, ++x_dimens_forward);

            if (middle_index + 1 < n_hanging_node) {
                int right_indexs[n_hanging_node - middle -1];
                for (int k=middle+1; k<n_hanging_node; ++k) {
                    right_indexs[k] = sort_place[k].i;
                }
                __knn_do_building_tree((*root)->right, (*root), right_indexs, (n_hanging_node-middle_index-1), _X, ++x_dimens_forward);
            }

        }
    }
    return 0;
}

kd_tree_node_t* knn_build_kd_tree(matrix2_t* _X)
{
    /**
     * 算法过程：
     * 1 循环第对 i 维数据进行排序，
     * 2 排序后取中间的那一位挂到树上。
     * 3 剩下的比这个节点小的，挂在这个节点的左子树，比这个节点大的挂在右子树。
     * 4 当所有的节点挂完，结束战斗。
     */

    kd_tree_node_t* root;

    int handing_node_indexes[_X->rows];
    for (int i=0; i<_X->rows; ++i) {
        handing_node_indexes[i] = i;
    }

    __knn_do_building_tree(&root, NULL, handing_node_indexes, _X_rows, _X, 0);

    return root;
    
}

int knn_destroy_kd_tree(kd_tree_node_t* root)
{
    if (root->left) knn_destroy_kd_tree(root->left);
    if (root->right) knn_destroy_kd_tree(root->right);

    free(root);
    return 0;
}

int knn_perdict(matrix2_t* _Input, int n_near_by, vfloat_t* perdict)
{
    int near_elems[n_near_by];
    matrix2_t* _X = Mat2_create(1,1);
    Mat2_slice_cols_to(_X, _Input, 0, _Input->cols);
    kd_tree_node_t* root = knn_build_kd_tree()
}