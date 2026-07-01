/*
归并排序
Time Limit: 1000 MS		Memory Limit: 1000 KB
Description

给定一维int型数组a[0,1,...,n-1], 使用归并排序方法, 对其进行从小到大排序, 请输出递归过程中自顶自下第三层的排序结果, 其中最顶层为第一层, 即最终的排序结果层. 
归并排序划分请按a[0,mid=(0+n-1)/2], a[(0+n-1)/2+1, n-1]进行划分子问题.

Input

输入第1行有一个int型正整数m (m<100), 表示有m行输入.
每行输入的第一个数为int型正整数n (8<n<1000), 后面接着输入n个int型整数.

Output

输出m行, 每行为排好序的输出.*/

/*
算法说明：分治类
①基本思想
归并排序把数组从中间分成左右两半，分别递归排序，再用线性合并得到有序数组。
本题按递归深度控制，只在指定层之后执行归并，从而输出要求层次的排序结果。

②伪代码
MergeSort(A, l, r, depth):
    if l >= r:
        return
    mid = (l + r) / 2
    MergeSort(A, l, mid, depth + 1)
    MergeSort(A, mid + 1, r, depth + 1)
    if depth满足题目要求:
        Merge(A, l, mid, r)

Merge(A, l, mid, r):
    用双指针合并两个有序区间A[l..mid]和A[mid+1..r]

③时间复杂度
完整归并排序每层合并总代价O(n)，共有O(log n)层，
最好、最坏时间复杂度均为O(nlogn)。
*/

#include<iostream>
using namespace std;

void merge(int arr[], int left, int mid, int right)
{
    int i=left,j=mid+1,k=0;
    int temp[right-left+1];
    while(i<=mid && j<=right)
    {
        if(arr[i]<=arr[j])
            temp[k++]=arr[i++];
        else
            temp[k++]=arr[j++];
    }
    while(i<=mid)
        temp[k++]=arr[i++];
    while(j<=right)
        temp[k++]=arr[j++];
    for(int i=0;i<k;i++)
        arr[left+i]=temp[i];
}

void mergesort(int arr[], int left, int right,int depth)
{
    if(left>=right)
        return;
    int mid=(left+right)/2;
    if(depth>1)
    { 
        mergesort(arr,left,mid,depth+1);
        mergesort(arr,mid+1,right,depth+1);
        merge(arr,left,mid,right);
    }
    else
    {
        mergesort(arr,left,mid,depth+1);
        mergesort(arr,mid+1,right,depth+1);
    }
}

int main()
{
    int m,n=0;
    cin>>m;
    for(int i=0;i<m;i++)
    {
        cin>>n;
        int arr[n];
        for(int j=0;j<n;j++)
            cin>>arr[j];
        mergesort(arr,0,n-1,0);
        for(int j=0;j<n;j++)
            cout<<arr[j]<<" ";
        cout<<endl;
    }
}