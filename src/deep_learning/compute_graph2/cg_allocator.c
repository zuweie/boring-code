#include <stdlib.h>
#include <string.h>
#include "cg_debug.h"
#include "cg_allocator.h"

int cg_allocator_init(cg_allocator_t* alloc)
{
    alloc->total_alloc = (unsigned long) 0;
    memset(alloc->blocks, (void*)0, sizeof(alloc->blocks));
    alloc->chunk_head = NULL;
    return 0;
}

int cg_allocator_reset(cg_allocator_t* alloc)
{
    // free all the memory chunk
    chunk_t* first = alloc->chunk_head;
    while(first) {
        chunk_t* del = first;
        first = first->next;
        free(del);
    }

    alloc->total_alloc = 0;
    memset(alloc->blocks, (void*)0, sizeof(alloc->blocks));
    alloc->chunk_head = NULL;
    return 0;
}

void* cg_alloc(cg_allocator_t* alloc, unsigned int size)
{
    
    unsigned int  alloc_size    = size + 1;
    unsigned char blocks_index  = BLOCK_INDEX( ROUND_UP(alloc_size) );

    CG_DEBUG("alloc size(real): %d, blocks index: %d\n", alloc_size, blocks_index);

    if (blocks_index < BLOCKS_SIZE) {
        // fetch the memory from the blocks

        block_node_t* block_head = alloc->blocks[blocks_index];

        if (block_head == NULL) {
            // 当前的 block 是空的，那么将申请 chunk.
            // 对应 block_index 得出 block_size 大小，一次申请 ALLOC_NUMBER 个 block 的 chunk
            // 然后将 chunk 划分出 ALLOC_NUMBER 个 block_size 大小的 block

            unsigned int block_size = BLOCK_SIZE(blocks_index);

            CG_DEBUG("alloc size(align) %d\n", block_size);

            chunk_t *chunk = (chunk_t *)malloc(sizeof(chunk_t) + block_size * ALLOC_NUMBER);

            // 把刚刚申请的 chunk 挂到 alloc 的 chunk 上
            chunk->next       = alloc->chunk_head;
            alloc->chunk_head = chunk;

            // 把这个 chunk 分成 ALLOC_NUMBER 块大小相同的 block
            block_node_t *block_first = (block_node_t *)chunk->blocks;
            block_head                = block_first;

            for (int i = 0; i < ALLOC_NUMBER-1; ++i)
            {
                block_first->link = (char *)block_first + block_size;
                block_first       = block_first->link;
            }
            // 最后一块 block 的 link 字段指向 alloc->blocks[blocks_index] 的地址。
            // 将这块划分好的 blocks 挂入 alloc->blocks 中。
            block_first->link           = alloc->blocks[blocks_index];
            alloc->blocks[blocks_index] = block_head;

            // 添加统计
            alloc->total_alloc += block_size * ALLOC_NUMBER; 
        }

        // 把当前的 block 抽出来。
        alloc->blocks[blocks_index] = block_head->link;
        // 把 block 的序号信息存放到 block 的头部第一个字节，回收的时候就可以轻松找到对应的index。
        SET_INDEX(block_head, blocks_index);
        // 把 block 的工作空间往下调 1。因为头部的 1 个字节用于保存 blocks_index。
        return EXPOSE(block_head);

    } else {
        // 对齐八位
        alloc_size = BLOCK_SIZE(blocks_index);
        CG_DEBUG("malloc size(align): %d from system, blocks index is %d\n", alloc_size, blocks_index);
        // 直接从系统申请内存
        void* block_head = (void*) malloc (alloc_size);
        // 配置快索引号，即使不存在。
        SET_INDEX(block_head, blocks_index);
        // 添加内存统计
        alloc->total_alloc += alloc_size;
        // 返回可用的内存地址。
        return EXPOSE(block_head);
    }
}

int cg_recycle(cg_allocator_t* alloc, void* ptr)
{
    unsigned char blocks_index = GET_INDEX(ptr);
    block_node_t* block        = COVER(ptr);
    unsigned int  block_size   = BLOCK_SIZE(blocks_index);

    if (blocks_index < BLOCKS_SIZE) {

        block->link = alloc->blocks[blocks_index];
        alloc->blocks[blocks_index] = block;

    } else {
        // 直接 free 
        alloc->total_alloc -= block_size;
        free(block);
    }
    return 0;
}