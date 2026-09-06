/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-02-22 15:34:38
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-09-06 00:20:19
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_ticket.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_TICKET_H__
#define __CG_TICKET_H__

typedef struct cg_hash cg_hash_t;
typedef struct cg_node cg_node_t;

typedef struct cg_ticket {
    
    char respect_id[64];
    int  used;
    struct cg_ticket* next;
} cg_ticket_t;

int cg_ticket_give(cg_hash_t* marker, cg_node_t* respect, cg_node_t* x);
int cg_ticket_use(cg_ticket_t* ticket);
int cg_ticket_is_used(cg_ticket_t* ticket);

int cg_ticket_get(cg_hash_t* market, cg_node_t* respect, cg_node_t* x, cg_ticket_t** ticket);
int cg_ticket_is_clean(cg_hash_t* market, cg_node_t* x);
int cg_ticket_recycle(cg_ticket_t* ticket_first);

#endif