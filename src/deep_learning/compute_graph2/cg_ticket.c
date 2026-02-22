/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-02-22 15:34:30
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-02-22 17:47:19
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_ticket.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <string.h>
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

    if (found != -1) {
        
        cg_ticket_t* new_ticket = __cg_ticket_create(repect->vertex.id);
        if (found == 0) {
            ticket->next = new_ticket;
        } else {
            cg_hash_set(marker, x->vertex.id, new_ticket);
        }
    }
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

int cg_ticket_get(cg_hash_t* marker, cg_node_t* repect, cg_node_t* x, cg_ticket_t** ticket)
{
    cg_ticket_t* ticket_first = cg_hash_get(marker, x->vertex.id);
    
    if (ticket_first) {

        if (strcmp(ticket_first->repect_id, repect->vertex.id) == 0) {
            *ticket = ticket_first;
            return 1;
        } 

        *ticket = ticket_first;
        ticket_first = ticket_first->next;

    } else {
        return -1;
    }
    return 0;
}

int cg_ticket_is_clean(cg_hash_t* market, cg_node_t* x)
{
    cg_ticket_t* first = cg_hash_get(market, x->vertex.id);
    int used = 0;
    if (first) {
        while (first && (used == first->used)) {
            first = first->next;
        }
        return used;
    }
    return 0;
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
