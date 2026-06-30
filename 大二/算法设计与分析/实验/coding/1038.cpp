/* Description
继非诚勿扰第一期节目顺利结束后，非诚勿扰第二期开始了！
这一期节目同样一共邀请了n位男嘉宾和m位女嘉宾，每位男嘉宾都有几个自己中意的女嘉宾。
更加幸运的是，这期的女嘉宾同样个个都是好演员，只要男嘉宾有想法她们愿意配合演出牵手成功！
不过今时不同往日，女嘉宾答应演出是需要收费的。每位女嘉宾对于答应配合牵手不同的男嘉宾需要的费用视她的心情而定。
现在导演组拿到了每位女嘉宾对于配合演出与每位男嘉宾牵手需要的费用，以及每位男嘉宾中意的女嘉宾名单，请问导演组在促成尽量多的荧幕情侣的同时，需要付给女嘉宾的最少预算是多少。 */

// Input
// 第一行输入T(T<=10)表示有T组数据。 每组数据先输入两个正整数 n,m (n,m<=50)，紧接着输入m行每行n个数a[i][j]，用空格隔开，代表i号女嘉宾如果和j号男嘉宾牵手需要收取的费用。  接下来输入n行每行先输入一个k，代表第i号男嘉宾中意的女嘉宾人数(k<=10)，随后输入k个数用空格分开，代表这名男嘉宾中意的女嘉宾编号名单。

// Output
// 输出T行每行两个正整数，第i行表示第i组数据下的最多能有多少对男女嘉宾牵手成功以及节目组需要支付给女嘉宾的费用。(输出保证不溢出int)

// Sample Input
// 1
// 2 2
// 3 5
// 4 2
// 1 1
// 2 1 2

// Sample Output
// 2 5

// 最小费用最大流问题

/*
算法说明：网络流
①算法思想
建立源点到男嘉宾、男嘉宾到中意女嘉宾、女嘉宾到汇点的网络。
每条匹配边容量为1，费用为牵手费用。反复用SPFA在残量网络中寻找费用最小的增广路，
每次增广1单位流量，直到不存在增广路，得到最大匹配数及其最小费用。

②伪代码
MinCostMaxFlow():
    建立源点、男嘉宾、女嘉宾、汇点网络
    flow = 0, cost = 0
    while 残量网络中存在从源点到汇点的最小费用路径:
        沿该路径增广流量
        更新正反向边容量
        累加费用
    return flow, cost

③时间复杂度
设点数为V，边数为E，最大流量为F。
若增广路很少且很快找到，实际运行较快；按SPFA估计，
每次最短路最坏O(VE)，总最坏时间复杂度约为O(FVE)。
*/

#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>
#include <algorithm>
using namespace std;

struct Edge {
    int to;
    int rev; // 反向边在对方邻接表中的索引
    int cap; // 剩余容量
    long long cost;
};

void addEdge(vector<vector<Edge>>& g, int u, int v, int cap, int cost) {
    Edge a{v, (int)g[v].size(), cap, cost};
    Edge b{u, (int)g[u].size(), 0, -cost};
    g[u].push_back(a);
    g[v].push_back(b);
}

int main(){
    int T;
    cin >> T;
    while(T--){
        int n, m;
        cin >> n >> m;
        vector<vector<int>> cost(m + 1, vector<int>(n + 1)); // 女嘉宾woman与男嘉宾man的牵手的费用 [1][1] ~ [m][n]
        for(int woman = 1; woman <= m; woman++){
            for(int man = 1; man <= n; man++){
                cin >> cost[woman][man];
            }
        }

        // 构建最小费用最大流图
        // 编号：source = 0; sink = n + m + 1; 男嘉宾编号1 ~ n，女嘉宾编号n+1 ~ n+m
        int source = 0;
        int sink = n + m + 1;
        vector<vector<Edge>> g(n + m + 2); // 邻接表表示的图
        // source -> man
        for(int man = 1; man <= n; man++) 
            addEdge(g, source, man, 1, 0); 
        // woman -> sink
        for(int woman = 1; woman <=  m; woman++) 
            addEdge(g, woman + n, sink, 1, 0); 
        // man -> woman
        for(int man = 1; man <= n; man++){ 
            int numOfLuv;
            cin >> numOfLuv;
            while(numOfLuv--){
                int woman;
                cin >> woman;
                addEdge(g, man, n + woman, 1, cost[woman][man]);
            }
        }
        // 建图完成

        // 使用最小费用最大流算法求解 SPFA
        int flow = 0; // 流量
        long long TC = 0; // 总费用
        const long long INF = numeric_limits<long long>::max() >> 2;
        while(1){
            // 初始化dist prev inq pe
            vector<long long> dist(sink + 1, INF);
            vector<int> prev(sink + 1, -1); // prev[v]表示v的前驱节点
            vector<bool> inq(sink + 1, false);  // v是否在队列中
            vector<int> pe(sink + 1, -1); // prev[v]到v是第几条边
            
            // SPFA找source到sink的最小费用路径
            dist[source] = 0;
            queue<int> q;
            q.push(source);
            inq[source] = true;

            while(!q.empty()){
                int u = q.front();
                q.pop();
                inq[u] = false;

                for(int i = 0; i < (int)g[u].size(); i++){
                    Edge& e = g[u][i];
                    if(e.cap > 0 && dist[u] + e.cost < dist[e.to]){
                        dist[e.to] = dist[u] + e.cost;
                        prev[e.to] = u;
                        pe[e.to] = i;
                        if(!inq[e.to]){
                            q.push(e.to);
                            inq[e.to] = true;
                        }
                    }
                }
            }

            // 检查是否找到增广路径
            if(dist[sink] == INF) break;

            // 找到增广路径，更新流量和费用
            int u = sink;
            int minCap = 1;
            while(u != source){
                int v = prev[u];
                int edgeIdx = pe[u];
                minCap = min(minCap, g[v][edgeIdx].cap);
                u = v;
            }

            u = sink;
            while(u != source){
                int v = prev[u];
                int edgeIndex = pe[u];
                g[v][edgeIndex].cap -= minCap;
                g[u][g[v][edgeIndex].rev].cap += minCap;
                TC += (long long)minCap * g[v][edgeIndex].cost;
                u = v;
            }
            flow += minCap;
        }
        cout << flow << " " << TC << endl;

    }
}
