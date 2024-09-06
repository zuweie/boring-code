<!--
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2024-09-02 13:52:46
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2024-09-06 11:14:48
 * @FilePath: /boring-code/src/deep_learning/compute_graph/readme.md
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
-->
# 计算图 （compute graph）
## 简单介绍
- 1 计算图，主要用于组织各种神经网络进行前向传播与后向传播算法。
- 2 用到图、盏，这两种数据结构
- 3 计算图的原理简单，但是实现起来工作量巨大特别巨大。
  
## 完成情况
- v0.1 只完成了前向传播和后向传播的大概框架，要实现真正的导数功能，需要在构建不同神经网络的时候，实现不同节的 fp、bp、update_payload等的函数实现。下一步其实要做的是，必须根据各个节点间的类别，使用一个对照表，把上下级需要的 fp 与 bp 函数加入表中，那么在进行 backward_propagation 的时候，直接就从表中取得相关的 fp 与 bp 函数，那就不用手动地去重复实现各类 fp 与 bp 函数，十分繁琐。现在由于有其他的项目急于上线，所一关与这一部分的编码留到下一个版版实现了。当前这个 v0.1 就只能做做训的模拟与仿真。