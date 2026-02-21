#ifndef __CG_NODE_H__
#define __CG_NODE_H__

#include <string.h>
#include "cg_list.h"
#include "cg_vertex.h"

typedef enum cg_node_type {
    e_operand = 1,
    e_operator,
} cg_node_type_t;

typedef struct cg_node {
    cg_vertex_t    vertex;
    cg_node_type_t node_type;
} cg_node_t;

typedef struct cg_ticket {

    char node_id[64];
    int used;
    struct cg_ticket* next;

} cg_ticket_t;

static inline int cg_node_init(cg_node_t* thiz, const char* id, cg_node_type_t node_type) 
{
    thiz->node_type = node_type;
    strcpy(thiz->vertex.id, id);

    thiz->vertex.in      = cg_list_create();
    thiz->vertex.out     = cg_list_create();

    return 0;
}

static inline int cg_node_reset(cg_node_t* thiz) 
{
    cg_list_recycle(thiz->vertex.in, NULL);
    cg_list_recycle(thiz->vertex.out, NULL);
    return 0;
}

static inline cg_ticket_t* cg_ticket_create(const char* id) 
{
    cg_ticket_t* ticket = (cg_ticket_t*) malloc (sizeof(cg_ticket_t));
    strcpy(ticket->node_id, id);
    ticket->used = 0;
    ticket->next = NULL;
    return ticket;
}

static inline int cg_ticket_insert(cg_ticket_t** next, cg_ticket_t* ticket) 
{
    *next = ticket;
    return 0;
}

static inline int cg_ticket_recycle(cg_ticket_t* ticket_list)
{
    cg_ticket_t* del;
    while( ticket_list ) {
        del         = ticket_list;
        ticket_list = del->next;
        free(del);
    }
    return 0;
}

static inline int cg_ticket_check(cg_ticket_t* ticket_list) 
{
    if (ticket_list) {
        int finished;
        cg_ticket_t* first = ticket_list;
        while (first && (finished = first->used)) {
            first = first->next;
        }
        return finished;
    }
    return -1;
}

#endif