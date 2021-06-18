#ifndef __IRIS_H_
#define __IRIS_H_

#include "entity.h"
#include <iostream>

namespace iris {

enum class Labels { Setosa = 0, Versicolor, Virginica, Nb = 3 };
using Entity = knn::Entity<double, 4, Labels, int>;

inline std::ostream& operator<<(std::ostream& _out, const Labels& type) {
    switch (type) {
    case Labels::Setosa:      return _out << "Setosa";
    case Labels::Versicolor:  return _out << "Versicolor";
    case Labels::Virginica:   return _out << "Virginica";
    default:                  return _out << "None";
    }
}

// ex: 6.2,3.4,5.4,2.3,Iris-virginica
inline std::istream& operator>>(std::istream& _in, knn::Entity<double, 4, Labels, int>& entity) {
    std::string buf;
    for (int i=0; i<4; ++i) {
        std::getline(_in, buf, ',');
        entity[i] = stof(buf);
    }
    std::getline(_in, buf);
    switch (buf.size()) {// ex: Iris-virginica
    case 11: entity.label = Labels::Setosa; break;
    case 15: entity.label = Labels::Versicolor; break;
    case 14: entity.label = Labels::Virginica; break;
    }
    return _in;
}



}

#endif
