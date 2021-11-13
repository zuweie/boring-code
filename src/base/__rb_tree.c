/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-11 10:15:37
 * @LastEditTime: 2021-11-13 15:38:28
 * @LastEditors: Please set LastEditors
 */
#include <stdlib.h>
#include "__rb_tree.h"
#include "mem_pool/__mem_pool.h"
#include "container_of.h"
/** tree function **/

 static void __init_rb_tree (rb_tree_t* prb, unsigned char multi) 
 {
    
    /** 初始化 _null 边界节点 **/
    prb->_null.color = _rb_black;
    prb->_null.parent = _null(prb);
    prb->_null.left = _null(prb);
    prb->_null.right = _null(prb);
    
    /** 初始化其他 **/
    prb->_size = 0;
    prb->_root = _null(prb);
    prb->_first = _null(prb);
    prb->_last  = _null(prb);
    prb->multi = multi;
 }

// 找到该节点往下最小的那个节点
static rb_tree_node_t* __tree_minimum (rb_tree_t* prb, rb_tree_node_t* pnode) 
{
    while (pnode != _null(prb) && pnode->left != _null(prb)){
       pnode = pnode->left;
    }
    return pnode;
}

// 找到该下最大的那个节点
static rb_tree_node_t* __tree_maximun (rb_tree_t* prb, rb_tree_node_t* pnode) 
{
    while (pnode != _null(prb) && pnode->right != _null(prb)){
        pnode = pnode->right;
    }
    return pnode;
}



// 找该节点的下一个（比这个大的下一个）
static rb_tree_node_t* __tree_successor (rb_tree_t* prb, rb_tree_node_t* pnode) 
{
    // 如果该节点右不为空（右边子树的值都比这个节点大），则找到右边子树木最小的那那个值就是该节点的下一个。
    if (pnode->right != _null(prb)){
      return __tree_minimum(prb, pnode->right);
    }
    // 若该节点右边为空。则往上找。找到第一个为左孩子的节点就是它的下一个。
    rb_tree_node_t* up = pnode->parent;
    while( up != _null(prb) && pnode == up->right){
      pnode = up;
      up = pnode->parent;
    }
    return up;
}

// 找该节点的前一个（比这个小的最后一个）
static rb_tree_node_t* __tree_predecessor(rb_tree_t* prb, rb_tree_node_t* pnode)
{
    // 如果该节点左不为空（左边子树的值都比这个节点小），则找到左边子树最大的那个一个。
    if (pnode->left != _null(prb)) {
        return __tree_maximun(prb, pnode->left);
    }
    // 如果该节点左边为空。则往上找。找到第一个为右孩子的节点就是它的前一个节点。
    rb_tree_node_t* up = pnode->parent;
    while (up != _null(prb) && pnode == up->left) {
        pnode = up;
        up = pnode->parent;
    }
    return up;
}

static int __tree_left_rotate (rb_tree_t* prb, rb_tree_node_t* px)
{
    // 先动自己的右子树。
    // py为px 的右子树。
    // 将py的左子树变成自己的右子树了。
    // 也就是讲自己的孙子变为儿子了。
    
    rb_tree_node_t *py = px->right;

    // 如果 py 为 nil 节点，则结束旋转。
    if (py == _null(prb)) return -1;

    px->right = py->left;

    py->left->parent = px;

    // 这个位置 px 是 py 老爸。
    // 将px老爸变成py的老爸。也就是将爷爷变成老爸。升级了
    py->parent = px->parent;

    
    if (px->parent == _null(prb)){
        // 这里说明px原来是root，没有parent的。
        prb->_root = py;
    }else {
        // 这里说明px原来不是root，有partent的。把py设置成它px-
        if (px == px->parent->left){
            // px 是他老子的左子树。
            px->parent->left = py;
        }else{
            // px 是他老子的右子树。
            px->parent->right = py;
        }
    }

    py->left = px;
    px->parent = py;
    return 0;
}

static int __tree_right_rotate(rb_tree_t* prb, rb_tree_node_t* px)
{
    rb_tree_node_t *py = px->left;

    // 如果 py 为 nil 节点，则结束旋转
    if (py == _null(prb)) return -1;

    px->left = py->right;
    
    py->right->parent = px;

    py->parent = px->parent;

    if (px->parent == _null(prb)){
        prb->_root = py;
    }else  {
        if (px == px->parent->right) {
            px->parent->right = py;
        } else {
            px->parent->left = py;
        }
    }
    py->right = px;
    px->parent = py;
    return 0;
}

static rb_tree_node_t* __rb_tree_do_search(rb_tree_t* prb, rb_tree_node_t* pnode, type_value_t* find)
{
    if (pnode != _null(prb)) {
        int result = T_cmp(prb->container.type_clazz)(pnode->w, find);
        if (result == 0) {
            return pnode;
        }else if (result == 1) {
            // pnode->node > find, search it on pnode left side
            return __rb_tree_do_search(prb, pnode->left, find);
        }else{
            // pnode->node < find, search it on pnode right side
            return __rb_tree_do_search(prb, pnode->right, find); 
        }
    }
    // 这里找不到就返回边界指针 _null(prb)
    return pnode;
}


static int __rb_tree_insert_fixup (rb_tree_t* prb, rb_tree_node_t* pz)
{
    // 遇到 nullnode 它的颜色是黑色的，就跳出循环了。
    while (pz->parent->color == _rb_red)
    {
        // 能来到这里都违反了性质4 ： 老子是红色。但自己也是红色。
        // pz的老子是左子树

        if (pz->parent == pz->parent->parent->left)
        {
            // py 是 pz 老子的右兄弟。也就是其右叔叔
            rb_tree_node_t *py = pz->parent->parent->right;

            if (py->color == _rb_red)
            {
                // 如果其叔叔是红色 case 1
                // 将pz老子染为黑色
                pz->parent->color = _rb_black;
                // 将pz的叔叔py染为黑色。
                py->color = _rb_black;
                // 将pz的祖父染为红色。
                pz->parent->parent->color = _rb_red;
                // pz的指针跳到祖父哪里，进入下一伦循环。
                pz = pz->parent->parent;
                // 下一伦循环的时候自己也是红色。
                // 进去了就违反了性质4。
            }
            else
            {
                // case 2 3
                if (pz == pz->parent->right)
                {
                    // case 2 算法导论的 p171 页。
                    // 如果pz自己是右子树。这里还包含了一个隐藏条件就是，pz的右叔叔是黑色的。
                    // 如果pz的右叔叔是红色的，就会进入上面case1的处理。
                    // pz的指针跳到其父亲哪里。

                    pz = pz->parent;
                    // 然后左转一下。将pz。 这里旋转一下就是将其撸直了，让其出现case 3的情况。
                    __tree_left_rotate(prb, pz);
                }
                // case 3 这里无论如何都要做出调整的。
                pz->parent->color = _rb_black;
                pz->parent->parent->color = _rb_red;
                __tree_right_rotate(prb, pz->parent->parent);
            }
        }
        else
        {
            // py 的 pz 是左子树， 也就是左叔叔。
            rb_tree_node_t *py = pz->parent->parent->left;
            if (py->color == _rb_red)
            {
                //如果其叔叔是红色 CASE 1
                // 将pz的老子染为黑色。
                pz->parent->color = _rb_black;
                // 将pz的左叔叔染为黑色
                py->color = _rb_black;
                // 将祖父染为红色
                pz->parent->parent->color = _rb_red;
                // pz的指针跳到祖父哪里。妈的如果祖父是root
                pz = pz->parent->parent;
                // 下一伦循环的时候自己也是红色。
                // 进去了就违反了性质4。
            }
            else
            {
                // CASE 2 3
                if (pz == pz->parent->left)
                {
                    // 如果pz是左子树。这里还包含了一个隐藏条件，pz的左叔叔。
                    // case 2 算法导论的 p171 页。
                    // 如果pz自己是右子树。这里还包含了一个隐藏条件就是，pz的右叔叔是黑色的。
                    // 如果pz的右叔叔是红色的，就会进入上面case1的处理。
                    // pz的指针跳到其父亲哪里。
                    pz = pz->parent;
                    __tree_right_rotate(prb, pz);
                }

                pz->parent->color = _rb_black;
                pz->parent->parent->color = _rb_red;
                __tree_left_rotate(prb, pz->parent->parent);
            }
        }
    }
    prb->_root->color = _rb_black;
    return 0;
}

static rb_tree_node_t* __rb_tree_create_node (rb_tree_t* prb, type_value_t* t) {
    rb_tree_node_t* pnode = \
        allocate( \
            prb->container.mem_pool, sizeof (rb_tree_node_t) \
            + T_size(prb->container.type_clazz) \
        );

    pnode->parent = _null(prb);
    pnode->left   = _null(prb);
    pnode->right  = _null(prb);
    T_setup(prb->container.type_clazz)(pnode->w, t, 0);
    return pnode;
}

static int __rb_tree_insert (container_t* container, iterator_t pos, type_value_t* t) 
{
    rb_tree_t* prb = (rb_tree_t*)container;
	rb_tree_node_t* py = _null(prb);
	rb_tree_node_t* px = prb->_root; 
    
    /* alloc */
    // 找位置
    while(px != _null(prb)) {
        py = px;
        if ( T_cmp(container->type_clazz)(px->w, t) == 1 ){
            // 小于的情况
        	px = px->left;
        }else {
            
            if (prb->multi || T_cmp(container->type_clazz)(px->w, t) == -1) {
                // 大于或者允许多健值的情况
                px = px->right;
            } else {
                // 把旧值进行跟新
                // if (prb->conflict_fix) {
                //     prb->conflict_fix(px->w, t);
                // } else {
                //     type_value_cpy(px->w, t, prb->container.type_def.ty_size);
                //     //prb->container.type_def.ty_adapter.bit_cpy(px->w, t);
                // }
                T_setup(container->type_clazz)(px->w, t, 1);
                return 1;
            }
        }
    }
    rb_tree_node_t* pz = __rb_tree_create_node(prb, t);
    pz->parent = py;
    // 挂叶子
    if (py == _null(prb)){
    	prb->_root = pz;
    }else if (T_cmp(container->type_clazz)(pz->w, py->w)== -1){
    	py->left = pz;
    }else{
        py->right = pz;
    }

    pz->left = _null(prb);
    pz->right = _null(prb);
    pz->color = _rb_red;
    prb->_size++;
    // 修改 first 或者 last
    __rb_tree_insert_fixup(prb, pz);

    /* 更新 first 与 last */
    if (prb->_first == _null(prb)) {
        prb->_first = __tree_minimum(prb, prb->_root);
    } else if (prb->_first->left != _null(prb)) {
        prb->_first = prb->_first->left;
    }

    if (prb->_last == _null(prb)) {
        prb->_last = __tree_maximun(prb, prb->_root);
    } else if (prb->_last->right != _null(prb)) {
        prb->_last = prb->_last->right;
    }
    
    return 0;
}

static int __rb_tree_remove_fixup (rb_tree_t* prb, rb_tree_node_t* px)
{
    /*
     * 麻痹，好鸡巴复杂的逻辑啊。到底是怎么回事。谁他妈的发明红黑树啊。
     * 这里要好好说明算法的过程：
     * 1 删除z时候，y所在的位置：在rbtree中y一般是z的后置，于是y就差不多在底层：若y有右孩子那么y就在倒数第二层，x在底层。y没有右孩子y就在底层。
     *   详细见tree的删除过程
     *                            y
     *                            |
     *                            \
     *                             x (可能是NIL)
     *
     * 2 角色，y 是黑色的。x是他孩子（可能是NIL，黑色的)。在fixup的函数里面 y z是看不到的。他们的delete函数里面。
     *       y,被删掉的z的后继，是补充到z的位置上去的。
     *       x,y的左孩子或者右孩子。是补充到y的位置上去的。
     *
     * 3 分析，当y是黑色，被补充到z的位置上去的时候，y所在的位置就少了一个黑色。于是便不平衡。于是假设补充到y位置上的x继承y的黑色。也就是x有两个颜色。
     *   如果x是红色（那时x是红黑色）。那就皆大欢喜。直接将其变为黑色，（y的黑色便被保留了下来。大循环结束）。但如果x是黑色。他妈的就很鸡巴麻烦。
     *   x就有两个黑色。然后算法就是通过各种变态的旋转，旋转过程，保持颜色的平衡。将两个黑色往上扔，扔到再遇到红的节点
     *   (直接拉黑，等于那个少的分支黑加一，算法结束）。若是一直往上都没遇上红的。那让其到根部。那么从根出发的，多少重黑色都没问题。
     *
     */

    // 这里的px很可能是NULL
    // 若为NULL 则 那个
    rb_tree_node_t* pw = _null(prb);
    while (px != prb->_root && px->color == _rb_black) {

        if (px == px->parent->left){
            // 如果px是左孩子。
            // 取pw为px的右兄弟。
            pw = px->parent->right;
            
            if (pw->color == _rb_red){
                //CASE 1
                //若pw是红色。将其染为黑色
                pw->color = _rb_black;
                // 将其老子染为红色。
                px->parent->color = _rb_red;
                // 将老子转下来. px->parent 其实和 pw->parent 是一样的。
                __tree_left_rotate(prb, px->parent);
                // 左转了一下以后pw变成px的老子的老子。
                // 再把pw移下来变回px的右兄弟。
                pw = px->parent->right;
            }

            if (pw->left->color == _rb_black && pw->right->color == _rb_black) {
                // 若是pw的两个儿子都是黑色的。CASE 2
                // 将其pw染为红色
                pw->color = _rb_red;
                px = px->parent;
            } else {
                // CASE 3
                if (pw->right->color == _rb_black) {
                    // 将CASE 3 转变为CASE 4
                    // 这里有一个右孩子是黑色，且左孩子是红色。
                    // 就将其红色的左孩子染为黑色。
                    pw->left->color = _rb_black;
                    // 然后将自己染为红色
                    pw->color = _rb_red;
                    // 把自己的左孩子右转上去。
                    __tree_right_rotate(prb, pw);
                    pw = px->parent->right; 
                }
                // CASE 4
                pw->color = px->parent->color;
                px->parent->color = _rb_black;
                pw->right->color = _rb_black;
                __tree_left_rotate(prb, px->parent);
                px = prb->_root;
            } // else
        }else{
            // px 为右孩子
            // pw 为其左叔叔
            pw = px->parent->left;
            if (pw->color == _rb_red){
                // CASE 1
                pw->color = _rb_black;
                px->parent->color = _rb_red;
                __tree_right_rotate(prb, px->parent);
                pw = px->parent->left;
            }

            if (pw->left->color == _rb_black &&  pw->right->color == _rb_black){
                // CASE 2
                pw->color = _rb_red;
                px = px->parent;
            }else{
                // CASE 3
                if (pw->left->color == _rb_black){
                    pw->right->color = _rb_black;
                    pw->color = _rb_red;
                    __tree_left_rotate(prb, pw);
                    pw = px->parent->left;
                }
                // CASE 4
                pw->color = px->parent->color;
                px->parent->color = _rb_black;
                pw->left->color = _rb_black;
                __tree_right_rotate(prb, px->parent);
                px = prb->_root;
            } // else
        }
    } // big while
    px->color = _rb_black;
    return 0;
}

static int __rb_tree_remove (container_t* container, iterator_t pos, void* rdata)
{ 
    rb_tree_t* prb = (rb_tree_t*)container;
    rb_tree_node_t* pz = (rb_tree_node_t*)container_of(pos.reference, rb_tree_node_t, w);

    if (pz != _null(prb)){
        
        // 更新 tree 的 first 和 last
        // if (prb->_first == pz) {
        //     prb->_first = __tree_successor(prb, pz);
        // } 
        
        // if (prb->_last == pz) {
        //     prb->_last = __tree_predecessor(prb, pz);
        // }

        rb_tree_node_t *py = _null(prb);
        rb_tree_node_t *px = _null(prb);
        if (pz->left == _null(prb) || pz->right == _null(prb)){
            py = pz;
        } else {
            py = __tree_successor(prb, pz);
        }

        if (py->left != _null(prb)){
            px = py->left;
        }else{
            px = py->right;
        }

        px->parent = py->parent;

        if (py->parent == _null(prb)){
            prb->_root = px;
        }else{

            if (py == py->parent->left){
                py->parent->left = px;
            }else{
                py->parent->right = px;
            }
        }

        // 交换两个的值
        if (py != pz){
            type_value_swap(py->w, pz->w, T_size(container->type_clazz));
        }

        if (py->color == _rb_black){
            __rb_tree_remove_fixup(prb, px);
        }

        // 更新 first 于 last 的关系
        if (prb->_first == py) {
            prb->_first = __tree_minimum(prb, prb->_root);
        }

        if (prb->_last == py) {
            prb->_last = __tree_maximun(prb, prb->_root);
        }

        // 返回值。
        if (rdata) type_value_cpy(rdata, py->w, T_size(container->type_clazz));
        
        deallocate(prb->container.mem_pool, py);
        prb->_size--;
        //return rdata;
        return 0;
    }else{
        // 恶意删除，空节点，返回-1;
        return -1;
    }
}
/** tree function **/

/** iterator function **/

//static iterator_t _get_iter(void* refer, void* tree);

static int __rb_tree_move(iterator_t* it, int step) 
{
    rb_tree_node_t* pnode = container_of(it->reference, rb_tree_node_t, w);
    rb_tree_t* tree       = it->container;
    
    for (int next = step; next; step>0?next--:next++){
        /* code */
        if (step>0) pnode = __tree_successor(tree, pnode);
        else if (step<0) pnode = __tree_predecessor(tree, pnode);
    }
    it->reference = pnode->w;
    return 0;
}

/** container function **/
// 中序 遍历 
static iterator_t __rb_tree_first(container_t* container) 
{
    rb_tree_t* tree = container;
    /* 用计算的方式来获取第一个随着节点的增加变得慢 */
    //rb_tree_node_t* pnode = __tree_minimum(tree, tree->_root);
    rb_tree_node_t* pnode = tree->_first;
    return __iterator(pnode->w, container);
}

static iterator_t __rb_tree_last(container_t* container) 
{
    rb_tree_t* tree = container;
    /* 用计算方式来获取最后一个节点，随着节点的数量增多变慢 */
    //rb_tree_node_t* pnode = __tree_maximun(tree, tree->_root);
    rb_tree_node_t* pnode = tree->_last;
    return __iterator(pnode->w, container);
}

static iterator_t __rb_tree_search(container_t* container, iterator_t offset, type_value_t* find, int (*compare)(type_value_t*, type_value_t*)) 
{
    rb_tree_t* tree = container;
    rb_tree_node_t* p;
    if (iterator_is_null(offset)) {
        p = __rb_tree_do_search(tree, tree->_root, find);
    } else {
        // 从 offset 的位置开始搜索。
        rb_tree_node_t* node = container_of(offset.reference, rb_tree_node_t, w);
        p = __rb_tree_do_search(tree, node, find);
    }
    return __iterator(p->w, container);
}


static size_t __rb_tree_size(container_t* container)
{
    return ((rb_tree_t*)container)->_size;
}


container_t* rb_tree_create(T_clazz* __type_clazz, unsigned char multi)
{
    container_t* tree = (rb_tree_t*) malloc( sizeof(rb_tree_t) );
    pool_t* _mem_pool = alloc_create(0);
    initialize_container(
        tree, 
        __rb_tree_first, 
        __rb_tree_last, 
        __rb_tree_move,
        __rb_tree_search,
        __rb_tree_insert, 
        __rb_tree_remove, 
        __rb_tree_size,
        __type_clazz,
        _mem_pool
    );
    __init_rb_tree(tree, multi);
    return tree;
}

int rb_tree_destroy(container_t* tree) {
    alloc_destroy(tree->mem_pool);
    free(tree);
    return 0;
}

iterator_t rb_tree_root(rb_tree_t* tree) {
    return __iterator(tree->_root->w, tree);
}

iterator_t rb_tree_null(rb_tree_t* tree) 
{
    return __iterator(_null(tree)->w, tree);
} 
/** container function **/
