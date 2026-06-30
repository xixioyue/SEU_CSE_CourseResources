// Description
// 给定N(N<=50000)栋建筑物左右边界坐标及高度，输出这N栋建筑物天际轮廓的形状变化. 

// Input
// 第一行输入N表示有N栋建筑物，接下来N行每行输入三个int型整数a、b、h，分别表示每栋建筑物的左右边界坐标和高度.

// Output
// 输出若干行，每行输出两个数x、h，表示天际轮廓在坐标x处高度发生变化，变化为h.

// Sample Input
// 8
// 1 5 11
// 2 7 6
// 3 9 13
// 12 16 7
// 14 25 3
// 19 22 18
// 23 29 13
// 24 28 4

// Sample Output
// 1 11
// 3 13
// 9 0
// 12 7
// 16 3
// 19 18
// 22 3
// 23 13
// 29 0

// 分治算法的时间复杂度为O(nlogn)

/*
算法说明：分治类
①基本思想
单栋建筑的天际线由两个关键点表示：左端升到高度h，右端降到0。
多栋建筑的天际线可以把建筑集合分成左右两半，分别求出天际线，
再像归并一样合并两个轮廓，当前位置高度取左右轮廓高度的较大值。

②伪代码
// 求 buildings[l...r-1] 的天际线，区间采用左闭右开形式
Skyline(buildings, l, r):
    if r - l = 1:
        b = buildings[l]
        return [(b.left, b.height), (b.right, 0)]

    mid = floor((l + r) / 2)
    leftSkyline  = Skyline(buildings, l, mid)
    rightSkyline = Skyline(buildings, mid, r)
    return Merge(leftSkyline, rightSkyline)

// 将关键点(x, height)加入结果，并删除没有产生高度变化的冗余点
AppendPoint(result, x, height):
    if result非空 and result最后一个点的横坐标 = x:
        result最后一个点的高度 = height
        if result至少有两个点 and 最后两个点的高度相同:
            删除result最后一个点
        return
    if result为空 or result最后一个点的高度 != height:
        result末尾加入(x, height)

// 像归并排序一样，按横坐标合并两条天际线
Merge(A, B):
    result = 空序列
    i = 0, j = 0
    heightA = 0, heightB = 0

    while i < A.length and j < B.length:
        if A[i].x < B[j].x:
            x = A[i].x
            heightA = A[i].height
            i = i + 1
        else if A[i].x > B[j].x:
            x = B[j].x
            heightB = B[j].height
            j = j + 1
        else:
            x = A[i].x
            heightA = A[i].height
            heightB = B[j].height
            i = i + 1
            j = j + 1
        AppendPoint(result, x, max(heightA, heightB))

    while i < A.length:
        heightA = A[i].height
        AppendPoint(result, A[i].x, max(heightA, heightB))
        i = i + 1

    while j < B.length:
        heightB = B[j].height
        AppendPoint(result, B[j].x, max(heightA, heightB))
        j = j + 1

    return result

③时间复杂度
每层合并总代价O(n)，递归层数O(logn)，
最好、最坏时间复杂度均为O(nlogn)。
*/

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct bld{//building
    int l, r, h;
};

struct pnt{//point
    int x, h;
};

void addPnt(vector<pnt> &res, int x, int h){//用于添加天际轮廓的变化点，添加过程中会去除冗余点
    if(!res.empty() && res.back().h == h) //如果高度没有变化则不添加
        return;
    if(!res.empty() && res.back().x == x) //如果坐标相同则更新高度
        res.back().h = h;
    else    //否则添加新点
        res.push_back({x, h});
}

vector<pnt> mergeSkyl(const vector<pnt> &a, const vector<pnt> &b){//合并两个天际轮廓
    vector<pnt> res;
    int i = 0, j = 0;
    int h1 = 0, h2 = 0;
    
    while (i < a.size() && j < b.size()){
        if(a[i].x < b[j].x){
            h1 = a[i].h;
            addPnt(res, a[i].x, max(h1, h2));
            ++i;
        }else if(a[i].x > b[j].x){
            h2 = b[j].h;
            addPnt(res, b[j].x, max(h1, h2));
            ++j;
        }else{
            h1 = a[i].h;
            h2 = b[j].h;
            addPnt(res, a[i].x, max(h1, h2));
            ++i;
            ++j;
        }
    }

    while(i < a.size()){
        h1 = a[i].h;
        addPnt(res, a[i].x, max(h1, h2));
        ++i;
    }

    while(j < b.size()){
        h2 = b[j].h;
        addPnt(res, b[j].x, max(h1, h2));
        ++j;
    }

    return res;
}

vector<pnt> solve(vector<bld> &blds, int l, int r){//[l, r)
    //递归出口 只剩一个建筑物
    if(l + 1 >= r)
        return {{blds[l].l, blds[l].h}, {blds[l].r, 0}};

    //分治
    int mid = (l + r) / 2;
    vector<pnt> left = solve(blds, l, mid);
    vector<pnt> right = solve(blds, mid, r);
    
    return mergeSkyl(left, right);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<bld> blds(n);
    for (int i = 0; i < n; ++i) {
        cin >> blds[i].l >> blds[i].r >> blds[i].h;
    }
    vector<pnt> res = solve(blds, 0, n);
    for (const auto &p : res) {
        cout << p.x << " " << p.h << "\n";
    }
    return 0;
}
