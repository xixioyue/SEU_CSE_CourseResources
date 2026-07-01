// Description
// 给定平面上N个点, 请找出这N个点的最近点对.

// Input
// 第一行输入M表示包含M组测试数据，每组先输入N (N<=50000), 接着输入N个坐标(x,y), x和y均为int型整数.

// Output
// 输出最近点对距离,精度保留2位小数

// Sample Input
// 2
// 3 1 1 2 1 3 5 
// 10 851644 996635 20388 842736 262145 615142 890041 434439 787213 89181 99282 310353 179500 803495 728862 687090 225650 604015 765534 465397 

// Sample Output
// 1.00
// 38153.57


// 分治法

/*
算法说明：分治类
①基本思想
先按x坐标排序，把点集分为左右两半，递归求左右内部最近距离d。
跨越中线的最近点对只可能出现在距离中线小于d的条带内，
再按y坐标检查条带中的候选点并更新答案。

②伪代码
ClosestPair(P, l, r):
    if r - l <= 3:
        return 暴力计算最近距离
    mid = (l + r) / 2
    d = min(ClosestPair(P, l, mid), ClosestPair(P, mid, r))
    Q = 按照y从小到大归并Q[l, mid) 和 Q[mid, r)
    for Q中每个点:
        检查后面y差小于d的点并更新d
    return d

③时间复杂度
若每层条带排序，最好和最坏时间复杂度为O(nlog^2n)。
若在递归中维护按y有序，可优化到O(nlogn)。
*/

#include <iostream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <vector>
using namespace std;

struct Point {
    int x, y;
};

double dist(const Point &a,const Point &b){
    double dx = 1.0 * a.x - b.x;
    double dy = 1.0 * a.y - b.y;
    return dx * dx + dy * dy;
}

double naive_solve(Point points[], int l, int r) {
    double min_dist = 1e20;
    for (int i = l; i < r; ++i) {
        for (int j = i + 1; j < r; ++j) {
            min_dist = min(min_dist, dist(points[i], points[j]));
        }
    }
    return min_dist;
}

double closest_pair(Point points[],int l,int r) {
    //小范围暴力
    if(r - l <= 3) return naive_solve(points, l, r);

    //分治
    int mid = (l + r) / 2;
    double d = min(closest_pair(points, l, mid), closest_pair(points, mid, r));
    
    //处理中线附近的点
    int midX = points[mid].x;
    vector<Point> strip;
    for(int i=l; i<r; ++i){
        if(pow((points[i].x - midX), 2) < d){
            strip.push_back(points[i]);
        }
    }
    sort(strip.begin(), strip.end(), [](const Point &a, const Point &b) {
        return a.y < b.y;
    });

    for (size_t i = 0; i < strip.size(); ++i) {
        for (size_t j = i + 1; j < strip.size() && pow((strip[j].y - strip[i].y), 2) < d; ++j) {
            d = min(d, dist(strip[i], strip[j]));
        }
    }
    
    return d;
}

void sort_x(Point points[], int n) {
    sort(points, points + n, [](const Point &a, const Point &b) {
        if (a.x != b.x) 
            return a.x < b.x;
        return a.y < b.y;
    });
}

int main(){
    int m;
    cin >> m;
    while(m--){
        int n;
        cin >> n;
        Point points[50000];
        for (int i = 0; i < n; ++i) {
            cin >> points[i].x >> points[i].y;
        }
        sort_x(points, n);
        double result = sqrt(closest_pair(points, 0, n));
        cout << fixed << setprecision(2) << result << '\n';
    }
}