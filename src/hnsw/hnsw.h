#ifndef _HNSW
#define _HNSW

#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "data.h"
#include "distance.h"

#define _RAND_MAX 10000
#define ROOT 0

struct Node
{
    std::vector<int> _neighbor;
};

struct SearchResponse
{
    int _nodeId;
    double _distance;
    SearchResponse() = default;
    SearchResponse(int nodeId, double distance);
    bool operator<(const SearchResponse &another) const;
};

class HNSW
{
    int _layerNum;
    int _neighborNum;
    std::vector<Data> _data;
    std::vector<
        std::map<int, Node>>
        _hnswGraph;
    double _level_constant;
    Distance _distance;

public:
    HNSW();
    HNSW(int layerNum, int neighborNum);
    void insert(Data data);
    std::vector<SearchResponse> search(
        Data data,
        int responseSize);
    std::vector<SearchResponse> searchInSpecificLayer(
        Data data,
        double distThreshold,
        int startNodeId,
        int layerId,
        int responseSize);

private:
    void defaultInit();
    int randMaxLayer();
};

#endif