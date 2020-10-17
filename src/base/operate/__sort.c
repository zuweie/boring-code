/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-24 23:53:53
 * @LastEditTime: 2019-09-26 08:46:22
 * @LastEditors: Please set LastEditors
 */
#include "__sort.h"

static iterator_t _partition (iterator_t p, iterator_t r, int(*compare)(type_value_t, type_value_t))
{
    type_value_t x = iterator_dereference(r);
    iterator_t i = iterator_prev(p);

    for(;!iterator_equal(p,r);p=iterator_next(p)) {
        if (compare(iterator_dereference(p),x) != 1) {
            // p <= x
            i = iterator_next(i);
            iterator_exchange(i, p);
        }
    }
    iterator_exchange(iterator_next(i), r);
    return iterator_next(i);
}

int quick_sort(iterator_t p, iterator_t r, int(*compare)(type_value_t, type_value_t))
{
    if (!iterator_equal(p, r)) {
        iterator_t q = _partition(p, r, compare);
        
        if (!iterator_equal(p,q)) 
            quick_sort(p, iterator_prev(q), compare);

        if (!iterator_equal(q,r)) 
            quick_sort(iterator_next(q), r, compare);

    }
    return 0;
}