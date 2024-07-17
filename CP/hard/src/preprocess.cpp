#include "preprocess.h"

void preprocessNodes(FILE* nodesFile, FILE* outputFile, vector<uint32_t>& nodes_id){
    {
        vector<Node> nodes;    
        Node cur;       
        while(fscanf(nodesFile, "%u%lf%lf", &cur.id, &cur.lat, &cur.lon) > 0){
            cur.id -= 1;
            nodes.push_back(cur);
        }

        sort(nodes.begin(), nodes.end());

        nodes_id.resize(nodes.size());
        for(size_t i = 0; i < nodes.size(); ++i){
            nodes_id[i] = nodes[i].id;
        }

        size_t size = nodes.size();
        fwrite(&size, sizeof(size), 1, outputFile);
        fwrite(&nodes_id[0], sizeof(nodes_id[0]), nodes_id.size(), outputFile); 
        for(size_t i = 0; i < size; ++i){
            fwrite(&nodes[i].lat, sizeof(nodes[0].lat), 1, outputFile); 
            fwrite(&nodes[i].lon, sizeof(nodes[0].lon), 1, outputFile);
        }
    }
}

void preprocessEdges(FILE* edgesFile, FILE* outputFile, vector<uint32_t>& nodes_id){
    vector<Edge> edges;
    uint32_t n, curId, prevId;
    while(fscanf(edgesFile, "%u%u", &n, &curId) == 2){
        for(size_t i = 1; i < n; ++i){
            prevId = curId;
            fscanf(edgesFile, "%u", &curId);
            edges.push_back({prevId - 1, curId - 1});
            edges.push_back({curId - 1, prevId - 1}); 
        }
    }

    sort(edges.begin(), edges.end());

    uint32_t i = 0;
    for(size_t k = 0; k < nodes_id.size(); ++k){
        uint32_t curEdgeFrom = nodes_id[k];
        while(i < edges.size() && edges[i].from == curEdgeFrom){
            ++i;
        }
        fwrite(&i, sizeof(i), 1, outputFile);
    }

    for(Edge& edge : edges){
        fwrite(&edge.to, sizeof(edge.to), 1, outputFile);
    }
}