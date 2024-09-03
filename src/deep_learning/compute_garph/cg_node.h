/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2024-09-02 13:57:43
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2024-09-03 09:27:15
 * @FilePath: /boring-code/src/deep_learning/compute_garph/cg_node.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#ifndef __CG_NODE_H__
#define __CG_NODE_H__

typedef void* cg_ref_t;
typedef struct _cg_node cg_node_t;

struct _cg_node {
    cg_ref_t ref;
    cg_node_t* prev;
};

#endif