#ifndef MNIST_H_
#define MNIST_H_

#include "entity.h"

#include <iostream>
#include <string>
#include <sstream>

namespace mnist {


enum class Label { Zero = 0, One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Nb=10 };

using Entity = knn::Entity<uint8_t, 784, Label, int>;

inline std::ostream& operator<<(std::ostream& _out, const Label& label) {
    return _out << static_cast<int>(label);
}

// TODO: Average out the values in squares to bring down the number of dimensions
//inline make_coarser(const Entity& )


// inline std::istream& operator>>(std::istream& _in, knn::Entity<uint8_t, 784, Label, int>& entity) {
//     std::string buf;
//     std::getline(_in, buf);
//     std::istringstream ss{buf};

//     std::getline(ss, buf, ',');
//     entity.label = Label(stoi(buf));

//     for (int i=1; i<784; ++i) {
//         std::getline(ss, buf, ',');
//         entity[i] = static_cast<uint8_t>(std::stoi(buf));
//     }
//     return _in;
// }

} // namespace mnist

#endif // MNIST_H_
