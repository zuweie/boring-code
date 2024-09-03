#ifndef __CG_GARPH_H__
#define __CG_GARPH_H__

typedef struct {

    const char *p_id;
    char visit_mark;
    cg_list_t* p_income_vertexes;
    cg_list_t* p_outcome_vertexes;
    
} cg_vertex_t;

typedef struct {
    cg_list_t* p_vertexes;
} cg_garph_t;

int cg_garph_init(cg_garph_t* p_garph);
int cg_garph_link(const char* p_from_id, const char* p_to_id);
cg_vertex_t* cg_garph_add_vertex(cg_garph_t* p_garph, const char* id);
int cg_garph_search_paths(cg_vertex_t* p_star, cg_vertex_t* p_end, cg_list_t* p_paths)
int cg_garph_recycle(cg_garph_t* p_garph);

#endif 