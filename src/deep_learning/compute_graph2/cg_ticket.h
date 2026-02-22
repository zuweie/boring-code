#ifndef __CG_TICKET_H__
#define __CG_TICKET_H__

typedef struct cg_hash cg_hash_t;
typedef struct cg_node cg_node_t;

typedef struct cg_ticket {
    
    char repect_id[64];
    int  used;
    struct cg_ticket* next;
} cg_ticket_t;

int cg_ticket_give(cg_hash_t* marker, cg_node_t* repect, cg_node_t* x);
int cg_ticket_use(cg_ticket_t* ticket);
int cg_ticket_is_used(cg_ticket_t* ticket);

int cg_ticket_get(cg_hash_t* market, cg_node_t* repect, cg_node_t* x, cg_ticket_t** ticket);
int cg_ticket_is_clean(cg_hash_t* market, cg_node_t* x);
int cg_ticket_recycle(cg_ticket_t* ticket_first);

#endif