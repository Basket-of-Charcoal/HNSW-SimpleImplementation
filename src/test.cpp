#include "data.hpp"
#include "distance.hpp"
#include "hnsw.hpp"

using namespace std;

int main() {
    srand(time(0));

    Data node1 = Data(vector({1.0, 2.0}));
    Data node2 = Data(vector({1.5, -3.0}));
    Data node3 = node1 + node2;
    node3.display();
    // [2.5, -1]

    Distance euclid = Distance(EUCLIDIAN);
    cout << euclid.distance(node1, node3) << endl;
    // 3.354

    HNSW hnsw = HNSW();
    cout << hnsw.randMaxLevel() << endl;
    return 0;
}