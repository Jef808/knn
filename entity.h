#ifndef __ENTITY_H_
#define __ENTITY_H_

#include <array>
#include <iosfwd>

namespace knn {


template < typename FeatureT, std::size_t NFeatures, typename Label_enum, typename IndexT = int >
struct Entity;

template < typename FeatureT, std::size_t NFeatures, typename Label_enum, typename IndexT >
extern std::istream& operator>>(std::istream& _in, const Entity<FeatureT, NFeatures, Label_enum, IndexT>&);

template < typename FeatureT, std::size_t NFeatures, typename Label_enum, typename IndexT >
struct Entity {
    using Index = IndexT;
    using Feature_t = FeatureT;
    using Features = std::array<Feature_t, NFeatures>;
    using Label = Label_enum;

    constexpr Entity() = default;
    constexpr explicit Entity(Index _ndx) : ndx{_ndx} {}
    Index ndx { 0 };
    Features features { 0 };
    Label label { Label::Nb };

    Feature_t& operator[](int i) { return features[i]; }
    Feature_t operator[](int i) const { return features[i]; }
    static const std::size_t n_features() { return NFeatures; }
    static const typename std::underlying_type<Label_enum>::type n_labels = static_cast<typename std::underlying_type<Label_enum>::type>(Label::Nb);
    friend std::istream& operator>> <> (std::istream& _in, const Entity&);
};

} // namespace knn

#endif
