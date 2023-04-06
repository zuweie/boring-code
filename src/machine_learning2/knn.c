#include <stdlib.h>
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

kd_tree_node_t* knn_build_kd_tree(matrix2_t* _X)
{
    kd_tree_node_t* node = malloc(sizeof(kd_tree_node_t));

    for (int i=0; i<_X->rows; ++i) {
        
    }

    return node;
}

int knn_destroy_kd_tree(kd_tree_node_t* tree)
{

}

int knn_perdict(matrix2_t* _Input, int n_near_by, vfloat_t* perdict)
{

}