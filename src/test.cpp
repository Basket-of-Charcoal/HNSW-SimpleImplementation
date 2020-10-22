#include "data.h"
#include "distance.h"
#include "hnsw.h"

using namespace std;

int main()
{
    srand(time(0));

    Data node1 = Data(vector<double>({1.0, 2.0}));
    Data node2 = Data(vector<double>({1.5, -3.0}));
    Data node3 = node1 + node2;
    node3.display();
    // [2.5, -1]

    Distance euclid = Distance(EUCLIDIAN);
    cout << euclid.distance(node1, node3) << endl;
    // 3.354

    HNSW hnsw = HNSW();
    hnsw.insert(node1);
    hnsw.insert(node2);
    hnsw.insert(node3);

    vector<SearchResponse> response = hnsw.search(node1, 2);
    cout << "Test1 ---------" << endl;
    for (SearchResponse r : response)
    {
        cout << r._nodeId << " " << r._distance << endl;
    }
    cout << "---------" << endl;
    return 0;
}