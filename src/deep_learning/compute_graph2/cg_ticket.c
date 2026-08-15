/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-02-22 15:34:30
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-08-16 00:17:48
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_ticket.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <string.h>
#include "cg_debug.h"
#include "cg_hash.h"
#include "cg_node.h"
#include "cg_ticket.h"



static cg_ticket_t* __cg_ticket_create(const char* repect_id) 
{
    cg_ticket_t* ticket = (cg_ticket_t*) malloc (sizeof (cg_ticket_t));
    strcpy(ticket->repect_id, repect_id);
    ticket->next = NULL;
    ticket->used = 0;
    return ticket;
}

int cg_ticket_give(cg_hash_t* marker, cg_node_t* repect, cg_node_t* x)
{
    cg_ticket_t* ticket = NULL;

    int found = cg_ticket_get(marker, repect, x, &ticket);

    if (found != 1) {
        
        cg_ticket_t* new_ticket = __cg_ticket_create( CG_NODE_ID(repect) );
        if (found == 0) {
            ticket->next = new_ticket;
        } else {
            // found == -1, dose not had any ticket, set the first ticket about repect
            cg_hash_set(marker, CG_NODE_ID(x), new_ticket);
        }
        return 0;
    } 
    CG_DEBUG("INFO <%d@%s>: x(%s) had ticket about repect(%s)\n", __LINE__, __FILE__,  CG_NODE_ID(x), CG_NODE_ID(repect));
    return 0;
}

int cg_ticket_use(cg_ticket_t* ticket)
{
    ticket->used = 1;
    return 0;
}

int cg_ticket_is_used(cg_ticket_t* ticket)
{
    return ticket->used;
}

/**
 * @brief 寻找 x 是否有关于上线 repect 的 ticket。如果这个点没有 ticket 返回但是没有关于 repect 的 ticket 返回 0， 如果有返回 1，
 * 如果完全没有任何 ticket 返回  -1.
 * 
 * @param marker 
 * @param repect 
 * @param x 
 * @param ticket 
 * @return int 
 */
int cg_ticket_get(cg_hash_t* marker, cg_node_t* repect, cg_node_t* x, cg_ticket_t** ticket)
{
    cg_ticket_t* ticket_first = cg_hash_get(marker, CG_NODE_ID(x));
    if (ticket_first) {
        while (ticket_first) {

            // 若是找不到 则返回最后一个 ticket
            *ticket = ticket_first;

            if (strcmp(ticket_first->repect_id,  CG_NODE_ID(repect)) == 0) 
                return 1;

            ticket_first = ticket_first->next;
        }
        return 0;
    } 
    CG_DEBUG("INFO <%d@%s>: node x(%s) has not any tickets\n", __LINE__, __FILE__, x->vertex.id);
    return -1;
}

int cg_ticket_is_clean(cg_hash_t* market, cg_node_t* x)
{
    int used;
    cg_ticket_t* first = cg_hash_get(market, CG_NODE_ID(x));

    if (first) {

        while (first && (used = first->used)) first = first->next;
        return used;
        
    } else {
        return 1;
    }
}

int cg_ticket_recycle(cg_ticket_t* ticket_first) 
{
    cg_ticket_t* del;
    while (ticket_first) {
        del = ticket_first;
        free(del);
        ticket_first = ticket_first->next;
    }
    return 0;
}
