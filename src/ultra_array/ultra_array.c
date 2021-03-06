/*
 * @Author: your name
 * @Date: 2021-01-31 16:24:27
 * @LastEditTime: 2021-03-11 11:11:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/xarray/xarray.c
 */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "ultra_data_chunk.h"
#include "ultra_router.h"
#include "ultra_padding.h"
#include "ultra_array.h"

u_array_t ua_unable = {
    .start = {NULL, NULL},
    .axis_n = -1,
};

static size_t 
__axis_mulitply(size_t axes[], int tail, int from) 
{
    size_t size = 1;
    for (int i=from; i<tail; ++i) {
        size *= axes[i];
    }
    return size;
}

static void*
__alloc_memory(size_t n) 
{
    // if (alloc) 
    //     return allocate(alloc, n);
    // else
        return malloc(n);
}

static void 
__recycle_memory(void* p) 
{
    // if (alloc) 
    //     deallocate(alloc, p);
    // else
        free(p);
}

static void*
__alloc_data(size_t data_n) 
{
    void* p = __alloc_memory(data_n*sizeof(double));
    return p;
}

static void*
__alloc_shape(size_t axis_n, size_t shape[])
{
    if (axis_n > 0) {
        void* p = __alloc_memory((axis_n <=0? 1: axis_n) * sizeof(size_t));
        memcpy(p, shape, axis_n * sizeof(size_t));
        return p;
    } 
    return NULL;
}

static void
__alloc_start(size_t axis_n, size_t shape[], char *start[]) 
{
    start[0] = __alloc_shape(axis_n, shape);
    start[1] = __alloc_data(__axis_mulitply(shape, axis_n, 0));
    return;
}

static void
__recycle_start(char* start[]) 
{
    if (start[0] != NULL) 
        __recycle_memory(start[0]);
    
    if (start[1] != NULL) 
        __recycle_memory(start[1]);

    return;
}

static double
__dot_1d(double *a, double *b, size_t n) 
{
    double v = 0.f;
    for (int i=0; i<n; ++i) {
        v += a[i] * b[i];
    }
    return v;
}

static void
__to_display(u_array_t* arr, int axis, char* ptr, int bank_number) 
{
    bank_number++;
    if (axis == UA_axisn(arr) - 1) {
        double* data = ptr;

        for (int i=0; i<bank_number; ++i) printf("  "); 

        printf("[");

        size_t chunk_number = UA_shape_axis(arr, axis);
        
        for (int i=0; i<chunk_number; ++i) {
            printf("%0.5f ", data[i]);
        }
        printf("],\n");
    } else {
        
        size_t chunk_number = UA_shape_axis(arr, axis);
        size_t chunk_size = UArray_axis_mulitply(arr, axis+1) * sizeof(double);
        for (int i=0; i<bank_number; ++i) printf("  "); 
        printf("[\n");
        for (int i=0; i<chunk_number; ++i) {
            __to_display(arr, axis+1, ptr + i*chunk_size, bank_number);
        }
        for (int i=0; i<bank_number; ++i) printf("  "); 
        printf("],\n");
    }
}

// static void
// __dot_2d(double *a, size_t a_r, size_t a_c, double *b, size_t b_r, size_t b_c, double *c) 
// {
//     double (*_a)[a_c] = a;
//     double (*_b)[b_c] = b;
//     double (*_c)[b_c] = c;
//     double v = 0.f;

//     for (int i=0; i<a_r; ++i) {
//         for (int j=0; j<b_c; ++j) {
//             v = 0.f;
//             for (int k=0; k<a_c; ++k) {
//                 v += _a[i][k] * _b[k][j];
//             }
//             _c[i][j] = v;
//         }
//     }
// }

static size_t
__xd_coord_to_1d_offset(size_t coord[], size_t axes[], int axis_n) {

    size_t offset = 0, axis_mulitply;
    for (int i=0; i<axis_n; ++i) {
        size_t co = coord[i];
        axis_mulitply = __axis_mulitply(axes, axis_n, i+1);
        offset += co * axis_mulitply;
    }
    return offset;
}

static void
__1d_offset_to_xd_coord( size_t offset, size_t axes[], int axis_n, size_t coord[])
{
    size_t div, mod, i, axis_mulitply, middle_value;
    middle_value = offset;
    for(i=0; i<axis_n-1; ++i) {
        axis_mulitply = __axis_mulitply(axes, axis_n, i+1);
        div = middle_value / axis_mulitply;
        mod = middle_value % axis_mulitply;
        coord[i] = div;
        middle_value = mod;
    }
    coord[i] = mod;
    return;
}

static void
___transpose(double* a, size_t shape_a[], double* b, size_t shape_b[], int axis_n, size_t trans_axis_index[]) 
{
    size_t coord_a[axis_n];
    size_t coord_b[axis_n];
    size_t size_a = __axis_mulitply(shape_a, axis_n, 0);

    for (int i=0; i<size_a; ++i) {
        __1d_offset_to_xd_coord(i, shape_a, axis_n, coord_a);

        // 坐标轴调换：
        for (int j=0; j<axis_n; ++j) {
            coord_b[j] = coord_a[ trans_axis_index[j] ];
        }
        // 计算 
        size_t index = __xd_coord_to_1d_offset(coord_b, shape_b, axis_n);
        b[index] = a[i];
    }
    return;
}
static int
__update_shape(u_array_t* arr, size_t shape[], int axis_n) 
{
    if (arr->axis_n != axis_n ) {
        __recycle_memory(UA_shape(arr));
        arr->start[0] = __alloc_shape(axis_n, shape);
        arr->axis_n   = axis_n;
    } else if (axis_n != 0){
        memcpy(UA_shape(arr), shape, axis_n * sizeof(size_t));
    }
    return 0;
}

u_array_t UArray_create_with_axes_dots(int axis_n, ...)
{
    va_list valist;
    va_start(valist, axis_n);
    size_t axes[axis_n];
    for (int i=0; i<axis_n; ++i) {
        axes[i] = va_arg(valist, size_t);
    }
    va_end(valist);
    return UArray_create_with_axes_array(axis_n, axes);
}

u_array_t UArray_create_with_axes_array(int axis_n, size_t shape[]) 
{
    if (axis_n >= 0) {
        u_array_t n_array;
        n_array.axis_n = axis_n;

        __alloc_start(axis_n, shape, n_array.start);
        
        return n_array;
    }
    return ua_unable;
}
double UArray_get(u_array_t* arr, ...) 
{
    va_list valist;
    va_start(valist, arr);
    size_t coord[UA_axisn(arr)];
    for (int i=0; i<UA_axisn(arr); ++i) {
        coord[i] = va_arg(valist, size_t);
    }
    va_end(valist);
    size_t offset = UA_cover_coordinate(arr, coord);
    return ((double*)(UA_data_ptr(arr)))[offset];
}
void UArray_set(u_array_t* arr, double v, ...)
{

    va_list valist;
    va_start(valist, v);
    size_t coord[UA_axisn(arr)];
    for (int i=0; i<UA_axisn(arr); ++i) {
        coord[i] = va_arg(valist, size_t);
    }
    va_end(valist);
    size_t offset = UA_cover_coordinate(arr, coord);
    ((double*)(UA_data_ptr(arr)))[offset] = v;
    return;
}
void UArray_destroy(u_array_t* arr)
{  
    return __recycle_start(arr->start);
}

void* UArray_data_copy(u_array_t* parr) 
{
    size_t size_arr_data = UA_size(parr) * sizeof(double);
    void* pdata = malloc(size_arr_data);
    memcpy(pdata, UA_data_ptr(parr), size_arr_data);
    return pdata;
}

u_array_t* UArray_arange(u_array_t *arr, int range)
{
    double* data   = UA_data_ptr(arr);
    size_t  size_a = UA_size(arr);
    
    for (int i=0; i<range && i<size_a; ++i) {
        data[i] = i;
    }
    return arr;
}
u_array_t* UArray_arange_scope(u_array_t* arr, int start, int tail) 
{
    double* data_ptr = UA_data_ptr(arr);
    size_t size_arr  = UA_size(arr);
    double per_step = (double)(tail-start) / (double)size_arr;
    for (int i=0; i<size_arr; ++i) {
        data_ptr[i] = start + i * per_step;
    }
    return arr;
}
u_array_t* UArray_ones(u_array_t* arr, double v) {
    double* data_ptr = UA_data_ptr(arr);
    size_t  size_a   = UA_size(arr);
    for (int i=0; i<size_a; ++i) {
        data_ptr[i] = v;
    }
    return arr;
}

u_array_t* UArray_reshape(u_array_t* a, size_t axes[], int axis_n) 
{
    size_t size_r = __axis_mulitply(axes, axis_n, 0);
    size_t size_a = UA_size(a);
    if (size_r == size_a) {
        __update_shape(a, axes, axis_n);
    }
    return a;
}


//操作降维
u_array_t* UArray_operate(u_array_t* arr, int axis, operater_t op)
{
    if (axis>=0 && axis<arr->axis_n) {
        double result;
        size_t number = __axis_mulitply(UA_shape(arr), axis, 0); 
        size_t step   = __axis_mulitply(UA_shape(arr), arr->axis_n, axis);
        double *arr_data_copy = UArray_data_copy(arr);
        double *arr_data = UA_data_ptr(arr);
        size_t shape_new[axis];
        int    axisn_new = axis;

        for (int i=0; i<axis; ++i) {
            shape_new[i] = UA_shape_axis(arr, i);
        }

        // 1 开始计算数据并更新数据。
        for (int k=0, m=0; k<number; ++k, ++m) {

            if (op == ua_sub || op == ua_sum) {
                result = 0.f;
            } else {
                result = 1.f;
            }
            
            for (int l=0; l<step; ++l) {
                switch (op)
                {
                case ua_sum:
                    result += arr_data_copy[ k*step + l ];
                    break;
                case ua_sub:
                    result -= arr_data_copy[ k*step + l ];
                    break;
                case ua_mulitply:
                    result *= arr_data_copy[ k*step + l ];
                    break;
                case ua_div:
                    if (arr_data_copy[ k*step +l ] != 0.f)
                        result /= arr_data_copy[ k*step + l ];
                    break;
                default:
                    break;
                }
            }
            arr_data[m] = result;
        }
        free(arr_data_copy);

        // update shape
        __update_shape(arr, shape_new, axisn_new);
        //return 0;
    }
    //return -1;
    return arr;
}


size_t UArray_axis_mulitply(u_array_t* a, int axis_idx_from) 
{
    return __axis_mulitply(UA_shape(a), a->axis_n, axis_idx_from);
}


u_array_t* UArray_transpose(u_array_t* arr, size_t trans_axis_index[]) 
{
    size_t trans_shape[arr->axis_n];

    for (int i=0; i<arr->axis_n; ++i) {
        trans_shape[i] = UA_shape_axis(arr, trans_axis_index[i]);
    }


    size_t size_arr_tmp  = UA_size(arr) * sizeof(double);
    void*  data_arr_tmp  = malloc(size_arr_tmp);

    void*   data_arr   = UA_data_ptr(arr);
    size_t* shape_arr  = UA_shape(arr);
    
    memcpy(data_arr_tmp, data_arr, size_arr_tmp);

    ___transpose(data_arr_tmp, shape_arr, data_arr, trans_shape, arr->axis_n, trans_axis_index);

    __update_shape(arr, trans_shape, arr->axis_n);

    free(data_arr_tmp);

    return arr;
}

u_array_t* UArray_transform(u_array_t* arr) 
{
    size_t shape_trans[arr->axis_n];
    for (int i=0; i<arr->axis_n; ++i) {
        shape_trans[i] = arr->axis_n - i -1;
    }
    return UArray_transpose(arr, shape_trans);
}

/**
 * 超级鸡吧复杂多维内积算法，核心思想就是用第一个数组的最后一维，点积第二个数组倒数第二维。
*/
u_array_t UArray_dot_new_copy(u_array_t* a1, u_array_t* a2) 
{
    if (a1->axis_n >=2 && a2->axis_n >= 2) {
        size_t row_size_a1 = UA_shape_axis(a1, a1->axis_n-1);
        size_t col_size_a2 = UA_shape_axis(a2, a2->axis_n-2);

        if (row_size_a1 == col_size_a2){
            
            int i,j,k,m=0,l=0;

            size_t n_axes[a1->axis_n + a2->axis_n - 2];
            size_t* shape_a1 = UA_shape(a1);
            size_t* shape_a2 = UA_shape(a2);

            for (i=0; i<a1->axis_n-1; ++i) {
                n_axes[i] = shape_a1[i];
            }

            for (j=0; j<a2->axis_n; ++j) {
                if (j != a2->axis_n - 2) {
                    n_axes[i++] = shape_a2[j];
                }
            }

            u_array_t a3 = UArray_create_with_axes_array(a1->axis_n + a2->axis_n - 2, n_axes);

            // a1 的 总的行数
            size_t total_rows_number_a1  = __axis_mulitply(shape_a1, a1->axis_n-2, 0) * UA_shape_axis(a1, a1->axis_n-2);

            // a2 的 二维块的总数
            size_t cube_number_a2 = __axis_mulitply(shape_a2, a2->axis_n-2, 0);
            // a2 的每个二维块的行的大小。
            size_t col_number_a2   = UA_shape_axis(a2, a2->axis_n-1);
            // a2 每个二维块的大小。
            size_t cube_size_a2   = col_size_a2 * col_number_a2;

            // a1 data
            double* data_a1 = UA_data_ptr(a1);
            double* data_a2 = UA_data_ptr(a2);
            double* data_a3 = UA_data_ptr(&a3);

            double col_data_a2[col_size_a2];
            
            for (i=0; i<total_rows_number_a1; ++i) {
                for (j=0; j<cube_number_a2; ++j) {
                    
                    for (k=0; k<col_number_a2; ++k) {
                        for (m=0; m<col_size_a2; ++m ) {

                            size_t offset = j*cube_size_a2 + m * col_number_a2 + k;
                            col_data_a2[m] = data_a2[offset];

                        }
                        data_a3[l++] =__dot_1d( &data_a1[i*row_size_a1], col_data_a2, row_size_a1);
                    }
                }
            }
            return a3;
        } 
    }
    return ua_unable;
}

u_array_t UArray_fission(u_array_t* a, char indicator_str[])
{
    ua_indicator_t* indicators;
    UArray_indicator_parse(indicator_str, &indicators);
    u_array_t fission = UArray_fission_with_indicators(a, indicators);
    UArray_indicator_release(indicators);
    return fission;
}

u_array_t UArray_fission_with_indicators(u_array_t* a, ua_indicator_t* indicators) 
{
    ua_chunk_note_t chunk_note;
    UArray_indicator_analysis(indicators, a, &chunk_note);
    u_array_t fission = UArray_create_with_axes_array(chunk_note.axis_n, chunk_note.shape);

    ua_data_chunk_t* ptr = chunk_note.chunk_map;
    char* data_ptr       = UA_data_ptr(&fission);
    
    while( ptr != NULL) {

        memcpy(data_ptr, ptr->chunk_addr, ptr->chunk_size);
        data_ptr += ptr->chunk_size;
        ptr = ptr->next;

    }
    UArray_chunk_note_finalize(&chunk_note);
    return fission;
}

u_array_t* UArray_assimilate(u_array_t* a1, char indicator_str[], u_array_t* a2)
{
    ua_indicator_t* indicators;
    UArray_indicator_parse(indicator_str, &indicators);
    u_array_t* ass = UArray_assimilate_with_indicators(a1, indicators, a2);
    UArray_indicator_release(indicators);
    return ass;
}

u_array_t* UArray_assimilate_with_indicators(u_array_t* a1, ua_indicator_t* indicators, u_array_t* a2)
{
    ua_chunk_note_t chunk_note;
    UArray_indicator_analysis(indicators, a1, &chunk_note);
    int do_copy = 0;
    if (UA_axisn(a2) == 0 
    || __axis_mulitply(chunk_note.shape, chunk_note.axis_n, 0) % UA_size(a2) == 0 ) {
        
        ua_data_chunk_t* ptr = chunk_note.chunk_map;
        size_t chunk_size_a2 = UA_size(a2)*sizeof(double);
        char* data_ptr_a2 = UA_data_ptr(a2);

        while(ptr != NULL) {
            
            if (chunk_size_a2 > ptr->chunk_size) {
                
                int cpy_step = chunk_size_a2 / ptr->chunk_size;
                for (int i=0; i<cpy_step && ptr != NULL; ++i, ptr = ptr->next) {
                    memcpy(ptr->chunk_addr, data_ptr_a2 + i * ptr->chunk_size, ptr->chunk_size);
                }

            } else {
                char* chunk_addr_note = ptr->chunk_addr;
                int cpy_step = ptr->chunk_size / chunk_size_a2;
                for (int i=0; i<cpy_step; ++i) {
                    memcpy(chunk_addr_note + i * chunk_size_a2, data_ptr_a2, chunk_size_a2);
                }
                ptr = ptr->next;
            }
        }
        do_copy = 1;
    }     
    UArray_chunk_note_finalize(&chunk_note);
    return do_copy? a1 : NULL;
}

u_array_t UArray_padding(u_array_t* arr, ua_pad_width_t padding[], ua_pad_mode_t pad_mode, double* constansts)
{
    char router[256] = {'\0'};

    int axisn_arr = UA_axisn(arr);
    size_t pad_shape[axisn_arr];

    for (int i=0; i<axisn_arr; ++i) {
        ua_pad_width_t pad = padding[i];
        pad_shape[i] = UA_shape_axis(arr, i) + pad.before_n + pad.after_n;
    }

    u_array_t pad_arr = UArray_create_with_axes_array(axisn_arr, pad_shape);

    UArray_cover_pad_width_to_router_str(padding, axisn_arr, router);

    UA_assimilate(&pad_arr, router, arr);

    // 开始填充周边的数字

    for (int k=axisn_arr-1; k>=0; --k) {
        UArray_fill_pad_data(&pad_arr, k, 0, UA_data_ptr(&pad_arr), padding, pad_mode, constansts);
    }

    return pad_arr;
}

u_array_t UArray_pad(u_array_t* arr, char pad_width_str[], ua_pad_mode_t mode, double* constansts)
{
    int axisn_arr = UA_axisn(arr);
    ua_pad_width_t pad_width[axisn_arr];
    memset(pad_width, 0, axisn_arr * sizeof(ua_pad_width_t));

    int ret = UArray_parse_pad_width_str(pad_width_str, pad_width, axisn_arr);
    return ret == 0 ? UArray_padding(arr, pad_width, mode, constansts) : ua_unable;
}

u_array_t UArray_empty_like(u_array_t* arr) 
{
    u_array_t copy = UArray_create_with_axes_array(UA_axisn(arr), UA_shape(arr));
    return copy;
}

u_array_t* UArray_log(u_array_t* arr) 
{
    size_t size_a    = UA_size(arr);
    double* data_ptr = UA_data_ptr(arr);
    for (size_t i=0; i<size_a; ++i) {
        data_ptr[i] = log(data_ptr[i]);
    }
    return arr;
}

u_array_t* UArray_pow2(u_array_t* arr) 
{
    size_t size_arr = UA_size(arr);
    double* ptr = UA_data_ptr(arr);
    for(size_t i=0; i<size_arr; ++i) {
        ptr[i] = ptr[i] * ptr[i];
    }
    return arr;
}


void UArray_display(u_array_t* arr)
{
    return __to_display(arr, 0, UA_data_ptr(arr), 0);
}

size_t UArray_xd_coord_to_1d_offset(u_array_t* arr, size_t coord[])
{
    size_t* axes = UA_shape(arr);
    return __xd_coord_to_1d_offset(coord, axes, arr->axis_n);
}

void UArray_1d_offset_to_xd_coord(u_array_t* arr, size_t offset, size_t coord[])
{
    size_t* axes = UA_shape(arr);
    return __1d_offset_to_xd_coord(offset, axes, arr->axis_n, coord);
}
