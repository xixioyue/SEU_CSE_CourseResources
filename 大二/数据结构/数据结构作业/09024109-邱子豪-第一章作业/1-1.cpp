#include<iostream>
#include<cmath>

float n_squared(int n)
{
    return n*n;
}

float function_2(int n)
{
    return pow(2,n-2);
}

int main()
{
    for(int i=-5;i<=15;i++)
    {
        if(function_2(i)>n_squared(i))
            std::cout<<"n="<<i<<"时，后者大于前者。\n";
    }
    return 0;
}