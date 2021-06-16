#ifndef __KNN_H_
#define __KNN_H_

#include <array>
#include <iosfwd>


namespace knn {


static constexpr auto k = 6;
static constexpr auto N = 150;


enum class IrisType { Setosa, Versicolour, Virginica };

struct Iris {
    double sepal_l{0}, sepal_w{0}, petal_l{0}, petal_w{0};
    IrisType type;
};


struct Data {
    using Index = int;
    std::array<Iris, N> m_data;
};

struct Nbh {
    using Index = Data::Index;
    Index center;
    std::array<Index, k> m_nbh;
};


class KNN {
    using Index = Data::Index;
    using NbhData = std::array<Nbh, N>;
public:
    void input_data(std::istream& _in) {};

double dot(const Iris& a, const Iris& b) {
    double res = 0;
    for (int i=0; i<4; ++i) {
        res += a[i] * b[i]
    }
    return res;
}

        double dist2(const Iris& a, const Iris& b) {
            double res = 0;
            for (int i=0; i<4; ++i) {
                res += (a[i] - b[i]) * (a[i] - b[i]);
            }
            return res;
        }
private:
    NbhData m_data;
};


} // namespace knn



#endif // __KNN_H_
