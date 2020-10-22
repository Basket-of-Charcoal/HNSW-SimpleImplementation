#ifndef _DISTANCE
#define _DISTANCE

#include <cmath>
#include "data.h"

enum DISTANCE_TYPE {
    EUCLIDIAN,
    MANHATTAN,
    COSINE
};

double euclidian(Data a, Data b) {
    assert(a._dim == b._dim);
    double sum = 0.0;
    for (int i = 0; i < a._dim; i++) {
        sum += (a._v[i] - b._v[i]) * (a._v[i] - b._v[i]);
    }
    return sqrt(sum);
}

double manhattan(Data a, Data b) {
    assert(a._dim == b._dim);
    double sum = 0.0;
    for (int i = 0; i < a._dim; i++) {
        sum += abs(a._v[i] - b._v[i]);
    }
    return sum;
}

double cosine(Data a, Data b) {
    double dot = a * b;
    return dot / a.length() / b.length();
}

class Distance {
    DISTANCE_TYPE _distanceType;
public:
    Distance() {
        this->_distanceType = EUCLIDIAN;
    }

    Distance(DISTANCE_TYPE distanceType) {
        this->_distanceType = distanceType;
    }

    double distance(Data a, Data b) {
        switch (this->_distanceType)
        {
            case EUCLIDIAN:
                return euclidian(a, b);
            case MANHATTAN:
                return manhattan(a, b);
            case COSINE:
                return cosine(a, b);
        }
    }
};

#endif