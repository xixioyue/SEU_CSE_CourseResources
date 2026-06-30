// 1007 最大K乘积
/*Description

设有一个长度为N的数字串，要求选手使用K个乘号将它分成K+1个部分，找出一种分法，使得这K+1个部分的乘积能够为最大。
同时，为了帮助选手能够正确理解题意，主持人还举了如下的一个例子：
有一个数字串：312， 当N=3，K=1时会有以下两种分法：

1)  3*12=36
2)  31*2=62
   
这时，符合题目要求的结果是：31*2=62
现在，请你帮助你的好朋友XZ设计一个程序，求得正确的答案。

Input

第一行输入M表示包含M组测试数据，每组输入有一行包含两个自然数N，K（6≤N≤40，1≤K≤6）,以及一个长度为N的数字串。

Output

对于每组输入数据，输出所求得的最大乘积（一个自然数）,每组一行。

Sample Input

2
6 1 101010
9 4 321044105

Sample Output

10100
5166000*/

/*
算法说明：动态规划类
①dp含义
value[i][j]表示数字串从i到j形成的整数。
dp[used][end]表示在前end+1个数字中使用used个乘号时可以得到的最大乘积。

②dp转移方程
枚举最后一个乘号的位置cut：
dp[used][end] = max(dp[used - 1][cut] * value[cut + 1][end])
其中 used - 1 <= cut < end。

③dp初始值
不使用乘号时：
dp[0][end] = value[0][end]。
其余状态初始为0或无效值，逐层由更少乘号的状态转移得到。

④算法时间复杂度
预处理value为O(n^2)，状态转移为O(k*n^2)。若计入大整数乘法，单次转移还需乘法位数相关的额外代价。
*/

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct BigInt {
    static const int BASE = 1000000000;
    vector<int> d;

    //BigInt默认构造函数
    //以BASE为基数存储数字，每个元素存储9位，低位在前，高位在后
    //E.g. d=[234567890, 1] ~表示 1 234567890
    BigInt(long long x = 0) {
        if (x == 0) return;
        while (x > 0) {
            d.push_back(x % BASE);
            x /= BASE;
        }
    }
    
    //比较BigInt大小
    bool operator<(const BigInt &other) const {
        if (d.size() != other.d.size()) return d.size() < other.d.size();
        for (int i = static_cast<int>(d.size()) - 1; i >= 0; --i) {
            if (d[i] != other.d[i]) return d[i] < other.d[i];
        }
        return false;
    }

    //乘以一个整数
    void mul_int(int x) {
        long long carry = 0;
        for (int &v : d) {
            long long cur = 1LL * v * x + carry;
            v = cur % BASE;
            carry = cur / BASE;
        }
        if (carry) d.push_back(static_cast<int>(carry));
    }

    void add_int(int x) {
        if (d.empty()) d.push_back(0);
        long long carry = x;
        for (int i = 0; i < static_cast<int>(d.size()) && carry; ++i) {
            long long cur = 1LL * d[i] + carry;
            d[i] = cur % BASE;
            carry = cur / BASE;
        }
        if (carry) d.push_back(static_cast<int>(carry));
    }

    string str() const {
        if (d.empty()) return "0";
        stringstream ss;
        ss << d.back();
        for (int i = static_cast<int>(d.size()) - 2; i >= 0; --i) {
            ss << setw(9) << setfill('0') << d[i];
        }
        return ss.str();
    }
};

BigInt operator*(const BigInt &a, const BigInt &b) {
    if (a.d.empty() || b.d.empty()) return BigInt(0);
    BigInt res;
    vector<long long> tmp(a.d.size() + b.d.size());
    for (int i = 0; i < static_cast<int>(a.d.size()); ++i) {
        for (int j = 0; j < static_cast<int>(b.d.size()); ++j) {
            tmp[i + j] += 1LL * a.d[i] * b.d[j];
        }
    }

    long long carry = 0;
    for (long long cur : tmp) {
        cur += carry;
        res.d.push_back(static_cast<int>(cur % BigInt::BASE));
        carry = cur / BigInt::BASE;
    }
    while (carry) {
        res.d.push_back(static_cast<int>(carry % BigInt::BASE));
        carry /= BigInt::BASE;
    }
    while (!res.d.empty() && res.d.back() == 0) res.d.pop_back();
    return res;
}

ostream &operator<<(ostream &out, const BigInt &x) {
    return out << x.str();
}

BigInt solve_case(int n, int k, const string &s) {
    vector<vector<BigInt>> value(n, vector<BigInt>(n));
    for (int i = 0; i < n; ++i) {
        BigInt num = 0;
        for (int j = i; j < n; ++j) {
            num.mul_int(10);
            num.add_int(s[j] - '0');
            value[i][j] = num;
        }
    }

    vector<vector<BigInt>> dp(k + 1, vector<BigInt>(n));
    for (int end = 0; end < n; ++end) {
        dp[0][end] = value[0][end];
    }

    for (int used = 1; used <= k; ++used) {
        for (int end = used; end < n; ++end) {
            BigInt best = 0;
            for (int cut = used - 1; cut < end; ++cut) {
                BigInt candidate = dp[used - 1][cut] * value[cut + 1][end];
                if (best < candidate) best = candidate;
            }
            dp[used][end] = best;
        }
    }

    return dp[k][n - 1];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n, k;
        string s;
        cin >> n >> k >> s;
        cout << solve_case(n, k, s) << '\n';
    }

    return 0;
}