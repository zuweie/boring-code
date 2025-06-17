#ifndef __CG_VERTEX_H__
#define __CG_VERTEX_H__

typedef struct cg_list cg_list_t;

typedef struct cg_vertex
{
    const char id[64];
    cg_list_t*  in_vertexes;
    cg_list_t*  out_vertexes;
} cg_vertex_t;

#endif