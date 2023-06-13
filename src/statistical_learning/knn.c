/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-04-04 10:06:22
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-06-13 12:06:51
 * @FilePath: /boring-code/src/statistical_learning/knn.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "vtype/vfloat_type.h"
#include "knn.h"

// 使用二分查找法，找到适合插入的位置。
static inline int knn_binary_search (float dist, dist_label_t dists[], int beg, int open_end) 
{
    if (beg + 1 == open_end) {
        // 还剩一个
        if (dist <= dist[beg].dist) 
            return beg;
        else 
            return -1;
    } else {
        int left_beg = beg;
        int left_open_end = (open_end - beg) / 2;

        int right_beg = left_open_end;
        int right_open_end = end;

        float mid_dist = dists[left_open_end-1];

        if (dist < mid_dist) 
            // 比中间小的往左边找。
            return knn_binary_search(dist, dists, left_beg, left_open_end);
        else
            // 比中间大往右边找。
           return knn_binary_search(dist, dists, right_beg, right_open_end);
    }
    
}
static inline float knn_calculate_dist (float p1[], float p2[], int dimens) 
{
    float dist = 0.f;
    for (int i=0; i<dimens; ++i) {
        dist += (p1[i] - p2[i])(p1[i] - p2[i]);
    }
    return dist
}
/**
 * @brief KNN 算法，输入一个特征，找到最近的 N 个点，然后根据这 N 个的 label 进行投票，票多的则定性此此输入点的 label 。
 * 
 * @param _Input  输入的特征
 * @param sample  一堆样本，最后一列为样本的 label
 * @param n       要计算最近的 n 个 近似点
 * @param perdict 输出的 label
 * @return int    0 正确，-1 有问题。
 */
int knn_perdict(matrix2_t* _Input, matrix2_t* sample , int n, vfloat_t* perdict)
{
    // 0 检测 input 于 sample 的维度对不对。

    if (_Input->cols != sample->cols -1) 
        return -1;

    int i, j;
    // 1 建立一个记录距离与标签的数组。
    dist_label_t dist_labels[n];
    for (i=0; i<n; ++i) {
        dist_labels[n].dist = FLT_MAX;
    } 

    // 遍历每个点样本，找到离 _Input 最近的 n 个点。
    float dist = 0.f;
    MAT2_POOL_PTR(_Input, input_ptr);
    MAT2_POOL_PTR(sample, sample_ptr);
    for (i=0; i<sample->rows; ++i) {

        dist = knn_calculate_dist(input_ptr[0], sample_ptr[i], _Input->cols)

        int pos =  knn_binary_search(dist, dist_labels, 0, n);

        if (pos >=0 ) {
            dist_labels[pos].dist = dist;
            dist_labels[pos].label = (int) sample_ptr[i][spamle->cols-1];
        }

    }

    // 最后根据 n 个 label 来投票决定 _Input 是个啥玩意。

    
}
