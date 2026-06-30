/*
快速排序
Time Limit: 1000 MS		Memory Limit: 1000 KB
Description

给定一维int型数组a[0,1,...,n-1], 使用快速排序方法, 对其进行从小到大排序, 请输出递归过程中自顶自下第二层的划分结果, 其中最顶层为第一层, 即最终的排序结果层. 
划分时请用第1个元素作为划分基准, 并使用课件上的方法进行一次扫描实现划分.

Input

输入第1行有一个int型正整数m (m<100), 表示有m行输入.
每行输入的第一个数为int型正整数n (8<n<1000), 后面接着输入n个int型整数.

Output

对每组数据, 输出自顶自下第二层的划分结果.
*/

/*
算法说明：分治类
①基本思想
快速排序选择当前区间第一个元素作为基准，通过一次扫描把小于基准的元素放到左侧，
大于等于基准的元素放到右侧，再递归处理左右两个子区间。

②伪代码
QuickSort(A, l, r, depth):
    if l >= r:
        return
    pivot = A[l]
    i = l
    for j = l + 1 to r:
        if A[j] < pivot:
            i = i + 1
            swap(A[i], A[j])
    swap(A[l], A[i])
    记录题目要求层数的划分结果
    QuickSort(A, l, i - 1, depth + 1)
    QuickSort(A, i + 1, r, depth + 1)

③时间复杂度
划分均衡时最好时间复杂度为O(nlogn)。
若每次基准都是最小或最大元素，最坏时间复杂度为O(n^2)。
*/

#include<iostream>
using namespace std;

int n=0;
int arr[1000];
int res[1000];

void quicksort(int arr[], int left, int right,int depth)
{
    if(left>=right)
        return;
    int pivot=arr[left];
    int i=left;
    for(int j=left+1;j<=right;j++)
    {
        if(arr[j]<pivot)
        {
            i++;
            swap(arr[i],arr[j]);
        }
    }    
    swap(arr[left],arr[i]);
    if(depth==1)
    {
        res[i]=arr[i];
    }
    if(depth==2) 
    {
        for(int k=left;k<=right;k++)
            res[k]=arr[k];
    }
    quicksort(arr,left,i-1,depth+1);
    quicksort(arr,i+1,right,depth+1);
}

int main()
{
    int m;
    cin>>m;
    for(int i=0;i<m;i++)
    {
        cin>>n;
        for(int j=0;j<n;j++)
        {
            cin>>arr[j];
            res[j]=0;
        }
        quicksort(arr,0,n-1,1);
        for(int j=0;j<n;j++)
            cout<<res[j]<<" ";
        cout<<endl;
    }
}