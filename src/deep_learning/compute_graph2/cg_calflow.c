/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-02-19 15:08:47
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-02-21 23:26:02
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_calflow.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "cg_debug.h"
#include "cg_hash.h"
#include "cg_node.h"
#include "cg_operand.h"
#include "cg_operator.h"
#include "cg_calflow.h"

static int __do_calculate(cg_node_t* znode, int version, cg_ref_t params)
{
    cg_operand_t* operand = (cg_operand_t*) znode;

    if (operand->version < version) {
        // 此节点的数据为旧的，需要计算。
        if (cg_list_size(znode->vertex.in) == 1) {
            // 此节点需要经过 operator 节点计算
            cg_operator_t* operator = cg_list_get(znode->vertex.in, 0);
            cg_list_node_t* first   = CG_LIST_TOP(operator->_base.vertex.in);

            while (first != CG_LIST_HEAD(operator->_base.vertex.in)) {
                __do_calculate(first, version, params);
                first = first->prev;
            }
            
            operator->calculate(znode, operator->_base.vertex.in, params);
        }
        operand->version = version;
    } 
    return 0;
}

static int __distribute_tickets (cg_node_t* znode, cg_hash_t* tickets) 
{
    cg_node_t* operator_node = cg_list_get(znode->vertex.in, 0);
    
    cg_list_node_t* first   = CG_LIST_TOP(operator_node->vertex.in);

    while (first != CG_LIST_HEAD(operator_node->vertex.in))
    {
        cg_node_t* operand = first->ref;
        cg_ticket_t* new_ticket = cg_ticket_create(operator_node->vertex.id);

        if (cg_hash_has(tickets, operand->vertex.id)) {
            // 插在屁股后面，
            cg_ticket_t* ticket = cg_hash_get(tickets, operand->vertex.id);
            ticket->next = ticket;
        } else {
            // 生成一个新的，放在里面去。
            cg_hash_set(tickets, operand->vertex.id, new_ticket);
        }
        /* code */
    }
    
}

static int __do_derivative(cg_node_t* znode, int version, cg_hash_t* tickers, cg_ref_t params)
{
    return 0;
}

int cg_calculate(cg_node_t* znode, int version, cg_ref_t params)
{
    if (znode->node_type == e_operand && cg_list_size(znode->vertex.in) <= 1) {
        return __do_calculate(znode, version, params);
    } 
    return -1;
}


/**
 * @brief 这里我发明了一种 ticket 算法。
 * 第一步我从一个 operand A 的节点出发，使用深度优先算法，遍历其地下所有的节点。经过一个 operand X 节点，便给这个 operand X 节点派发，一张 ticket。
 * 若果有多次经过这个 operand X 节点，它将获得多个 ticket。
 * 第二步，开始正式的做 derivative 的时候，我们会将遇到的要计算 gradient 的 operand 的时候，我们将检查他的上级节点是否与 ticket 的中的 handle 是否一致。
 * 如果不一致，那么说明这个计算图的拓扑结果是错误的。当完成了这个 derivative 后，对应的 ticket 删除，然后更新数据节点的 version。那么这个节点的 derivate 
 * 工作就完成了。然后将这个节点往下一级的尚未对其子节点的节点重复上述动作。
 * 
 * 简化版，可以将 ticket 换成计数，但是无法验证上家是否一致。
 * 
 * @param znode 
 * @param version 
 * @param params 
 * @return int 
 */
int cg_derivative(cg_node_t* znode, int version, cg_ref_t params)
{
    if (znode->node_type == e_operand && cg_list_size(znode->vertex.in) <= 1) {
         
    }
    return 0;
}

int cg_derivateive_to(cg_node_t* znode, cg_node_t* x)
{
    return 0;
}