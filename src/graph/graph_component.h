/*
 * @Author: your name
 * @Date: 2020-12-01 14:41:05
 * @LastEditTime: 2020-12-01 16:20:33
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/graph/graph_component.h
 */
#ifndef _GRAPH_COMPONENT_H_
#define _GRAPH_COMPONENT_H_

static int
GAPRH_MATCH_VERTEX(Tv v1, Tv v2) 
{
    vertex_t* pv = t2p(v1);
    return Tv_equl(pv->vertex_id, v2);
}

static int
GRAPH_MATCH_PATH(Tv v1, Tv v2) 
{
    path_t* path = t2p(v1);
    return Tv_equl(path->to->vertex_id, v2);
}

#endif 