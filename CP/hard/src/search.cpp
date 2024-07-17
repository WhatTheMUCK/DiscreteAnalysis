#include "search.h"

inline Node getNode(uint32_t number, FILE* inputFile, uint32_t infoStart){
    fseek(inputFile, infoStart + number * 2 * sizeof(double), SEEK_SET);

    double lat, lon;
    fread(&lat, sizeof(lat), 1, inputFile);
    fread(&lon, sizeof(lon), 1, inputFile);

    return {0, lat, lon};
}

size_t binSearch(uint32_t cur, vector<uint32_t>& ids){
    int l = 0, r = ids.size();
    while(l < r){
        int m = (l + r) / 2;
        if(ids[m] < cur){
            l = m + 1;
        } else{
            r = m;
        }
    }

    if(ids[l] != cur){
        throw logic_error("Ligma");
    }

    return l;
}

inline double radians(double x){
    return x * M_PI / ANGLE;
}

inline double calcDistance(const Node& a, const Node& b){
    double deltaLat = radians(b.lat) - radians(a.lat);
    double deltaLon = radians(b.lon) - radians(a.lon);
    return 2 * SPHERE_RADIUS * asin(sqrt(pow(sin(deltaLat / 2), 2) + cos(radians(a.lat)) * cos(radians(b.lat)) * pow(sin(deltaLon / 2), 2)));
}

void readGraph(FILE*& inputFile, vector<uint32_t>& ids, vector<uint32_t>& offsets){
    fseek(inputFile, 0, SEEK_SET);

    size_t size = 0;
    assert(fread(&size, sizeof(size), 1, inputFile) == 1);

    ids.resize(size);
    assert(fread(&ids[0], sizeof(ids[0]), size, inputFile) == ids.size());

    fseek(inputFile, sizeof(size) + size * sizeof(ids[0]) + 2 * size * sizeof(double), SEEK_SET);

    offsets.resize(size);
    assert(fread(&offsets[0], sizeof(offsets[0]), size, inputFile) == offsets.size());
}

vector<uint32_t> recoverPath(uint32_t from, uint32_t goal, vector<uint32_t>& ids, vector<uint32_t>& parent){
    uint32_t cur = binSearch(goal - 1, ids);
    from = binSearch(from - 1, ids);
    vector<uint32_t> path;

    path.push_back(ids[cur]);

    while(cur != from) {
        cur = parent[cur];
        path.push_back(ids[cur]);    
    }

    return path;
}

double AStar(uint32_t start, uint32_t goal, vector<uint32_t>& ids, 
             vector<uint32_t>& offsets, FILE* inputFile, uint32_t infoStart, uint32_t adjStart, 
             vector<double>& g, vector<double>& f, vector<uint32_t>& parent){
    
    g.assign(g.size(), -1.0);
    f.assign(f.size(), -1.0);
    priority_queue<Path> q;

    start = binSearch(start, ids);
    goal = binSearch(goal, ids);

    Node goalNode = getNode(goal, inputFile, infoStart);

    g[start] = 0;
    f[start] = g[start] + calcDistance(getNode(start, inputFile, infoStart), goalNode);
    q.push({start, f[start]});

    parent[start] = start;

    while(!q.empty()){
        uint32_t cur = q.top().to;
        double curCost = q.top().cost;
        q.pop();
        
        if(cur == goal){
            break;
        }

        if(curCost > f[cur]){
            continue;
        }

        size_t startLoad = 0, toLoad = 0;
        if(cur == 0){
            toLoad = offsets[0];
        } else{
            startLoad = offsets[cur - 1]; //сколько ребер пропустить
            toLoad = offsets[cur] - offsets[cur - 1]; //скольлко ребер у текущей вершины
        }

        vector<uint32_t> curAdj(toLoad); //список доступных вершин из данной
        fseek(inputFile, adjStart + startLoad * sizeof(uint32_t), SEEK_SET);
        for(size_t i = 0; i < toLoad; ++i){
            fread(&curAdj[i], sizeof(uint32_t), 1, inputFile);
        }

        Node curNode = getNode(cur, inputFile, infoStart);

        for(uint32_t next : curAdj){
            next = binSearch(next, ids);

            Node nextNode = getNode(next, inputFile, infoStart);

            double tentativeScore = g[cur] + calcDistance(nextNode, curNode);

            if((g[next] < 0) || (1e-6 < (g[next] - tentativeScore))){
                g[next] = tentativeScore;
                f[next] = g[next] + calcDistance(nextNode, goalNode);

                parent[next] = cur;
                q.push({next, f[next]});
            }
        }
    }

    if(g[goal] == -1.0){
        return __DBL_MAX__;
    }

    return g[goal];
}

void search(FILE* graphFile, FILE* inputFile, FILE* outputFile, bool fullOutput){
    vector<uint32_t> ids;
    vector<uint32_t> offsets;
    readGraph(graphFile, ids, offsets);
    fseek(graphFile, 0, SEEK_SET);

    uint32_t infoStart = sizeof(size_t) + sizeof(ids[0]) * ids.size();
    uint32_t adjStart = infoStart + 2 * sizeof(double) * offsets.size() + sizeof(offsets[0]) * offsets.size();

    uint32_t from, goal;
    vector<double> g(ids.size() + 1, -1.0);
    vector<double> f(ids.size() + 1, -1.0);
    vector<uint32_t> parent(ids.size() + 1);

    while(fscanf(inputFile, "%u%u", &from, &goal) == 2){                
        double ans = AStar(from - 1, goal - 1, ids, offsets, graphFile, infoStart, adjStart, g, f, parent);
        
        if(ans == __DBL_MAX__){
            fprintf(outputFile, "%d", -1);
        } else{
            fprintf(outputFile, "%#.6f", ans);
        }

        if(fullOutput){
            if(ans == __DBL_MAX__){
                fprintf(outputFile, " %d", 0);
            } else{
                vector<uint32_t> path = recoverPath(from, goal, ids, parent);
                fprintf(outputFile, " %zu ", path.size());

                for(size_t i = 0; i < path.size(); ++i){
                    fprintf(outputFile, "%u ", path[path.size() - 1 - i] + 1);
                }
            }
        }

        fprintf(outputFile, "\n");
    }
}