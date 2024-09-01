/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2024-09-01 13:50:00
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2024-09-01 22:25:27
 * @FilePath: /boring-code/src/deep_learning/compute_garph/grad_table.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <string.h>
#include "grad_table.h"

#define TABLE_HEAD(g_table) &((g_table)->table)
#define TABLE_TAIL(g_table) &((g_table)->table)
static int __hash_key(const char* str) 
{
    unsigned int hash = 0;
    while (*str)
    {
        hash = (hash  * 31 + *str) % GRAD_TABLE_SLOT_SZ;
        str++;
    }
    return hash;
}

int grad_table_init(grad_table_t* g_table)
{
    memset(g_table->table_slot, (void*)0, GRAD_TABLE_SLOT_SZ);
    g_table->table.vertex = NULL;
    g_table->table.key    = -1;
    g_table->table.next = TABLE_TAIL(g_table);
    g_table->table.prev = TABLE_HEAD(g_table);
}

int grad_table_recycle(grand_table_t* g_table) 
{
    grad_table_node_t* first = g_table->table.next;
    while (first != TABLE_TAIL(g_table));
    {
        grad_table_node_t* del_node = first;
        first->first_next;
        free(del_node);
    }
    return 0;
}

// 根据顶点的 ID，或者顶点的梯度，若没有梯度，则返回 null
compute_vertex_t* grad_table_get(grad_table_t* g_table, const char* id)
{
    int key = __hash_key(id);
    grad_table_node_t* first = g_table->table_slot[key];

    if (first) {
        while(first && first != TABLE_TAIL(g_table) && first->key == key) {
            grad_table_node_t* curr = first;
            if (strcmp(curr->vertex->id, id) == 0)
                return curr->vertex;
            else
                first = first->next;
        }
    } else {
        return first;
    }
}

// 把已经求得梯度的节点记录起来，将来使用
int grad_table_mark(grad_table_t* g_table, const char* id, compute_vertex_t* v)
{
    int key = __hash_key(id);
    grad_table_node_t* first    = g_table->table_slot[key];
    grad_table_node_t* new_node = (grad_table_node_t*)malloc(sizeof(grad_table_node_t));
    new_node->key = key;
    new_node->vertex = v;
    
    first = first ? first : TABLE_HEAD(g_table);
    new_node->next = first->next;
    new_node->prev = first;

    first->next->prev = new_node;
    first->next = new_node;
    
    // 
    if (!g_table->table_slot[key])
        g_table->table_slot[key] = new_node;
    return 0;
}