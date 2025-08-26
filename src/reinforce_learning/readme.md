<!--
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-08-22 10:02:30
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-08-22 10:18:04
 * @FilePath: /boring-code/src/reinforce_learning/readme.md
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
-->
# bilbil 赵世钰的强化学习，模拟编程。
这是在 bilbil 中学习赵世钰老师的《强化学习的数学原理》公开课后的编程模拟，如果不去做这个编程模拟，根本感觉学不会。这些编程模拟公开课中的例子会从第三课开始。

第一部分是 model-base 的 value iteration 和 policy iteration 算法开始。需要搞懂问题如下：
- 如何理解 state value，以及如何去计算 state value。
- 如何理解 action value，以及如何计算 action value。
- 在得到最大的 action value 后，如何跟新 state value 和 policy。

第二部分是 model-free 的 MC（蒙地卡罗估计算法），要搞懂如下问题：
- action 如何从 model-base 转向 model-free 的。
- action 的 exception 的是如何计算的，视频里说的是使用蒙地卡罗采样，得到的 exception。
  
现在暂时只看到第五章，已经懵懵懂懂的。现在需要通过写代码的方式把这五章的内容搞懂。
