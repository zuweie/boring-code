#include <stdlib.h>
#include <stdio.h>
#include "container/cn.h"
#include "container/entity.h"
#include "naive_bayes.h"

 
static int label_cmp_func(const void* a, const void* b) {
    return ( *(int*)a - *(int*)b );
}
/**
 * @brief 做label统计以及label下feature的统计
 * 
 * @param train_mat 训练集
 * @param train_label_mat 训练集的 label
 * @param navie_bayes_statistics_table_t 
 * @return int 0 ok -1 不ok
 */
int navie_bayes_statistics(matrix2_t* train_mat, matrix2_t* train_label_mat, navie_bayes_statistics_table_t* table)
{

    // 创建一个 hash_map, 可以放入三个数，
    // 第一个 label 作为 key
    // 第二个 label_count 记录 label 的数量

    CN hash_map = CN_create(HASH_MAP, int_t, int_t);

    // 统计各种 label 的概率

    for (int i=0; i<train_label_mat->rows; ++i) {

        int label = (int)train_label_mat->pool[i];

        if (!CN_has(hash_map, label)) {
            
            CN_set(hash_map, label, 1);

        } else {
            int* count = CN_get(hash_map, label);
            (*count) ++;
        }

    }

    table->label_number  = CN_size(hash_map);
    table->label_archive = malloc(table->label_number * 2 * sizeof(int));


    // 此处将 label 取出，放入熟组。
    int label_index=0;

    // 把指针的形状塑好，避免会有内存问题。 2 * n 的二维数组。
    int (*label_archive_table_ptr)[table->label_number] = table->label_archive;

    for (It first=CN_first(hash_map); !It_equal(first, CN_tail(hash_map)); It_next(first)) {
        entity_t* ent = It_ptr(first);
        label_archive_table_ptr[0][label_index] = ef_int(ent, ef_key);
        label_index ++;
    }

    // 给label排个序，后续比较好找到label。
    qsort(label_archive_table_ptr[0], table->label_number, sizeof(int), label_cmp_func);

    for (int j=0; j<table->label_number; ++j) {
        int label = label_archive_table_ptr[0][j];
        int* label_count = CN_get(hash_map, label);
        label_archive_table_ptr[1][j] = *label_count;
    }

    // 做 label feature 的统计。
    int max_feature_value_number = 0;
    MAT2_POOL_PTR(train_mat, train_mat_ptr);
    CN arr = CN_create(VECTOR, int_t);
    
    for (int j=0; j<train_mat->cols; ++j) {
        for (int i=0; i<train_mat->cols; ++i) {

            train_mat_ptr[i][j];

        }
    }
    
    CN_finalize(hash_map, NULL);
}

int navie_bayes_release_statistics_table(navie_bayes_statistics_table_t* table) {

    if (table->label_archive) free(table->label_archive);
    if (table->label_feature_archive) free(table->label_feature_archive);

    return 0;
}

int navie_bayes_inspect_statistics_table(navie_bayes_statistics_table_t* table) {

    printf("\n inspect table: \n");
    printf("-- **** label archive *** --\n");

    int (*label_archive_table_ptr)[2] = table->label_archive;

    for (int i=0; i<2; ++i) {
        for (int j=0; j<table->label_number; ++j) {
            printf("%d ", label_archive_table_ptr[i][j]);
        }
        printf("\n");
    }
}