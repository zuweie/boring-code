/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-06-19 16:32:24
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-06-20 14:27:00
 * @FilePath: /boring-code/src/statistical_learning/matrix2_count.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "matrix2_count.h"

#define SPARE_ARR_INC_SIZE 4
#define COUNT_DIFF_SIZE_PTR(diff_ptr) ((int*)(diff_ptr))
#define COUNT_DIFF_LIST_PTR(diff_ptr) ((vfloat_t*)( &(COUNT_DIFF_SIZE_PTR(diff_ptr)[1])))
#define COUNT_DIFF_NUMBERS_PTR(diff_ptr)  ((int*)(&(COUNT_DIFF_LIST_PTR(diff_ptr)[*COUNT_DIFF_SIZE_PTR(diff_ptr)])))

static int binary_search(vfloat_t* arr, vfloat_t target, int begin, int open_end) 
{
    if (begin == open_end-1) {
        if (target < arr[begin]) {
            return begin;
        } else {
            return -1;
        }

    } else {

        int left_beg = begin;
        int left_open_end = left_beg +  (open_end - begin) / 2;

        int right_beg = left_open_end;
        int right_open_end = open_end;

        vfloat_t mid_value = arr[left_open_end-1];

        if (target < mid_value) {
            return binary_search(arr, target, left_beg, left_open_end);
        } else if (target > mid_value) {
            return binary_search(arr, target, right_beg, right_open_end);
        } else {
            // 相等
            return -1;
        }
    }
}

static int binary_search_insert(vfloat_t* arr, vfloat_t target, int begin, int open_end) 
{
    int pos = binary_search(arr, target, begin, open_end);

    if (pos >=0) {
        // 移动位置。
        for (int i=open_end-2; i>=pos; --i) {
            arr[i+1] = arr[i];
        }
        
        arr[pos] = target;
    }
    return pos;
}

/**
 * @brief 把一列中不同数值(只取一次)出并且排序。
 * 
 * @param mat 输入矩阵
 * @param col 要统计的列
 * @param out 输出结果,其内存布置如下：
 *            {int,float, float, float, float,...., int, int, int, int,}
 *            其中第一个int，为输出的不同值的个数。float...，为不同值的数值，已经经过排序。
 *            最后 int...，为每个不同值的数量。
 * @param out 输出结果的数量
 * @return int 
 */
int __mat2_list_different(vfloat_t* in,  int in_size,  void** diff)
{
    // 先申请内存。
    // 最终内存模型如下
    // {int,float, float, float, float,...., int, int, int, int,}
    void* output = (vfloat_t*) malloc(sizeof(int)+SPARE_ARR_INC_SIZE * sizeof(vfloat_t));
    // 把 pool size 置于刚刚申请的内存大小。
    int pool_size = SPARE_ARR_INC_SIZE;
    // 把申请了的内存初始化最大的 float 点, 为了做二分查找插入。
    
    int* size_ptr     = COUNT_DIFF_SIZE_PTR(output);
    vfloat_t* arr_ptr = COUNT_DIFF_LIST_PTR(output);
    
    for (int i=0; i<pool_size; ++i) {

        arr_ptr[i] = FLT_MAX;
    }
    *size_ptr = 0;

    for (int i=0; i<in_size; ++i) {
        
        // 在二分查找插入前会确保 数组足够大。
        if (*size_ptr == pool_size) {

            // 满了需要添加内存
            output = (vfloat_t*)realloc(output, sizeof(int) + (pool_size+=SPARE_ARR_INC_SIZE) * sizeof(vfloat_t));
            
            // 重新申请内存后要及时更新 arr 以及 size 的地址。因为 realloc 后首地址可能会改变。
            size_ptr = COUNT_DIFF_SIZE_PTR(output);
            arr_ptr  = COUNT_DIFF_LIST_PTR(output);

            // 添加内存后，把它初始化为 max float。
            for (int j = *size_ptr; j<pool_size; ++j) {
                arr_ptr[j] = FLT_MAX;
            }
        }
        
        // 二分查找并且插入。成功返回插入位置。否则返回 -1
        int pos = binary_search_insert(arr_ptr, in[i], 0, pool_size);

        // 成功插入数值加一
        if (pos >= 0) (*size_ptr)++;
    }

    
    // 再次扩大内存放入每个值对应的 numbers 
    
    output = realloc(output, sizeof(int) + (*size_ptr) * sizeof(vfloat_t) + (*size_ptr) * sizeof(int));

    size_ptr    = COUNT_DIFF_SIZE_PTR(output);
    arr_ptr     = COUNT_DIFF_LIST_PTR(output);
    numbers_ptr = COUNT_DIFF_NUMBERS_PTR(output);

    memset(number_ptr, 0x0, sizeof(int) * (*size_ptr));

    for (int i=0; i<in_size; ++i) {
        vfoat_t target = in[i];
        int pos        = binary_search(arr_ptr, target, 0, *size_ptr);
        
        numbers_ptr[pos]++;
    }

    // 搞完收工。
    *diff = output;
    return 0;
}

/**
 * @brief 根据值来找到相应的数量
 * 
 * @param diff 
 * @param target 
 * @return int 
 */
int __mat2_get_diff_number(void* diff, vfloat_t target) 
{

    int* size_ptr   = COUNT_DIFF_SIZE_PTR(diff);
    int* diff_ptr   = COUNT_DIFF_LIST_PTR(diff);
    int* number_ptr = COUNT_DIFF_NUMBERS_PTR(diff);

    int pos = binary_search(diff_ptr, target, 0, (*size_ptr));

    if (pos >=0) return number_ptr[pos];

    return 0;

}
