// Description
// 给定平面上N个点, 请找出这N个点的凸包.

// Input
// 第一行输入M表示包含M组测试数据，每组先输入N (N<=100), 接着输入N个坐标(x,y), x和y均为int型整数.

// Output
// 以最下最左点开始逆时针输出凸包, 若有多个点在同一坐标,只输出一个,若凸包上有多个点在同一线上,只输出两端点.

// Sample Input
// 2
// 7 1 1 4 1 4 4 4 4 1 4 2 2 5 5
// 8 5 6 8 3 1 8 5 7 3 5 3 5 1 8 2 11

// Sample Output
// case 1:
// 1 1
// 4 1
// 5 5
// 1 4
// case 2:
// 8 3
// 2 11
// 1 8
// 3 5

/*
算法说明：基础算法类
①算法思想
使用Andrew单调链算法。先按横坐标、纵坐标排序并去重，
再分别维护下凸壳和上凸壳。每加入一个新点时，用叉积判断是否出现非逆时针转向，
若出现则弹出栈顶点，保证凸包边界只保留端点。

②伪代码
ConvexHull(P):
    将点按x、y排序并去重
    lower = 空栈
    for p in P:
        while lower中至少两个点且加入p后不构成逆时针转向:
            弹出lower栈顶
        p入lower
    upper同理从右向左扫描
    合并lower和upper得到凸包

③时间复杂度
排序占主要代价，最好、最坏时间复杂度均为O(nlogn)。
构造上下凸壳时每个点最多入栈、出栈一次，线性复杂度O(n)。
*/

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct Point {
    int x;
    int y;
};

bool operator<(const Point &a, const Point &b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

bool operator==(const Point &a, const Point &b) {
    return a.x == b.x && a.y == b.y;
}

long long cross(const Point &a, const Point &b, const Point &c) {
    return 1LL * (b.x - a.x) * (c.y - a.y) - 1LL * (b.y - a.y) * (c.x - a.x);
}

vector<Point> convex_hull(vector<Point> p) {
    sort(p.begin(), p.end());
    p.erase(unique(p.begin(), p.end()), p.end());
    if (p.size() <= 1) return p;

    vector<Point> lower, upper;
    for (const Point &pt : p) {
        while (lower.size() >= 2 && cross(lower[lower.size() - 2], lower.back(), pt) <= 0) {
            lower.pop_back();
        }
        lower.push_back(pt);
    }
    for (int i = (int)p.size() - 1; i >= 0; --i) {
        const Point &pt = p[i];
        while (upper.size() >= 2 && cross(upper[upper.size() - 2], upper.back(), pt) <= 0) {
            upper.pop_back();
        }
        upper.push_back(pt);
    }

    lower.pop_back();
    upper.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    for (int tc = 1; tc <= T; ++tc) {
        int n;
        cin >> n;
        vector<Point> p(n);
        for (Point &pt : p) cin >> pt.x >> pt.y;

        vector<Point> hull = convex_hull(p);
        if (!hull.empty()) {
            int start = 0;
            for (int i = 1; i < (int)hull.size(); ++i) {
                if (hull[i].y < hull[start].y ||
                    (hull[i].y == hull[start].y && hull[i].x < hull[start].x)) {
                    start = i;
                }
            }
            rotate(hull.begin(), hull.begin() + start, hull.end());
        }

        cout << "case " << tc << ":\n";
        for (const Point &pt : hull) {
            cout << pt.x << ' ' << pt.y << '\n';
        }
    }
    return 0;
}