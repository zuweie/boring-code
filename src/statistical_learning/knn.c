/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-04-04 10:06:22
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-09 12:20:27
 * @FilePath: /boring-code/src/statistical_learning/knn.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "vtype/vfloat_type.h"
#include "container/Cont.h"
#include "container/Entity.h"
#include "knn.h"

// 使用二分查找法，找到适合插入的位置。
static inline int knn_binary_search (float dist, dist_label_t dist_labels[], int beg, int open_end) 
{
    if (beg == open_end - 1) {
        // 还剩一个
        if (dist < dist_labels[beg].dist) 
            return beg;
        else 
            return -1;
    } else {
        int left_beg = beg;
        int left_open_end = left_beg +  (open_end - beg) / 2;

        int right_beg = left_open_end;
        int right_open_end = open_end;

        // 左边最后一个。
        float mid_dist = dist_labels[left_open_end-1].dist;

        if (dist < mid_dist) 
            // 比中间小的往左边找。
            return knn_binary_search(dist, dist_labels, left_beg, left_open_end);
        else
            // 比中间大往右边找。
           return knn_binary_search(dist, dist_labels, right_beg, right_open_end);
    }
    
}
static inline float knn_calculate_dist (float p1[], float p2[], int dimens) 
{
    float dist = 0.f;
    for (int i=0; i<dimens; ++i) {
        dist += (p1[i] - p2[i]) * (p1[i] - p2[i]);
    }
    return dist;
}
/**
 * @brief KNN 算法，输入一个特征，找到最近的 N 个点，然后根据这 N 个的 label 进行投票，票多的则定性此此输入点的 label 。
 * 
 * @param _Input  输入的特征
 * @param sample  一堆样本，第一列为样本的 label
 * @param sample_label 一堆样本的标识
 * @param n       要计算最近的 n 个 近似点
 * @param perdict 输出的 label
 * @return int    0 正确，-1 有问题。
 */
int knn_predict(matrix2_t* _Input, matrix2_t* sample, matrix2_t* sample_label, int n, int* predict_label)
{
    // 0 检测 input 于 sample 的维度对不对。
    if (_Input->cols != sample->cols) 
        return -1;

    if (sample->rows != sample_label->rows)
        return -1;

    // 1 建立一个记录距离与标签的数组。
    dist_label_t dist_labels[n];
    for (int i=0; i<n; ++i) {
        dist_labels[i].dist = FLT_MAX;
    } 

    // 遍历每个点样本，找到离 _Input 最近的 n 个点。
    float dist = 0.f;
    MAT2_POOL_PTR(_Input, input_ptr);
    MAT2_POOL_PTR(sample, sample_ptr);
    MAT2_POOL_PTR(sample_label, sample_label_ptr);
    
    for (int i=0; i<sample->rows; ++i) {

        dist = knn_calculate_dist(input_ptr[0], sample_ptr[i], _Input->cols);

        int pos =  knn_binary_search(dist, dist_labels, 0, n);

        if (pos >=0 ) {
            
            // 挪位。
            for (int j=n-2; j>=pos; --j) {
                dist_labels[j+1] = dist_labels[j];
            }

            dist_labels[pos].dist = dist;
            dist_labels[pos].label = (int) sample_label_ptr[i][0];
        }

    }

    // 最后根据 n 个 label 来投票决定 _Input 是个啥玩意。
    // CN hash_map = CN_Create(HASH_MAP, int_t, int_t);
    CN hash_map = CN_create(HASH_MAP, int_t, int_t);
    for (int i=0; i<n; ++i) {

        int label = dist_labels[i].label;

        if (CN_has(hash_map, label)) {
            // 找到的话原地加一
            int* count = (int*) CN_get(hash_map, label);
            *count += 1;

        } else {
            
            CN_set(hash_map, label, 1);

        }
    }
    
    int max_label;
    int max_count = -1;
    for (Iter first = CN_first(hash_map); !It_equal(first, CN_tail(hash_map)); first=It_next(first)) {

        entity_t* ent = It_ptr(first);
        if (ef_int(ent, ef_value) > max_count) {
            
            max_count = ef_int(ent, ef_value);
            max_label = ef_int(ent, ef_key);

        }        

    }

    *predict_label = max_label;
    CN_finalize(hash_map, NULL);

    return 0;
}
