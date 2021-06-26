#ifndef __ENTITY_H_
#define __ENTITY_H_

#include <array>
#include <concepts>
#include <iosfwd>
#include <iterator>
#include <type_traits>

namespace knn {


template < typename FeatureT, std::size_t NFeatures, typename Label_enum, typename IndexT = int >
struct Entity;

template < typename FeatureT, std::size_t NFeatures, typename Label_enum, typename IndexT >
extern std::istream& operator>>(std::istream& _in, const Entity<FeatureT, NFeatures, Label_enum, IndexT>&);

template < typename FeatureT, std::size_t NFeatures, typename Label_enum, typename IndexT >
struct Entity {
    using Index_t = IndexT;
    using Feature_t = FeatureT;
    using Features = std::array<Feature_t, NFeatures>;
    using Label_t = Label_enum;
    using iterator = typename Features::iterator;
    using const_iterator = typename Features::const_iterator;
    using Entity_t = Entity< FeatureT, NFeatures, Label_enum, IndexT >;

    Entity() = default;
    template < typename InputIterator >
    Entity(Index_t _ndx, Label_t _label, InputIterator _beg, InputIterator _end) : ndx{input_ndx(_ndx)}, features{input_features(_beg, _end)}, label(input_label(_label)) {}
    template < typename InputIterator >
    Entity(Index_t _ndx, Label_t _label, InputIterator _beg) : ndx{input_ndx(_ndx)}, features{input_features(_beg)}, label(input_label(_label)) {}
    Index_t ndx { 0 };
    Features features { 0 };
    Label_t label { Label_t::Nb };

    Feature_t& operator[](int i) { return features[i]; }
    Feature_t operator[](int i) const { return features[i]; }
    iterator coordinates_begin() { return features.begin(); }
    iterator coordinates_end() { return features.end(); }
    const_iterator coordinates_begin() const { return features.begin(); }
    const_iterator coordinates_end() const { return features.end(); }

    const Index_t input_ndx(Index_t n) { return n; }
    const Label_t input_label(int l) { return static_cast<Label_t>(l); }
    template < typename I >
    const Features input_features(I _beg) { std::array<Feature_t, NFeatures> _ret; std::transform(_beg, _beg + n_features(), _ret.begin(), convert_value()); return _ret; }
    template < typename I >
    static constexpr Feature_t convert_value(typename I::value_type i) { return static_cast<Feature_t>(i); }
    template < typename I >
    static constexpr Features input_features(I _beg, I _end) { std::array<Feature_t, NFeatures> _ret; std::transform(_beg, _end, _ret.begin(), convert_value()); return _ret; }
    static constexpr std::size_t n_features() { return NFeatures; }
    static constexpr typename std::underlying_type<Label_t>::type n_labels() { return static_cast<typename std::underlying_type<Label_enum>::type>(Label_t::Nb); }
    friend std::istream& operator>> <> (std::istream& _in, const Entity<FeatureT, NFeatures, Label_enum, IndexT>&);
};

} // namespace knn

#endif
