from math import *
from random import *

MIN_lon = -90
MAX_lon = 90
MIN_lat = -180
MAX_lat = 180

DIGITS = 6
SPHERE_RADIUS = 6371 * 10 ** 3
PI = 3.1415926535897932384626433832795
ANGLE = 180.0
 
def rand_cord():
    return (
        randint(MIN_lat * 10 ** DIGITS, MAX_lat * 10 ** DIGITS) / (10 ** DIGITS),
        randint(MIN_lon * 10 ** DIGITS, MAX_lon * 10 ** DIGITS) / (10 ** DIGITS)
    )

def my_radians(x):
    return x * PI / ANGLE

def dist(u, v):
    (id_u, lat_u, lon_u) = u
    (id_v, lat_v, lon_v) = v

    deltalat = radians(lon_u) - radians(lon_v)
    deltalon = radians(lat_u) - radians(lat_v)
    return 2 * SPHERE_RADIUS * asin(sqrt((sin(deltalon / 2) ** 2) + cos(radians(lat_u)) * cos(radians(lat_v)) * (sin(deltalat / 2) ** 2)))

TEST_COUNT = 10
MIN_VERTICIES = 20
MAX_VERTICIES = 20
MIN_ID = 1
MAX_ID = 2 ** 32 - 1
MIN_EDGES = 50
MAX_EDGES = 500
MIN_ROADS = 10
MAX_ROADS = 20
MIN_ROAD_LEN = 5
MAX_ROAD_LEN = 20
MIN_QUERY = 5
MAX_QUERY = 50

INF = 10 ** 18

for i in range(1, TEST_COUNT + 1):
    nodes_file = open("tests/" + str(i) + ".nodes", "w")
    edges_file = open("tests/" + str(i) + ".edges", "w")
    input_file = open("tests/" + str(i) + ".in", "w")
    output_file = open("tests/" + str(i) + ".out", "w")

    n = randint(MIN_VERTICIES, MAX_VERTICIES)
    verticies = []
    ids = set()
    for j in range(n):
        vertex_id = randint(MIN_ID, MAX_ID)
        while (vertex_id in ids):
            vertex_id = randint(MIN_ID, MAX_ID)
        ids.add(vertex_id)
        (vertex_lat, vertex_lon) = rand_cord()
        verticies.append((vertex_id, vertex_lat, vertex_lon))
        nodes_file.write(str(verticies[-1][0]) + " " + "{:.6f}".format(verticies[-1][1]) + " " + "{:.6f}".format(verticies[-1][2]) + "\n")

    m = min((n * (n - 1)) // 2, randint(MIN_EDGES, MAX_EDGES))
    exist_edges = set()
    edges = []
    for j in range(m):
        u = randint(1, n)
        v = randint(1, n)
        while (v == u):
            v = randint(1, n)
        while ((u, v) in exist_edges or (v, u) in exist_edges):
            u = randint(1, n)
            v = randint(1, n)
            while (v == u):
                v = randint(1, n)
        exist_edges.add((u, v))

        edges.append((u, v))
        edges.append((v, u))

    roads = randint(MIN_ROADS, MAX_ROADS)

    for j in range(roads):
        u = randint(1, n)
        road = [u]
        road_len = randint(MIN_ROAD_LEN, MAX_ROAD_LEN)
        for k in range(road_len):
            prolatg = False
            for v in range(1, n):
                if ((u, v) in exist_edges):
                    road.append(v)
                    exist_edges.remove((u, v))
                    prolatg = True
                    break
            if (prolatg):
                u = road[-1]
            if (len(road) == road_len):
                break
        if (len(road) > 1):
            edges_file.write(str(len(road)) + " ")
            for k in range(0, len(road) - 1):
                edges_file.write(str(verticies[road[k] - 1][0]) + " ")
            edges_file.write(str(verticies[road[-1] - 1][0]) + "\n")

    for j in exist_edges:
        edges_file.write("2 " + str(verticies[j[0] - 1][0]) + " " + str(verticies[j[1] - 1][0]) + "\n")

    q = randint(MIN_QUERY, MAX_QUERY)
    query = []
    for j in range(q):
        u = randint(1, n)
        v = randint(1, n)
        while (u == v):
            v = randint(1, n)
        query.append((u, v))

    # Ford-Bellman
    ans = []
    paths = []
    for j in range(q):
        d = [INF for ii in range(n)]
        prev = [-1 for ii in range(n)]
        d[query[j][0] - 1] = 0
        for ii in range(2 * n):
            for jj in range(2 * m):
                u = edges[jj][0] - 1
                v = edges[jj][1] - 1
                if (d[u] + dist(verticies[u], verticies[v]) < d[v]):
                    d[v] = d[u] + dist(verticies[u], verticies[v])
                    prev[v] = u
        path = []
        cur = query[j][1] - 1
        while (prev[cur] != -1):
            path.append(verticies[cur][0])
            cur = prev[cur]
        path.append(verticies[cur][0])
        path = path[::-1]
        ans.append(d[query[j][1] - 1])
        paths.append(path)

    # .in
    for j in range(q):
        id_u = verticies[query[j][0] - 1][0]
        id_v = verticies[query[j][1] - 1][0]
        input_file.write(str(id_u) + " " + str(id_v) + "\n")

    # .out
    for j in range(q):
        if (ans[j] < INF):
            path = paths[j]
            output_file.write("{:.6f}".format(ans[j]) + "\n")
        else:
            output_file.write("-1\n")

    nodes_file.close()
    edges_file.close()
    input_file.close()
    output_file.close()