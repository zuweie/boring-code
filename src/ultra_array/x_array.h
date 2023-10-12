#ifndef __X_ARRAY_H__
#define __X_ARRAY_H__

typedef struct 
{
    /* data */
    int axisn;
    int* dimens;
    char* elems;
    int data_size;

} x_array_t;

x_array_t* xarray_create(int axisn, ...);
x_array_t* xarray_slice(x_array_t* arr, ...);
x_array_t* xarray_padding(x_array_t* arr, float padding_to, ...);
int xarray_arange(x_array_t* arr, float start, float end);
int xarray_display(x_array_t* arr);
int xarray_recycle(x_array_t* arr);

#endif