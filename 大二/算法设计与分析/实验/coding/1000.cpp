#include<iostream>
int main()
{
    int n,a,b=0;
    std::cin>>n;
    for(int i=0;i<n;i++)
    {
        std::cin>>a>>b;
        std::cout<<a+b<<'\n';
    }
    return 0;
}