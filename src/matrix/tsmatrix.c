/*
 * @Author: your name
 * @Date: 2020-06-13 10:14:00
 * @LastEditTime: 2020-06-14 11:57:58
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /kitc/src/matrix/tsmatrix.c
 */ 

#include "tsmatrix.h"
#include "matrix.h"

static inline int _free_elem (tv e) 
{
    ts_elem* elem = t2p(e);
    free(elem);
}
static inline int _find_elem (tv e, tv c) {

    ts_coord* coord = t2p(c);
    ts_elem*  elem  = t2p(e);
    if (elem->position.x == coord->x && elem->position.y == coord->y) {
        return 0;
    }
    return -1;
}

static ts_elem* _create_ts_elem (int i, int j, float v) 
{
    ts_elem* elem = (ts_elem*) malloc (sizeof(ts_elem));
    elem->position.x = i;
    elem->position.y = j;
    elem->value = v;
    return elem; 
}

TSMatrix* TSMatrix_create(size_t row, size_t col) 
{
    TSMatrix* tsmatrix = (TSMatrix*) malloc (sizeof(TSMatrix));
    tsmatrix->row = row;
    tsmatrix->col = col;
    LinkArr_init(&tsmatrix->elems, _find_elem);
    return tsmatrix;
}

TSMatrix* TSMatrix_create_by(size_t row, size_t col, float data[][col])
{
    TSMatrix * tsmatrix = (TSMatrix* ) malloc (sizeof(TSMatrix));
    tsmatrix->row = row;
    tsmatrix->col = col;
    LinkArr_init(&tsmatrix->elems, NULL);

    for (int i=0; i<row; ++i) {
        for (int j=0; j<col; ++j) {
            if (data[i][j] > 0.0f) {
                ts_elem* elem = _create_ts_elem(i,j, data[i][j]);
                CN_add_tail(&tsmatrix->elems, p2t(elem));
            }
        }
    }
    return tsmatrix;
}

TSMatrix* TSMatrix_create_transpose(TSMatrix* tsmatrix)
{
    // TODO : make transpose.
    TSMatrix* trans = TSMatrix_create(tsmatrix->col, tsmatrix->row);
    for (it first = CN_first(&tsmatrix->elems); !It_equal(first, CN_tail(&tsmatrix->elems)); first=It_next(first)) {
        ts_elem* elem = It_getptr(first);
        float value = TSMatrix_get(tsmatrix, elem->position.x, elem->position.y);
        TSMatrix_set(trans, elem->position.y, elem->position.x, value);
    }
    return trans;
}

Matrix* TSMatrix_create_matrix(TSMatrix* tsmatrix)
{
    Matrix* matrix = Matrix_create(tsmatrix->row, tsmatrix->col);
    for (it first = CN_first(&tsmatrix->elems); !It_equal(first, CN_tail(&tsmatrix->elems)); first= It_next(first)) {
        ts_elem* elem = It_getptr(first);
        Matrix_set(matrix, elem->position.x, elem->position.y, elem->value);
    }
    return (Matrix*)NULL;
}

int TSMatrix_destroy(TSMatrix* tsmatrix)
{
    CN_cleanup(&tsmatrix->elems, _free_elem);
    LinkArr_free(&tsmatrix->elems);
    free(tsmatrix);
    return 0;
}

float TSMatrix_get(TSMatrix* tsmatrix, int x, int y)
{
    ts_coord coord;
    coord.x = x;
    coord.y = y;
    it pos = CN_find(&tsmatrix->elems, p2t(&coord));
    return It_valid(pos)? ((ts_elem*)(It_getptr(pos)))->value : 0.0f;
}

int TSMatrix_set(TSMatrix* tsmatrix, int x, int y, float v)
{
    ts_coord coord;
    coord.x = x;
    coord.y = y;
    it pos = CN_find(&tsmatrix->elems, p2t(&coord));

    if (v != 0.0f) {
        !It_valid(pos) ? CN_add_tail(&tsmatrix->elems, p2t(_create_ts_elem(x,y,v))) : (((ts_elem*) It_getptr(pos))->value = v);
    }else if ( It_valid(pos) ){
        CN_remove(&tsmatrix->elems, pos, NULL);
    }
    return 0;
}