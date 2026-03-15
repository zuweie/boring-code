/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-02-19 15:08:47
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-03-15 16:16:15
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_calflow.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "cg_debug.h"
#include "cg_hash.h"
#include "cg_node.h"
#include "cg_graph.h"
#include "cg_operand.h"
#include "cg_operator.h"
#include "cg_ticket.h"
#include "cg_calflow.h"

static int __marker_hash(void* key) 
{
    unsigned int hash = 0;
    const char* str = key;
    while(*str) {
        hash = (hash * 31 + *str) % SLOT_NUM;
        str++;
    }
    return hash;
}

static int __marker_cmp(void* k1, void* k2) 
{
    return strcmp(k1, k2);
}

static int __recycle_path(cg_ref_t path) 
{
    cg_list_t* list = path;
    cg_list_recycle(list, NULL);
    return 0;
}

static int __prepare_tickets (cg_node_t* znode, cg_hash_t* marker) 
{
    // 拥有小弟，才能给小弟派导数券
    if (CG_NODE_TYPE(znode) == e_operand znode->node_type == e_operand) {
        if (cg_node_is_respect(znode)) {

            cg_operator_t*  operator = cg_operand_get_operator(znode);
            cg_list_node_t* first    = CG_LIST_TOP(operator->_base.vertex.in);

            while (first != CG_LIST_HEAD(operator->_base.vertex.in)){

                cg_node_t* sub_znode = first->ref;
                cg_ticket_give(marker, operator, sub_znode);
                first = first->prev;
            }

            // 找一下小弟的自变量。
            first = CG_LIST_TOP(operator->_base.vertex.in);
            while (first != CG_LIST_HEAD(operator->_base.vertex.in)){

                cg_node_t* sub_znode = first->ref;
                __prepare_tickets(sub_znode, marker);
            }
        }
        return 0;
    } else {
        CG_DEBUG("ERROR: <__prepare_tickets>, znode is not a operand [%d]\n", __LINE__);
    }
    return -1;
}

static int __do_calculate(cg_node_t* znode, cg_hash_t* marker)
{

    if ( CG_NODE_IS_OPERAND(znode) ) {
        
        if (!cg_hash_has(marker, znode->vertex.id)) {

            // 此节点的数据为旧的，需要计算。
            if (cg_node_is_respect(respect)) {

                // 此节点需要经过 operator 节点计算
                cg_node_t*      sub_znode;
                cg_operator_t*  operator = cg_operand_get_operator(znode);
                cg_list_node_t* first    = CG_LIST_TOP(operator->_base.vertex.in);

                while (first != CG_LIST_HEAD(operator->_base.vertex.in)) {

                    sub_znode = first->ref;
                    __do_calculate(sub_znode, marker);
                    first = first->prev;
                }

                operator->calculate(operator, respect->x);
            }
            //  计算完了，更新这个数据的版本。
            cg_hash_set(marker, znode->vertex.id, 1);
        }
        return 0;
    } else {
        CG_DEBUG("ERROR: <__do_calculate>, znode is not a operand [%d]\n", __LINE__);
    }
 
    return -1;
}

static int __do_differentiate(cg_node_t* znode, cg_hash_t* marker)
{   
    if ( CG_NODE_IS_OPERAND(znode) ) {
        cg_operand_t* repect = (cg_operand_t *)znode;
        
        if (cg_node_is_respect(znode)) {

            cg_operator_t*   operator = cg_operand_get_operator(znode);
            cg_list_node_t*  first;
            cg_node_t*       sub_znode;
            cg_ticket_t*     ticket;
            int ticket_found;

            first = CG_LIST_TOP(operator->_base.vertex.in);

            while (first != CG_LIST_HEAD(operator->_base.vertex.in)) {

                sub_znode = first->ref;
                ticket_found = cg_ticket_get(marker, operator, sub_znode, &ticket);
                if (ticket_found == 1 && !cg_ticket_is_used(ticket)){
                    if (!cg_ticket_is_used(ticket)) {
                        operator->differentiate(operator, sub_znode, repect->Gx);
                        cg_ticket_use(ticket);
                    } else {
                        CG_DEBUG("WARNING: <__do_differentiate>, ticket should not be used [%d]\n", __LINE__);
                    }
                } 
                first = first->prev;
            }

            first = CG_LIST_TOP(operator->_base.vertex.in);
            while (first != CG_LIST_HEAD(operator->_base.vertex.in)){

                sub_znode = first->ref;

                if (cg_ticket_is_clean(marker, sub_znode)) {

                    __do_differentiate(sub_znode, marker);
                    
                }
            }
            return 0;
        }
    } else {
        CG_DEBUG("ERROR: <__do_differentiate>, znode is not is operand [%d]\n", __LINE__);
    }

    return -1;
}

int cg_calculate(cg_node_t* znode)
{
    int ret = -1;
    if (CG_NODE_IS_OPERAND(znode)) {
        cg_hash_t* update_marker = cg_hash_create(__marker_hash, __marker_cmp);
        ret = __do_calculate(znode, update_marker);
        cg_hash_recycle(update_marker, NULL);
    } else {
        CG_DEBUG("ERROR: <cg_calculate>, znode is not a operand [%d]\n", __LINE__);
    }
    return ret;
}


/**
 * @brief 这里我发明了一种 ticket 算法。
 * 1. 我从一个 operand A 的节点出发，使用深度优先算法，遍历其地下所有的节点。经过一个 operand X 节点，便给这个 operand X 节点派发，一张 ticket。
 * 若果有多次经过这个 operand X 节点，它将获得多个 ticket。
 * 2. 开始正式的做 derivative 的时候，我们会将遇到的要计算 gradient 的 operand 的时候，我们将检查他的上级节点是否与 ticket 的中的 handle 是否一致。
 * 如果不一致，那么说明这个计算图的拓扑结果是错误的。当完成了这个 derivative 后，对应的 ticket 删除，然后更新数据节点的 version。那么这个节点的 derivate 
 * 工作就完成了。然后将这个节点往下一级的尚未对其子节点的节点重复上述动作。
 * 
 * 
 * @param znode 
 * @param version 
 * @param params 
 * @return int 
 */
int cg_differentiate(cg_node_t* znode)
{
    cg_hash_t* ticket_marker = cg_hash_create(__marker_hash, __marker_cmp);
    int ret = __do_differentiate(znode, ticket_marker);
    cg_hash_recycle(ticket_marker, cg_ticket_recycle);
    return 0;
}

/**
 * @brief 这个函数是自定 z 到 x 的偏导。
 * 1. 使用深度优先算法，找到所有 z 到 x 的偏导路径，
 * 2. 给路径上所有的数据节点派上 ticket。
 * 3. 开始进行编导。
 * 
 * @param znode 
 * @param x 
 * @return int 
 */
int cg_derivateive_to(cg_node_t* z, cg_node_t* x)
{
    cg_node_t* operator;
    cg_node_t* znode;
    cg_list_node_t* path_first;
    cg_list_node_t* znode_first;
    cg_list_t* znodes;

    cg_list_t* paths = cg_list_create();
    cg_hash_t* marker = cg_hash_create(__marker_hash, __marker_cmp);

    // TODO 1: 我们需要派发 ticket 给偏导路径上的 operand
    cg_graph_search_paths(x, z, paths);

    path_first = CG_LIST_TOP(paths);
    while (path_first != CG_LIST_HEAD(paths)) {

        znodes = path_first->ref;

        // 搜索到的路径是 从起点到终点，需要将其反过来，
        cg_list_revert(znodes);

        // 终点并不需要配发ticket，所以直接跳到下一个节点。
        znode_first = CG_LIST_TOP(znodes)->prev;

        while (znode_first != CG_LIST_HEAD(znodes)) {

            operator == NULL;
            znode = znode_first->ref;
            if (znode->node_type == e_operator) {
                operator = znode;
                znode_first = znode_first->prev;
                znode = znode_first->ref;
            } 

            if (znode->node_type == e_operand && operator != NULL) {
                
                cg_ticket_give(marker, operator, znode);

            } else {
                // big error here
                return -1;
            }
            znode_first = znode_first->prev;
        }
        
        path_first = path_first->prev;
    }

    // TODO 2: 做偏导。
    __do_differentiate(znode, marker);

    // TODO 3: 释放资源。
    cg_list_recycle(paths, __recycle_path);
    cg_hash_recycle(marker, cg_ticket_recycle);
    
    return 0;
}