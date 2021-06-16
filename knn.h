#ifndef __KNN_H_
#define __KNN_H_

#include <array>
#include <iosfwd>
#include <string_view>
#include <type_traits>

namespace knn {


static constexpr auto k = 6;
static constexpr auto N = 100;
static constexpr auto MAX = 150;


enum class IrisType { Setosa, Versicolour, Virginica };

template < typename E >
typename std::underlying_type<E>::type to_integral(const E e) { return std::underlying_type<IrisType>(e); }

struct Iris {
    std::array<double, 4> m_chars;
    // double sepal_l{0}, sepal_w{0}, petal_l{0}, petal_w{0};
    IrisType type;
    double operator[](int i) const {
        return m_chars[i];
    }
};


struct Data {
    using Index = int;
    std::array<Iris, N> m_data;
};

struct Nbh {
    using Index = Data::Index;
    Iris center;
    std::array<Index, k> m_nbh;
};


class KNN {
    using Index = Data::Index;
    using NbhData = std::array<Nbh, N>;
public:
    KNN(std::string_view);
    std::array<Iris, k> query (const Iris&);

private:
    NbhData m_data;
    int m_size;
    std::array<double, MAX> m_dist2_map;
    double dist2(const Iris&, const Iris&) const;
    std::array<Index, k> query_impl (const Iris&);
};


} // namespace knn



#endif // __KNN_H_
