/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 10:08:02
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-05-27 12:28:10
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_allocator.h
 * @Description: 关于这个内存配置器简单的一个设计如下：
 * 1 按照申请 size ruond up 到 8，例如  53 round_up 56
 * 2 根 56 / 8 - 1 = 6，找到第 6 个槽位，然后从槽位处返回一块 56 size 的 block 内存。
 * 3 当槽为空的时候，算法将向系统 malloc 一个 56 * 64 的块，然后将块分割成 64 个 56 size 的block，然后返回其中的一个 block。
 * 4 改进版本，可以向系统申请一大块 chunk。然后再慢慢向各个槽分配内存，但是这个方法及其复杂，所以暂时不搞了。
 */
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