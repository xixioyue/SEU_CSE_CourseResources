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
    int i=left,j=right;
    while(i<j)
    {
        while(i<j && arr[j]>=pivot)
            j--;
        arr[i]=arr[j];
        while(i<j && arr[i]<=pivot)
            i++;
        arr[j]=arr[i];
    }
    arr[i]=pivot;
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