#include "deep_learning/compute_graph2/cg_list.h"
#include "tf.h"

int tf_init(tf_t* model)
{
    cg_allocator_init(&model->alloc);
    model->node_list   = cg_list_create();
    model->build_stack = cg_list_create();
    return 0;
}

int tf_reset(tf_t* model)
{
    cg_list_recycle(model->node_list);
    cg_list_recycle(model->build_stack):
    cg_allocator_reset(&model->alloc);
    return 0;
}
