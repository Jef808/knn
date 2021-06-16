#include "knn.h"

#include <cassert>
#include <iostream>
#include <fstream>
//#include <string>
#include <vector>

namespace knn {


void operator>>(std::istream& _in, Iris& i) {

}

KNN::KNN(std::string_view fn_sv)
    : m_data()
    , m_size{N}
{
    auto filename = std::string(fn_sv);
    std::ifstream _if;
    _if.open(filename);
    if (!_if) {
        std::cerr << "Cannot open file data/iris.data" << std::endl;
        assert(1 == 0);
    }

    for (int i=0; i<N; ++i) {
        _if >> m_data[i].center;
    }
};

std::array<Iris, k> KNN::query (const Iris& iris) {
    std::array<Iris, k> ret {};
    auto indices = query_impl(iris);
    for (int i=0; i<k; ++i) {
        ret[i] = m_data[indices[i]].center;
    }
    return ret;
}

std::array<Data::Index, k> KNN::query_impl(const Iris& q) {
    std::array<Index, k> indices;
    for (int i=0; i<m_size; ++i) {
        m_dist2_map[i] = dist2(q, m_data[i].center);
    }
    std::sort(indices.begin(), indices.end(), [&](const auto a, const auto b) {
        return m_dist2_map[a] < m_dist2_map[b];
    });
    return indices;
}

double KNN::dist2(const Iris& a, const Iris& b) const {
    double res = 0;
    for (int i=0; i<4; ++i) {
        res += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return res;
}


} // namespace knn


using namespace knn;

const std::string filename = std::string("data/iris.data");

int main()
{
    const int n_query = 50;
    auto knn = KNN(filename);

    std::vector<Iris> query;
    query.reserve(50);

    return 0;
}
