#ifndef _DATA
#define _DATA

#include <cstdarg>
#include <vector>
#include <iostream>
#include <cmath>

class Data {
public:
    std::vector<double> _v;
    int _dim;

    Data() {
        this->_dim = 0;
    }

    Data(std::vector<double> v) {
        this->_dim = v.size();
        this->_v = v;
    }
    
    void display() {
        std::cout << "[";
        for (int i = 0; i < this->_dim; i++) {
            if (i) {
                std::cout << ", ";
            }
            std::cout << this->_v[i];
        }
        std::cout << "]" << std::endl;
        return;
    }

    Data operator=(Data &other)
    {
        this->_dim = other._dim;
        this->_v = other._v;
        return *this;
    }

    Data operator+(Data &other)
    {
        assert(this->_dim == other._dim);
        std::vector<double> resp;
        for (int i = 0; i < this->_dim; i++) {
            resp.push_back(this->_v[i] + other._v[i]);
        }
        return resp;
    }

    Data operator-(Data &other)
    {
        assert(this->_dim == other._dim);
        std::vector<double> resp;
        for (int i = 0; i < this->_dim; i++) {
            resp.push_back(this->_v[i] - other._v[i]);
        }
        return resp;
    }

    Data operator*(double &other)
    {
        std::vector<double> resp;
        for (int i = 0; i < this->_dim; i++) {
            resp.push_back(this->_v[i] * other);
        }
        return resp;
    }

    Data operator/(double &other)
    {
        assert(other != 0);
        std::vector<double> resp;
        for (int i = 0; i < this->_dim; i++) {
            resp.push_back(this->_v[i] / other);
        }
        return resp;
    }

    double operator*(Data &other)
    {
        assert(this->_dim == other._dim);
        double sum = 0.0;
        for (int i = 0; i < this->_dim; i++) {
            sum += this->_v[i] * other._v[i];
        }
        return sum;
    }

    double length() {
        double sum = 0.0;
        for (int i = 0; i < this->_dim; i++) {
            sum += this->_v[i] * this->_v[i];
        }
        return sqrt(sum);
    }
};

#endif