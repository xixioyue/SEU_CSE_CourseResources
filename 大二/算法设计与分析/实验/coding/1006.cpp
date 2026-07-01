/*
堆排序
Time Limit: 1000 MS		Memory Limit: 1000 KB
Description

给定一维int型数组, 请构造一棵最小堆. 总是优先向左子树调整.

Input

输入第1行有一个int型正整数m (m<100), 表示有m行输入.
每行输入的第一个数为int型正整数n (8<n<1000), 后面接着输入n个int型整数.

Output

输出m行, 每行为最小堆.

Sample Input

2
7 3 8 4 1 6 3 2
8 2 4 5 9 8 7 6 3

Sample Output

1 3 2 8 6 3 4 
2 3 5 4 8 7 6 9*/
//最小堆heapsort

/*
算法说明：基础算法类
①算法思想
把数组看成完全二叉树，从最后一个非叶子结点开始向前调整。
每次将当前结点与较小的孩子比较，若孩子更小则交换并继续向下调整，
最终使每个父结点都不大于其孩子，得到最小堆。

②伪代码
BuildMinHeap(A, n):
    for i = 最后一个非叶子结点 downto 0:
        SiftDown(A, i, n)

SiftDown(A, root, n):
    选出root左右孩子中较小的孩子child
    if A[child] < A[root]:
        swap(A[child], A[root])
        SiftDown(A, child, n)

③时间复杂度
自底向上建堆的最好、最坏时间复杂度均为O(n)。
若逐个插入建堆，则最坏为O(nlogn)，但本题使用的是线性建堆。
*/

#include <iostream>
using namespace std;

int n=0;
int arr[1000];

void sortdown(int arr[],int root,int length)
{
    if(root>=length)
        return;
    if(2*root+1<length)
    {
        if(2*root+2<length && arr[2*root+2]<arr[2*root+1]&&arr[2*root+2]<arr[root]){
            swap(arr[2*root+2],arr[root]);
            sortdown(arr,2*root+2,length);
        }
        else if(arr[2*root+1]<arr[root]){
            swap(arr[2*root+1],arr[root]);
            sortdown(arr,2*root+1,length);
        }
    }
}

void heapinit(int arr[],int length)
{
    for(int i=(length-1)/2;i>=0;i--)
    {
        sortdown(arr,i,length);
    }    
}

int main()  
{
    int m;
    cin>>m;
    for(int i=0;i<m;i++)
    {
        cin>>n;
        for(int j=0;j<n;j++)
            cin>>arr[j];
        heapinit(arr,n);
        for(int j=0;j<n;j++)
            cout<<arr[j]<<" ";
        cout<<endl;
    }
}