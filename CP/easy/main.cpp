#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <cmath>
#include <cfloat>
#include <limits.h>
 
 
const int INT_INF = INT_MAX / 3;
const double DBL_INF = DBL_MAX / 3;
 
struct Point {
    long long x, y;
};
 
struct Node {
    int vertex;
    double f;
    bool operator>(const Node& o) const {
        return this->f > o.f;
    }
};
 
double euclideanDistance(const Point& a, const Point& b) {
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}
 
double aStar(int start, int goal, const std::vector<Point>& points, const std::vector<std::vector<int>>& graph, std::vector<double>& costSoFar, std::vector<bool> closed) {
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
    openSet.push({start, euclideanDistance(points[start], points[goal])});
 
    double newCost, priority;
 
    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();
 
        closed[current.vertex] = true;
 
        if (current.vertex == goal) {
            return costSoFar[current.vertex];
        }
 
        for (int next : graph[current.vertex]) {
            if(closed[next])
                continue;
            newCost = costSoFar[current.vertex] + euclideanDistance(points[current.vertex], points[next]);
            if (newCost < costSoFar[next]) {
                costSoFar[next] = newCost;
                priority = newCost + euclideanDistance(points[next], points[goal]);
                openSet.push({next, priority});
            }
        }
    }
 
    return -1;
}
 
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0), std::cout.tie(0);
    std::cout << std::fixed << std::setprecision(6);
 
    int n, m, i;
    std::cin >> n >> m;
    std::vector<Point> points(n);
    for (i = 0; i < n; ++i) {
        std::cin >> points[i].x >> points[i].y;
    }
 
    std::vector<std::vector<int>> graph(n);
    int u, v;
    for (i = 0; i < m; ++i) {
        std::cin >> u >> v;
        --u; --v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
 
    int q;
    std::cin >> q;
    int a, b;
    std::vector<bool> closed(n);
    std::vector<double> costSoFar(n);
    while (q--) {
        std::fill(costSoFar.begin(), costSoFar.end(), DBL_INF);
        std::fill(closed.begin(), closed.end(), false);
        std::cin >> a >> b;
        --a; --b;
        costSoFar[a] = 0;
        std::cout << aStar(a, b, points, graph, costSoFar, closed) << "\n";
    }
 
    return 0;
}
