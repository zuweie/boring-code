/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-24 23:53:53
 * @LastEditTime: 2025-06-03 11:40:13
 * @LastEditors: zuweie jojoe.wei@gmail.com
 */

#include "base/__iterator.h"
#include "base/__container.h"
#include "__quick_sort.h"

static iterator_t __partition (iterator_t p, iterator_t r, int(*compare)(type_value_t*, type_value_t*))
{
    //type_value_t x = iterator_dereference(r);
    // iterator_t i = p;
    // iterator_prev(i);

    iterator_t i = iterator_prev(p);

    for(;!iterator_equal(p,r);p=iterator_next(p)) {
        if (compare(p.reference, r.reference) != 1)
            iterator_exchange((i=iterator_next(i)), p);
    }
    iterator_exchange((i=iterator_next(i)), r);
    return i;
}

int quick_sort(iterator_t p, iterator_t r, int(*compare)(type_value_t*, type_value_t*))
{
    if (!iterator_equal(p, r)) {
        iterator_t q = __partition(p, r, compare);
        
        iterator_t prev_q = q, next_q = q;

        if (!iterator_equal(p,q)) 
            quick_sort(p, iterator_prev(prev_q), compare);

        if (!iterator_equal(q,r)) 
            quick_sort(iterator_next(next_q), r, compare);

    }
    return 0;
}