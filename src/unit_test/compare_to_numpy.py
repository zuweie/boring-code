'''
Author: zuweie jojoe.wei@gmail.com
Date: 2025-06-07 21:35:15
LastEditors: zuweie jojoe.wei@gmail.com
LastEditTime: 2025-06-07 22:04:24
FilePath: /boring-code/src/unit_test/compare_to_numpy.py
Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
'''
import numpy as np

a = np.arange(2*2*3).reshape(2, 2, 3)  # 形状 (2, 3, 4)
print(a)

a_T = a.T  # 转置
print(a_T)  # 输出 (4, 3, 2)