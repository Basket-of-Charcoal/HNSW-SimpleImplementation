#include "hnsw.h"

SearchResponse::SearchResponse(int nodeId, double distance)
{
    this->_nodeId = nodeId;
    this->_distance = distance;
};

bool SearchResponse::operator<(const SearchResponse &another) const
{
    if (this->_distance != another._distance)
    {
        return this->_distance < another._distance;
    }
    return this->_nodeId < another._nodeId;
}

HNSW::HNSW()
{
    this->_layerNum = 3;
    this->_neighborNum = 5;
    this->defaultInit();
}

HNSW::HNSW(int layerNum, int neighborNum)
{
    this->_layerNum = layerNum;
    this->_neighborNum = neighborNum;
    this->defaultInit();
}

void HNSW::defaultInit()
{
    this->_level_constant = 1 / log2(
                                    this->_layerNum);
    this->_distance = Distance(EUCLIDIAN);

    // create empty graph
    for (int i = 0; i < this->_layerNum; i++)
    {
        this->_hnswGraph.push_back(std::map<int, Node>());
    }
}

int HNSW::randMaxLayer()
{
    // std::cout << -log2((rand() % _RAND_MAX)
    //     / (double) _RAND_MAX) << this->_level_constant << std::endl;
    return floor(
               -log2((rand() % _RAND_MAX) / (double)_RAND_MAX) * this->_level_constant) +
           1;
}

void HNSW::insert(Data data)
{
    int layer = this->randMaxLayer() - 1;
    int dataId = this->_data.size();
    std::cout << dataId << " " << layer << std::endl;

    this->_data.push_back(data);

    // insert root node when graph was empty
    if (dataId == ROOT)
    {
        for (int i = 0; i < this->_layerNum; i++)
        {
            this->_hnswGraph[i][ROOT] = Node();
        }
    }
    else
    {
        for (int i = 0; i <= layer; i++)
        {
            this->_hnswGraph[i][dataId] = Node();
        }
        double dist = this->_distance.distance(
            data, this->_data[ROOT]);
        int nearestId = ROOT;

        // search nearest neighboor in layers higher than this node
        for (int i = this->_layerNum - 1; i > layer; i--)
        {
            SearchResponse response = this->searchInSpecificLayer(data,
                                                                  dist, nearestId, i, 1)[0];
            nearestId = response._nodeId;
            dist = response._distance;
        }

        // search neighboors and connect
        for (int i = layer; i > 0; i--)
        {
            std::vector<SearchResponse> response = this->searchInSpecificLayer(data,
                                                                               dist, nearestId, i, this->_neighborNum);
            nearestId = response[0]._nodeId;
            dist = response[0]._distance;

            for (SearchResponse n : response)
            {
                if (n._nodeId == dataId)
                {
                    continue;
                }
                this->_hnswGraph[i][dataId]._neighbor.push_back(n._nodeId);
                this->_hnswGraph[i][n._nodeId]._neighbor.push_back(dataId);
            }
            std::cout << "create edges: " << response.size() << std::endl;
        }
    }
}

std::vector<SearchResponse> HNSW::search(
    Data data,
    int responseSize)
{
    int nearestId = ROOT;
    double dist = this->_distance.distance(
        data, this->_data[ROOT]);
    for (int i = this->_layerNum - 1; i > 0; i--)
    {
        SearchResponse response = this->searchInSpecificLayer(data,
                                                              dist, nearestId, i, 1)[0];
        nearestId = response._nodeId;
        dist = response._distance;
    }
    // std::cout << "finded " << nearestId << " " << dist << std::endl;
    return this->searchInSpecificLayer(
        data, dist, nearestId, 0, responseSize);
}

std::vector<SearchResponse> HNSW::searchInSpecificLayer(
    Data data,
    double distThreshold,
    int startNodeId,
    int layerId,
    int responseSize)
{
    std::set<int> visited;
    std::set<SearchResponse> waitingPool;
    std::set<SearchResponse> responsePool;
    double distTh = distThreshold;
    int bestNodeId = ROOT;

    waitingPool.insert(SearchResponse(startNodeId, distThreshold));
    while (!waitingPool.empty())
    {
        SearchResponse closestNode = *waitingPool.begin();
        waitingPool.erase(waitingPool.begin());

        std::cout << "startNode: " << closestNode._nodeId << " " << closestNode._distance << std::endl;
        if (visited.find(closestNode._nodeId) != visited.end() || closestNode._distance > distTh)
        {
            continue;
        }
        visited.insert(closestNode._nodeId);
        responsePool.insert(closestNode);
        if (distTh > closestNode._distance)
        {
            distTh = closestNode._distance;
            bestNodeId = closestNode._nodeId;
        }

        for (int neighbor : this->_hnswGraph[layerId][closestNode._nodeId]._neighbor)
        {
            SearchResponse neighborResponse = SearchResponse(
                neighbor,
                this->_distance.distance(data, this->_data[neighbor]));
            if (visited.find(neighbor) != visited.end())
            {
                waitingPool.insert(neighborResponse);
            }
        }
    }

    std::cout << "search response: " << bestNodeId << " " << distTh << std::endl;
    std::vector<SearchResponse> response;
    for (SearchResponse r : responsePool)
    {
        if (!responseSize)
        {
            break;
        }
        responseSize--;
        response.push_back(r);
    }
    return response;
}