#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <queue>
#include <chrono>
#include <cstdio>
#include <cassert>
#include <exception>

using namespace std;

struct Node{
    uint32_t id;
    double lat;
    double lon;

    friend istream& operator>>(istream& is, Node& node){
        is >> node.id >> node.lat >> node.lon;
        return is;
    }

    friend bool operator<(Node& lhs, Node& rhs){
        return lhs.id < rhs.id;
    }
};

const double ANGLE = 180.0;
const double SPHERE_RADIUS = 6371 * 1e3;

struct Path{
    uint32_t to;
    double cost;

    friend bool operator<(const Path& a, const Path& b){
        if(a.cost != b.cost){
            return a.cost > b.cost;
        }
        return a.to > b.to;
    }
};

struct Edge{
    uint32_t from;
    uint32_t to;

    friend bool operator<(const Edge& a, const Edge& b){
        if(a.from != b.from){
            return a.from < b.from;
        }
        return a.to < b.to;
    }
};

inline Node getNode(uint32_t number, FILE* inputFile, uint32_t infoStart);

size_t binSearch(uint32_t cur, vector<uint32_t>& ids);

inline double radians(double x);

inline double calcDistance(const Node& a, const Node& b);