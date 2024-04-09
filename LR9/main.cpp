#include <bits/stdc++.h>
using namespace std;
using graph = vector<vector<int64_t>>;

const int64_t INF = 1e18;

bool bfs(graph &g, int s, int t, vector<int> &parent) {
    int n = g.size();
    vector<bool> visited(n, false);
    queue<int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; v++) {
            if (!visited[v] && g[u][v] > 0) {
                if (v == t) {
                    parent[v] = u;
                    return true;
                }
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    return false;
}

int64_t edmondsKarp(graph &g, int s, int t) {
    int n = g.size();
    graph rg = g; // Residual graph
    vector<int> parent(n);
    int64_t max_flow = 0;

    while (bfs(rg, s, t, parent)) {
        int64_t path_flow = INF;
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            path_flow = min(path_flow, rg[u][v]);
        }

        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            rg[u][v] -= path_flow;
            rg[v][u] += path_flow;
        }

        max_flow += path_flow;
    }
    return max_flow;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    graph g(n, vector<int64_t>(n));
    for (int i = 0; i < m; i++) {
        int u, v;
        int64_t c;
        cin >> u >> v >> c;
        --u; --v;
        g[u][v] += c;
    }
    int64_t flow = edmondsKarp(g, 0, n - 1);
    cout << flow << endl;
    return 0;
}
