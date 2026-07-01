//冒泡排序

/*
算法说明：基础算法类
①算法思想
冒泡排序通过不断比较相邻元素并交换逆序元素，使较大的元素逐步向数组后端移动。
本题代码等价于做一趟扫描，将当前最大值保留到最后输出，其余元素按扫描过程输出。

②伪代码
ScanOnce(A, n):
    max = A[0]
    for i = 1 to n - 1:
        if A[i] > max:
            输出max
            max = A[i]
        else:
            输出A[i]
    输出max

③时间复杂度
若实现完整冒泡排序并加入提前结束标记，最好为O(n)，最坏为O(n^2)。
本题当前代码只进行一趟扫描，最好、最坏时间复杂度均为O(n)。
*/

#include<iostream>

int main()
{
    int m,n=0;
    std::cin>>m;
    for(int i=0;i<m;i++)
    {
        std::cin>>n;
        std::cin.ignore();
        int max=0;
        for(int j=0;j<n;j++)
        {
            int temp;
            std::cin>>temp;
            if(j==0)
            {
                max=temp;
                continue;
            }
            if(temp>max)
            {
                std::cout<<max<<" ";
                max=temp;
            }
            else
                std::cout<<temp<<" ";
        }
        std::cout<<max<<std::endl;
    }
    return 0;
}