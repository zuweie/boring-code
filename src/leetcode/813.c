/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2022-11-30 09:24:49
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2022-12-04 10:00:51
 * @FilePath: /boring-code/src/leetcode/813.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
//#include <stdio.h>
static double avg(int* nums, int numsSize) 
{
    double sum = 0.f;
    for (int i=0; i<numsSize; ++i) {
        sum += nums[i];
    }
    return sum / (double) numsSize;
}

static double fn(int* nums, int numsSize, int k, double (*_buffer)[k+1])
{

    if (_buffer[numsSize][k] > 0.f)
        return _buffer[numsSize][k];

    if (k == 1) 
        return avg(nums, numsSize);

    int n = numsSize;
    //double avgs[n-k+1];

    double res = -1.f;

    for (int i=1; i<=n-k+1; ++i) {
       double a = avg(nums, i) +  fn(&nums[i], n-i, k-1, _buffer);
       if (res < a) res = a;
    }
    

    _buffer[numsSize][k] = res;

    return res;
}

double largestSumOfAverages(int* nums, int numsSize, int k)
{
    double _buffer[numsSize+1][k+1];
    for (int i=0; i<numsSize+1; ++i) {
        for (int j=0; j<k+1; j++) {
            _buffer[i][j] = -1.f;
        }
    }

    return fn(nums, numsSize, k, _buffer);
}
