#ifndef __KNN2_H_
#define __KNN2_H_

#include <algorithm>
#include <cassert>
#include <functional>
#include <numeric>

#include "entity.h"


namespace knn {
namespace utils {

template < std::size_t N >
auto constexpr one_to() {
    auto ret = std::array<int, N> {};
    std::iota(ret.begin(), ret.end(), 0);
    return ret;
}

template < typename E >
typename std::underlying_type_t<E> to_integral(const E e) { return static_cast<std::underlying_type_t<E>>(e); }
template < typename E, typename IntT > E to_enum(const IntT i) { return E(i); }

} // namespace utils


template < typename EntityT, std::size_t NbhSize, std::size_t NEntityMax >
class KNN {
public:
    using Entity = EntityT;
    using Index = typename Entity::Index;
    using Feature_t = typename Entity::Feature_t;
    using Features = typename Entity::Features;
    using Label = typename Entity::Label;
    using Distance_t = double;

    constexpr KNN()
        : m_entity_map{}
        , m_ndx_map{utils::one_to<NEntityMax>()}
        , m_dist_map{std::numeric_limits<Distance_t>::max()}
    {}

    template < typename InputIterator >
    void set_training_data(InputIterator _beg, InputIterator _end) {
        auto size = std::distance(_beg, _end);
        if (size < 0)
            std::swap(_beg, _end);
        std::copy(_beg, _end, m_entity_map.begin());
        m_size = abs(size);
    }

    Label most_likely_label(const Entity& entity) const {
        auto indexed_counter = count_nearby_labels(entity);
        auto cmp_second = [](const auto& a, const auto& b) { return a.second < b.second; };
        auto best_label_i = std::max_element(indexed_counter.begin(), indexed_counter.end(), cmp_second)->first;
        return utils::to_enum<Label>(best_label_i);
   }

    std::size_t size() const { return m_size; }
    std::size_t max_size() const { return NEntityMax; }
    std::size_t nbh_size() const { return NbhSize; }

private:
    using Index_map = std::array<Index, NEntityMax>;
    using Entity_map = std::array<Entity, NEntityMax>;
    using Distance_map = std::array<Distance_t, NEntityMax>;

    Entity_map           m_entity_map;
    mutable Index_map    m_ndx_map;
    mutable Distance_map m_dist_map;
    std::size_t m_size;

    auto count_nearby_labels(const Entity& entity) const {
        std::array<std::pair<int, int>, Entity::n_labels> ret {std::pair{0, 0}};  // (label_i, count)
        for (auto [i, it] = std::make_pair(0, ret.begin()); it!=ret.end(); ++it, ++i) {
            it->first = i;
        }
        find_nbhs_indices(entity);
        for (int i=0; i<NbhSize; ++i) {
            Label label = m_entity_map[m_ndx_map[i]].label;
            ++ret[utils::to_integral(label)].second;
        }
        return ret;
    }

    void find_nbhs_indices(const Entity& q) const {
        compute_distances_from(q);
        std::partial_sort(m_ndx_map.begin(), m_ndx_map.begin() + NbhSize, m_ndx_map.begin() + m_size, [&](auto a, auto b) {
            return m_dist_map[a] < m_dist_map[b];
        });
    }

    void compute_distances_from(const Entity& q) const {
        for (int i=0; i<m_size; ++i) {
            m_dist_map[i] = dist(q, m_entity_map[i]);
        }
    }

    Distance_t dist(const Entity& a, const Entity& b) const { // distance squared
        auto res = 0.0;
        for (int i=0; i<Entity::n_features(); ++i) {
            res += (a[i] - b[i]) * (a[i] - b[i]);
        }
        return res;
    }
};

} // namespace knn


#endif // KNN2_H_
