#ifndef __CG_ALLOCATOR_H__
#define __CG_ALLOCATOR_H__

#define ROUND_UP(x) (((x) + 8 - 1) & ~(8 - 1))
#define BLOCKS_SIZE  128
#define ALLOC_NUMBER  64

#define BLOCK_INDEX(size) ((size)/8-1)
#define BLOCK_SIZE(index) (8 * ((index)+1))

#define GET_INDEX(block) (*((unsigned char*)(block)))
#define SET_INDEX(block, index) (*((unsigned char*)(block)) = (unsigned char)(index))  //({*((unsigned char*)(block))=(unsigned char)(index);})

#define EXPOSE(block) ((char*)(block)-1)
#define COVER(block)  ((char*)(block)+1)

typedef struct chunk
{
    struct chunk* next;
    char blocks[0];
} chunk_t;

typedef union block_node
{
    union block_node* link;
    char data[0];
} block_node_t;

typedef struct cg_allocator
{
    /* data */
    unsigned long total_alloc;
    char*         blocks[BLOCKS_SIZE];
    chunk_t*      chunk_head;
}cg_allocator_t;


int cg_allocator_init(cg_allocator_t*);
int cg_allocator_reset(cg_allocator_t*);
void* cg_allocator_inspect(cg_allocator_t*);

void* cg_alloc(cg_allocator_t*, unsigned int);
int   cg_recycle(cg_allocator_t*, void*);


#endif