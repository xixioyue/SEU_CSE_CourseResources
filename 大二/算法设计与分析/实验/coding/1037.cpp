// Description
// 火爆（曾经？）的相亲界扛把子节目非诚勿扰第一期开始了！第一期节目一共邀请了n位男嘉宾和m位女嘉宾，每位男嘉宾都有几个自己中意的女嘉宾。为了节目收视率，导演组决定要让尽可能多的男嘉宾牵手成功！幸运的是这一期女嘉宾都是演员，她们答应只要男嘉宾愿意，她们一定会配合演出答应牵手。现在导演组拿到了每位男嘉宾中意的女嘉宾名单，请问导演组最多可以让多少对荧幕情侣牵手成功。

// Input
// 第一行输入T(T<=10)表示有T组数据。每组数据先输入两个正整数 n,m (n,m<=50)，接下来n行每行先输入一个k，代表第i号男嘉宾中意的女嘉宾人数(k<=10)，随后输入k个数用空格分开，代表这名男嘉宾中意的女嘉宾编号名单。

// Output
// 输出T行正整数，第i行表示第i组数据下的最多能有多少对男女嘉宾牵手成功。(输出保证不溢出int)

// Sample Input
// 1
// 3 3
// 1 1
// 2 1 2
// 1 2

// Sample Output
// 2

/*
算法说明：图算法
①算法思想
这是二分图最大匹配问题。依次尝试为每个男嘉宾寻找可匹配的女嘉宾。
若女嘉宾未匹配则直接匹配；若已匹配，则递归尝试让她当前匹配的男嘉宾改选其他女嘉宾，
如果能找到增广路径，就扩大匹配数。

②伪代码
MaxMatching():
    ans = 0
    for 每个男嘉宾u:
        清空本轮访问标记seen
        if Augment(u):
            ans = ans + 1
    return ans

Augment(u):
    for u喜欢的每个女嘉宾v:
        if v本轮访问过:
            continue
        标记v
        if v未匹配或Augment(v当前匹配的男嘉宾):
            match[v] = u
            return true
    return false

③时间复杂度
设左部点数为n，边数为E。
最好情况下每次很快找到空闲匹配，接近O(E)。
使用DFS增广的最坏时间复杂度为O(nE)。
*/

#include <iostream>
#include <vector>
using namespace std;

bool augment(int u, const vector<vector<int>>& like, vector<int>& match, vector<int>& seen) { // 增广路径算法
    for (int v : like[u]) {
        if (seen[v]) continue;
        seen[v] = 1;
        // match[v] == 0 --> v没有匹配的男嘉宾，直接匹配u
        // augment --> 要求match[v]的男嘉宾尝试寻找新的匹配对象
        if (match[v] == 0 || augment(match[v], like, match, seen)) {
            match[v] = u;
            return true;
        }
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n, m;
        cin >> n >> m;
        
        vector<vector<int>> like(n + 1);
        for (int i = 1; i <= n; ++i) { // 记录每位男嘉宾中意的女嘉宾名单
            int k;
            cin >> k;
            like[i].resize(k);
            for (int& x : like[i]) cin >> x;
        }

        vector<int> match(m + 1, 0); // 记录每个女嘉宾匹配的男嘉宾
        int ans = 0;
        for (int i = 1; i <= n; ++i) {
            vector<int> seen(m + 1, 0); // 记录每个女嘉宾是否在当前增广路径中被访问过
            if (augment(i, like, match, seen)) ++ans;
        }
        cout << ans << '\n';
    }
    return 0;
}
