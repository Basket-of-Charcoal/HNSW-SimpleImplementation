#ifndef _HNSW
#define _HNSW

#include <vector>
#include <cmath>
#include <iostream>
#include "data.hpp"

#define _RAND_MAX 10000

struct Node {
    std::vector<int> _neighbor;
    int _dataId;
};

class HNSW {
    int _layerNum;
    int _neighborNum;
    std::vector<Data> _data;
    std::vector<
        std::vector<Node>
    > _hnswGraph;
    double _level_constant;
public:
    HNSW() {
        this->_layerNum = 3;
        this->_neighborNum = 5;
        this->_level_constant = 1 / log2(
            this->_layerNum
        );
    }

    HNSW(int layerNum, int neighborNum) {
        this->_layerNum = layerNum;
        this->_neighborNum = neighborNum;
        this->_level_constant = 1 / log2(
            this->_layerNum
        );
    }

    int randMaxLevel() {
        // std::cout << -log2((rand() % _RAND_MAX)
        //     / (double) _RAND_MAX) << this->_level_constant << std::endl;
        return floor(
            -log2((rand() % _RAND_MAX)
            / (double) _RAND_MAX)
            * this->_level_constant)
            + 1;
    }

    
};

#endif