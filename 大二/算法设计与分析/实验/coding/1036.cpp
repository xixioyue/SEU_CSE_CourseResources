// Description - KMP算法实现
// 给定文本串s与模式串t，求s中有多少个子串与t相同，两个子串视为不同仅当他们长度不等或起始位置不同。

// Input
// 第一行输入T(T<=100)表示有T组数据。每组数据先输入两个正整数n、m(1<=n<=100000,1<=m<=n)，分别表示文本串与模式串长度。紧接着输入两行字符串，即为s、t。

// Output
// 输出T行正整数，第i行表示第i组文本串中有多少个子串与模式串相同。

// Sample Input
// 2
// 5 3
// ababa
// aba
// 3 1
// aaa
// a

// Sample Output
// 2
// 3

/*
算法说明：基础算法类
①算法思想
KMP算法先为模式串构造next数组，next[i]表示模式串前缀与后缀的最长相等长度。
匹配文本串时若发生失配，不回退文本串指针，而是根据next数组移动模式串指针，
从而避免重复比较。

②伪代码
BuildNext(pattern):
    next[0] = 0
    for i = 1 to m - 1:
        while j > 0且pattern[i] != pattern[j]:
            j = next[j - 1]
        if pattern[i] == pattern[j]:
            j = j + 1
        next[i] = j

KMP(text, pattern):
    count = 0
    for text中每个字符:
        失配时按next回退pattern指针
        匹配时pattern指针前进
        if pattern完整匹配:
            count = count + 1
            按next继续匹配
    return count

③时间复杂度
构造next数组为O(m)，匹配为O(n)。
最好情况下也需要线性扫描输入，时间复杂度为O(n+m)；
最坏情况下借助next数组仍为O(n+m)。
*/

#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<int> bldNext(const string& ptrn) {
    int m = (int)ptrn.size();
    vector<int> next(m,0);
    // i指向next数组的当前位置，j指向模式串的当前位置
    for (int i = 1, j = 0; i < m; ++i) {
        // ptrn[i] == ptrn[j] --> next[i] = j + 1 , j++
        // ptrn[i] != ptrn[j] --> if j!= 0, then j = next[j - 1] , test the new j
        while (j > 0 && ptrn[i]!= ptrn[j])
            j = next[j - 1];
        if (ptrn[i] == ptrn[j]) 
            ++j;
        next[i] = j;
    }
    return next;
}

int kmp(const string& txt, const string& ptrn, const vector<int>& nxt) { // return模式串在文本串中出现的次数
    // i指向文本串的当前位置，j指向模式串的当前位置
    // txt[i] == ptrn[j] --> i++, j++
    // txt[i] != ptrn[j] --> if j!= 0, then j = next[j - 1] , test the new j
    int n = (int)txt.size();
    int m = (int)ptrn.size();
    int count = 0;
    for (int i = 0, j = 0; i < n; ++i){
        while (j > 0 && txt[i]!= ptrn[j])
            j = nxt[j - 1];
        if (txt[i] == ptrn[j]) 
            ++j;
        if (j == m) { // 匹配成功
            ++count;
            j = nxt[j - 1];
        }
    }
    return count;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n, m; // n: 文本串长度, m: 模式串长度
        string s, t; // s: 文本串, t: 模式串
        cin >> n >> m >> s >> t;

        vector<int> next = bldNext(t);
        cout << kmp(s, t, next) << "\n";
    }
    return 0;
}