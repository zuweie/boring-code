/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-02-19 15:08:47
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-09-06 09:54:25
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
#include "cg_flow.h"

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

static int __recycle_flow(cg_ref_t flow) 
{
    cg_list_t* list = flow;
    cg_list_recycle(list, NULL);
    return 0;
}

static int __give_tickets (cg_node_t* znode, cg_hash_t* marker) 
{
    cg_operator_t*  operator = cg_operand_get_producer(znode);

    if (operator) {
        cg_list_node_t* first    = CG_LIST_TOP(((cg_node_t*)operator)->vertex.in);

        while (first != CG_LIST_HEAD( ((cg_node_t*)operator)->vertex.in)){

            cg_node_t* sub_znode = first->ref;

            if ( CG_NODE_IS_OPERAND(sub_znode) ) {
                cg_ticket_give(marker, operator, sub_znode);
                first = first->prev;
            } else {
                CG_DEBUG("ERROR <%d@%s>: %s should be a operand\n", __LINE__, __FILE__, sub_znode->vertex.id);
                return -1;
            }
        }

        // 找一下小弟的自变量。
        first = CG_LIST_TOP( ((cg_node_t*)operator)->vertex.in);
        while (first != CG_LIST_HEAD( ((cg_node_t*)operator)->vertex.in)){
            cg_node_t* sub_znode = first->ref;
            __give_tickets(sub_znode, marker);
            first = first->prev;
        }
        return 0;
    }
    return -1;
}

static int __do_calculate(cg_operand_t* znode, cg_hash_t* marker)
{
    int ret = 0;
    if (!cg_hash_has(marker, CG_NODE_ID(znode))) {
        // 尚未经过计算需要计算
        if ( CG_NODE_IS_OPERAND(znode) ) {
            cg_operator_t*  operator = cg_operand_get_producer(znode);
            if (operator) {
                // Todo 1: 检测本 operator 下的 sub operand 都计算完毕。
                cg_list_node_t* first = CG_LIST_TOP( CG_NODE_IN(operator) );

                while (first != CG_LIST_HEAD( CG_NODE_IN(operator) ))  {

                    cg_operand_t* sub_node = first->ref;

                    ret = __do_calculate(sub_node, marker);
                    if (ret) {
                        CG_DEBUG("ERROR <%d@%s>: sub calculate error(%d)\n", __LINE__, __FILE__, ret);
                        return ret;
                    }
                    first = first->prev;
                }
                // TODO 2: 计算完所有的 sub 
                ret = operator->calculate(operator, znode);
            } 

            // 一切正常，这个节点计算完了，将它 mark 一下, 表示这个已经是最新的节点了
            if (!ret)
                cg_hash_set(marker, CG_NODE_ID(znode), 1L );
            
        } else {
            CG_DEBUG("ERROR <%d@%s>: %s should be a operand\n", __LINE__, __FILE__, CG_NODE_ID(znode));
            ret = -1;
        }

    }
    return ret;
}

/**
 * @brief 复杂到嗨咁的偏导算法
 * 
 * @param znode 
 * @param marker 
 * @return int 
 */
static int __do_differentiate(cg_node_t* znode, cg_hash_t* marker)
{   
    int ret = 0;
    if ( CG_NODE_IS_OPERAND(znode) ) {
        
        cg_operator_t* operator = cg_operand_get_producer(znode);
        if (operator) {
            cg_list_node_t*  first;
            cg_node_t*       sub_znode;
            cg_ticket_t*     ticket;
            int ticket_found;

            first = CG_LIST_TOP(CG_NODE_IN(operator));

            while (first != CG_LIST_HEAD( CG_NODE_IN(operator) )) {

                sub_znode = first->ref;
                ticket_found = cg_ticket_get(marker, operator, sub_znode, &ticket);

                if (ticket_found == 1 && !cg_ticket_is_used(ticket)){
                    if (!cg_ticket_is_used(ticket)) {
                        ret = operator->differentiate(operator, sub_znode, znode);
                        if (ret) {
                            CG_DEBUG("ERROR <%d@%s>: differentiate error(%d)\n", __LINE__, __FILE__, ret);
                            return ret;
                        }
                        cg_ticket_use(ticket);
                    } else {
                        CG_DEBUG("INFO <%d@%s>: ticket has been used\n", __LINE__, __FILE__);
                    }
                } 
                first = first->prev;
            }

            // 继续往下一层的节点做偏导
            first = CG_LIST_TOP( CG_NODE_IN(operator) );
            while (first != CG_LIST_HEAD( CG_NODE_IN(operator))){
                sub_znode = first->ref;
                if (cg_ticket_is_clean(marker, sub_znode)) {
                    ret = __do_differentiate(sub_znode, marker);
                    if (ret) {
                        CG_DEBUG("ERROR <%d@%s>: differentiate ret error(%d)\n", __LINE__, __FILE__, ret);
                        return ret;
                    }
                }
                first = first->prev;
            }
        }

    } else {
        CG_DEBUG("ERROR <%d@%s>: %s is not is operand\n", __LINE__, __FILE__,CG_NODE_ID(znode));
        ret = -1;
    }

    return ret;
}

// 计算所有 zonde 下所有贡献者的对他的贡献。
int cg_calculate(cg_operand_t* znode)
{
    int ret = -1;
    if (CG_NODE_IS_OPERAND(znode)) {
        cg_hash_t* update_marker = cg_hash_create(__marker_hash, __marker_cmp);
        ret = __do_calculate(znode, update_marker);
        cg_hash_recycle(update_marker, NULL);
    } else {
        CG_DEBUG("ERROR <%d@%s>: znode is not a operand\n", __LINE__, __FILE__);
    }
    return ret;
}


/**
 * @brief 这里我发明了一种 ticket 算法。
 * 0. 从 znode 出发，计算所有关于 zonde 对它的变量贡献者的偏导
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
int cg_derivative(cg_operand_t* znode)
{
    int ret = -1;
    if ( CG_NODE_IS_OPERAND(znode) ) {
        cg_hash_t* ticket_marker = cg_hash_create(__marker_hash, __marker_cmp);
        __give_tickets(znode, ticket_marker);
        ret = __do_differentiate(znode, ticket_marker);
        cg_hash_recycle(ticket_marker, cg_ticket_recycle);
    } else {
        CG_DEBUG("ERROR <%d@%s>: znode is not a operand\n", __LINE__, __FILE__);
    }

    return ret;
}

/**
 * @brief 这个函数是 z 对某个 x 的偏导。
 * 1. 使用深度优先算法，找到所有 z 到 x 的偏导路径，
 * 2. 给路径上所有的数据节点派上 ticket。
 * 3. 开始进行编导。
 * 
 * @param znode 
 * @param x 
 * @return int 
 */
int cg_derivative_to(cg_operand_t* z, cg_operand_t* to_x)
{
    if (CG_NODE_IS_OPERAND(z) && CG_NODE_IS_OPERAND(to_x)) {
        cg_node_t* operator;
        cg_node_t* znode;
        cg_list_node_t* flow_fist;
        cg_list_node_t* znode_first;
        cg_list_t* derviative_flow;

        cg_list_t* flows  = cg_list_create();
        cg_hash_t* marker = cg_hash_create(__marker_hash, __marker_cmp);

        // 查找 x --> z 的所有路径，这些路径上的变量都要做偏导
        cg_graph_search_paths(to_x, z, flows);

        flow_fist = CG_LIST_TOP(flows);
        while (flow_fist != CG_LIST_HEAD(flows)) {

            derviative_flow = flow_fist->ref;

            // 搜索到的路径是 从起点到终点，需要将其反过来，
            cg_list_revert(derviative_flow);

            // 终点并不需要配发ticket，所以直接跳到下一个节点。
            znode_first = CG_LIST_TOP(derviative_flow)->prev;

            while (znode_first != CG_LIST_HEAD(derviative_flow)) {

                operator = NULL;
                znode    = znode_first->ref;
            
                if ( CG_NODE_IS_OPERATOR(znode) ) {
                    operator    = znode;
                    znode_first = znode_first->prev;
                    znode       = znode_first->ref;
                } 

                if ( CG_NODE_IS_OPERAND(znode) && operator != NULL) {
                
                    cg_ticket_give(marker, operator, znode);

                 } else {
                    // big error here
                    CG_DEBUG("Error <%d@%s>:  big error here\n", __LINE__, __FILE__);
                    return -1;
                }
                znode_first = znode_first->prev;
            }
        
            flow_fist = flow_fist->prev;
        }

        // TODO 2: 做偏导, 这里只会对拥有 ticket 的 operand 进行偏导。
        __do_differentiate(znode, marker);

        // TODO 3: 释放资源。
        cg_list_recycle(flows, __recycle_flow);
        cg_hash_recycle(marker, cg_ticket_recycle);
        return 0;
    }
    CG_DEBUG("Error <%d@%s>: %s or %s is not operand\n", __LINE__, __FILE__, CG_NODE_ID(z), CG_NODE_ID(to_x));
    return -1;
}