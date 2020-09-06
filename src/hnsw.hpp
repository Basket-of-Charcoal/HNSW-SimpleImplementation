#ifndef _HNSW
#define _HNSW

#include <vector>
#include <set>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "data.hpp"
#include "distance.hpp"

#define _RAND_MAX 10000
#define ROOT 0

struct Node {
    std::vector<int> _neighbor;
};

struct SearchResponse{
    int _nodeId;
    double _distance;
    SearchResponse() {};
    SearchResponse(int nodeId, double distance) {
        this->_nodeId = nodeId;
        this->_distance = distance;

    };
    bool operator<(const SearchResponse &another) const {
        if (this->_distance != another._distance) {
            return this->_distance < another._distance;
        }
        return this->_nodeId < another._nodeId;
    }
};

class HNSW {
    int _layerNum;
    int _neighborNum;
    std::vector<Data> _data;
    std::vector<
        std::vector<Node>
    > _hnswGraph;
    double _level_constant;
    Distance _distance;

public:
    HNSW() {
        this->_layerNum = 3;
        this->_neighborNum = 5;
        this->defaultInit();
    }

    HNSW(int layerNum, int neighborNum) {
        this->_layerNum = layerNum;
        this->_neighborNum = neighborNum;
        this->defaultInit();
    }

    void defaultInit() {
        this->_level_constant = 1 / log2(
            this->_layerNum
        );
        this->_distance = Distance(EUCLIDIAN);

        // create empty graph
        for (int i = 0; i < this->_layerNum; i++) {
            this->_hnswGraph.push_back(std::vector<Node>());
        }
    }

    int randMaxLayer() {
        // std::cout << -log2((rand() % _RAND_MAX)
        //     / (double) _RAND_MAX) << this->_level_constant << std::endl;
        return floor(
            -log2((rand() % _RAND_MAX)
            / (double) _RAND_MAX)
            * this->_level_constant)
            + 1;
    }

    void insert(Data data) {
        int layer = this->randMaxLayer() - 1;
        int dataId = this->_data.size();
        std:: cout << dataId <<  " "  << layer << std::endl;

        this->_data.push_back(data);

        // insert root node when graph was empty
        if (dataId == ROOT) {
            for (int i = 0; i < this->_layerNum; i++) {
                this->_hnswGraph[i].push_back(Node());
            }
        }
        else {
            double dist = this->_distance.distance(
                data, this->_data[ROOT]
            );
            int nearestId = ROOT;

            // search nearest neighboor in layers higher than this node
            for (int i = this->_layerNum-1; i > layer; i--) {
                SearchResponse response = this->searchInSpecificLayer(data,
                    dist, nearestId, i, 1)[0];
                nearestId = response._nodeId;
                dist = response._distance;
            }

            // search neighboors and connect
            for (int i = layer; i > 0; i--) {
                std::vector<SearchResponse> response = this->searchInSpecificLayer(data,
                    dist, nearestId, i, this->_neighborNum);
                nearestId = response[0]._nodeId;
                dist = response[0]._distance;

                for (SearchResponse n: response) {
                    if (n._nodeId == dataId) {
                        continue;
                    }
                    this->_hnswGraph[i][dataId]._neighbor.push_back(n._nodeId);
                    this->_hnswGraph[i][n._nodeId]._neighbor.push_back(dataId);
                }
                std::cout << "create edges: " << response.size() << std::endl;
            }
        }
    }

    std::vector<SearchResponse> search(
        Data data,
        int responseSize
    ) {
        int nearestId;
        double dist;
        for (int i = this->_layerNum-1; i > 0; i--) {
            SearchResponse response = this->searchInSpecificLayer(data,
                INFINITY, ROOT, i, 1)[0];
            nearestId = response._nodeId;
            dist = response._distance;
        }
        return this->searchInSpecificLayer(
            data, dist, nearestId, 0, responseSize);
    }

    std::vector<SearchResponse> searchInSpecificLayer(
            Data data,
            double distThreshold,
            int startNodeId,
            int layerId,
            int responseSize) {
        std::set<int> visited;
        std::set<SearchResponse> waitingPool;
        std::set<SearchResponse> responsePool;
        double distTh = distThreshold;
        int bestNodeId = ROOT;

        waitingPool.insert(SearchResponse(startNodeId, distThreshold));
        while (!waitingPool.empty()) {
            SearchResponse closestNode = *waitingPool.begin();
            waitingPool.erase(waitingPool.begin());

            std::cout << "startNode: " << closestNode._nodeId << " " << closestNode._distance << std::endl;
            if (visited.find(closestNode._nodeId)!=visited.end()
                    || closestNode._distance > distTh) {
                continue;
            }
            visited.insert(closestNode._nodeId);
            responsePool.insert(closestNode);
            if (distTh > closestNode._distance) {
                distTh = closestNode._distance;
                bestNodeId = closestNode._nodeId;
            }

            for (int neighbor: this->_hnswGraph[layerId][closestNode._nodeId]._neighbor) {
                SearchResponse neighborResponse = SearchResponse(
                    neighbor,
                    this->_distance.distance(data, this->_data[neighbor])
                );
                if (visited.find(neighbor)!=visited.end()) {
                    waitingPool.insert(neighborResponse);
                }
            }
        }

        std::cout << "search response: " << bestNodeId << " " << distTh << std::endl;
        std::vector<SearchResponse> response;
        for (SearchResponse r: responsePool) {
            if (!responseSize) {
                break;
            }
            responseSize--;
            response.push_back(r);
        }
        return response;
    }
};

#endif