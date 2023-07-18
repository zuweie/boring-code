#include "counting.h"

#define SPARE_ARR_INC_SIZE 64

/**
 * @brief 
 * 
 * @param arr 
 * @param target 
 * @param begin 
 * @param open_end 
 * @param new_place 
 * @return int 
 */
static int binary_search(vfloat_t* arr, vfloat_t target, int begin, int open_end, int new_place) 
{
    if (open_end == begin) return -1;
    
    if (begin == open_end-1) {
        if (new_place) {
            if (target < arr[begin]) {
                return begin;
            } else {
                return -1;
            }
        }else if (target == arr[begin]){
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
            return binary_search(arr, target, left_beg, left_open_end, new_place);
        } else if (target > mid_value) {
            return binary_search(arr, target, right_beg, right_open_end, new_place);
        } else {
            // 相等
            return new_place? -1:left_open_end-1;
        }
    }
}

static int binary_search_insert(vfloat_t* arr, vfloat_t target, int begin, int open_end) 
{
    int pos = binary_search(arr, target, begin, open_end, 1);

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
 * @brief 用于统计一个向量不同元素的个数，并且按照值的大小排序。
 * 
 * @param _Y 
 * @param counting_Y 输出结果,其内存布局如下：
 *            {int,float, float, float, float,...., int, int, int, int,}
 *            其中第一个int，为输出的不同值的个数。float...，为不同值的数值，已经经过排序。
 *            最后 int...，为每个不同值的数量。
 * @return int 
 */
int counting_Y(matrix2_t* _Y, void** counting_Y)
{
    if (Mat2_is_vector(_Y)) {
        // 先申请内存。
        // 最终内存模型如下
        // {int,float, float, float, float,...., int, int, int, int,}
        void* output = (vfloat_t*) malloc(sizeof(int)+SPARE_ARR_INC_SIZE * sizeof(vfloat_t));
        // 把 pool size 置于刚刚申请的内存大小。
        int pool_size = SPARE_ARR_INC_SIZE;
        // 把申请了的内存初始化最大的 float 点, 为了做二分查找插入。
    
        int*      size_ptr = CTY_size_ptr(output)
        vfloat_t* arr_ptr  = CTY_elems_ptr(output);
    
        for (int i=0; i<pool_size; ++i) {

            arr_ptr[i] = FLT_MAX;
        }

        *size_ptr    = 0;
        int arr_size = _Y->rows * _Y->cols;

        for (int i=0; i<arr_size; ++i) {
        
            // 在二分查找插入前会确保 数组足够大。
            if (*size_ptr == pool_size) {

            // 满了需要添加内存
            output = (vfloat_t*)realloc(output, sizeof(int) + (pool_size+=SPARE_ARR_INC_SIZE) * sizeof(vfloat_t));
            
            // 重新申请内存后要及时更新 arr 以及 size 的地址。因为 realloc 后首地址可能会改变。
            size_ptr = CTY_size_ptr(output);//MAT2_COUNTING_SIZE_PTR(output);
            arr_ptr  = CTY_elems_ptr(output);//MAT2_COUNTING_LIST_PTR(output);

            // 添加内存后，把它初始化为 max float。
            for (int j = *size_ptr; j<pool_size; ++j) {
                arr_ptr[j] = FLT_MAX;
            }
        }
        
        // 二分查找并且插入。成功返回插入位置。否则返回 -1
        int pos = binary_search_insert(arr_ptr, _Y->pool[i], 0, pool_size);

        // 成功插入数值加一
        if (pos >= 0) (*size_ptr)++;
    }
    return -1;
}

int counting_get_elem_number(void* counting, vfloat_t target)
{
    int size            = CTY_size(counting);
    vfloat_t* elem_ptr  = CTY_elems_ptr(counting);
    int* number_ptr     = CTY_elems_number_ptr(counting);

    int pos = binary_search(elem_ptr, target, 0, (*size_ptr), 0);

    if (pos >=0) return number_ptr[pos];

    return 0;
}

int counting_XY(matrix2_t* _Y, matrix2_t* _X, void** counting_table)
{
    matrix2_t* countingY_mat = Mat2_create(1,1);
    void* Y_counting = NULL;
    counting_Y(_Y, &Y_counting);

    int cty_size           =  CTY_size(Y_counting);//*MAT2_COUNTING_SIZE_PTR(*Py_counting);
    vfloat_t* cty_elem_ptr = CTY_elems_ptr(Y_counting);//MAT2_COUNTING_LIST_PTR(*Py_counting);

    MAT2_POOL_PTR(_Y, _Y_ptr);
    MAT2_POOL_PTR(_X, _X_ptr);

    // 多申请多两个单位，用于存储横列信息。
    size_t table_size = 2 * sizeof(int) + cty_size * _X->cols * sizeof(char*);
    void* table1 = malloc( table_size );
    memset(table1, 0x0, table_size);
    ((int*)table1)[0] = cty_size;
    ((int*)table1)[1] = _X->cols;

    char* (*table_ptr)[train_mat->cols] = CTXY_counting_ptr(table1); //&(((int*)table1)[2]);
    // 
    
    // 建立统计表。
    vfloat_t elem_value[_X->rows];
    int elem_size;

    for (int i=0; i<cty_size; ++i) {
        float_t cty_elem = cty_elem_ptr[i];

        for (int j=0; j<_X->cols; ++j) {

            elem_size = 0;

            for (int k=0; k<_X->rows; ++k) {
                //
                float_t _y_elem = _Y_ptr[k][0];

                if (cty_elem == _y_elem) {
                    elem_value[elem_size++] = _X_ptr[k][j];
                }
            } 
            void* out;
            Mat2_load_on_shape(countingY_mat, elem_value, 1, elem_size);

            counting_Y(countingY_mat, &out);
            table_ptr[i][j] = out;
        }
    }

    *Px_y_counting_table = table1;
    Mat2_destroy(countingY_mat);

    return 0;
}

