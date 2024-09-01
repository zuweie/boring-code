/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2024-09-01 13:50:10
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2024-09-01 21:30:40
 * @FilePath: /boring-code/src/deep_learning/compute_garph/gard_table.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include "compute_garph.h"

#define GRAD_TABLE_SLOT_SZ 1000
typedef struct _grand_table_node grad_table_node_t;

struct _grad_table_node {
    int key;
    compute_vertex_t* node;
    grad_table_node_t* next;
    grad_table_node_t* prev;
};

typedef struct _gard_table {
    grad_table_node_t* table_slot[GRAD_TABLE_SLOT_SZ];
    grad_table_node_t table;
} grad_table_t;


int grad_table_init(grad_table_t*);

int grad_table_recycle(grand_table_t*);

// 根据顶点的 ID，或者顶点的梯度，若没有梯度，则返回 null
compute_vertex_t* grad_table_get(const char* id);

// 把已经求得梯度的节点记录起来，将来使用
int grad_table_mark(const char* id, compute_vertex_t* v);