#ifndef KNN_H_
#define KNN_H_


#include <algorithm>
#include <numeric>
#include <cstdint>
#include <vector>
#include "point.h"


namespace knn {

//auto constexpr inline image_size = 784;
//auto constexpr inline n_labels = 10;

class KNN {
public:
    using image_type = Point_Set::image_type;
    using image_it = image_type::const_iterator;

    const int image_size;
    const int n_labels;


    KNN(Point_Set& _ps) :
        ps(_ps), image_size(_ps.image_size), n_labels(_ps.n_labels) { }



    int get(const image_type& q_image, int k = 1) {
        auto nbh = compute_impl_sort(q_image, k);
        return guess_label(nbh);
    }

    int get(image_it q_beg, int  k = 1) {
        return get(image_type(q_beg, q_beg + 784));
    }

    /** Given the k-nearest neighbors, guess the label. */
    using neighborhood = std::vector<std::pair<size_t, double>>;
    int guess_label(neighborhood nbh) {
        auto counter = empty_counter();

        for (auto [ndx, dist] : nbh) {
            int label = ps.label(ndx);
            ++counter[label].second;
        }

        // {label_ndx, label_count}... Most frequent label nearby
        auto best_label = std::max_element(counter.begin(),
                                           counter.end(),
                                           [](const auto& lc_a, const auto& lc_b) {
                                               return lc_a.second < lc_b.second;
                                           });
        return best_label->first;
    }

/**
 * Given a Point_Set and a query image, compute the most likely
 * label for that image by the knn algorithm.
 *
 * Return pairs {distance, index} for debugging
 */
        std::vector<std::pair<size_t, double>> compute(image_type::iterator q_beg, size_t k = 1) {
            // k = 1 to begin with
            auto ps_iterator = ps.beg(0);
            auto ps_end = ps.end(ps.size());
            auto q_iterator = q_beg;
            size_t ndx = 0, best_ndx = 0;

            double dist = distance(ps_iterator, ps_iterator + image_size, q_iterator);
            double best_dist = dist;

            // Naive impl, check every member one after the other.
            while (ps_iterator != ps_end) {
                ++ndx;
                ps_iterator = ps.beg(ndx);
                q_iterator = q_beg;

                dist = distance(ps_iterator, ps_iterator + image_size, q_iterator);
                if (dist < best_dist)
                    best_dist = dist; best_ndx = ndx;
            }

            return std::vector<std::pair<size_t, double>> { std::pair{ best_ndx, best_dist } };
        }

        std::vector<std::pair<size_t, double>> compute_impl_sort(const image_type& q_image, size_t k) {
            std::vector<std::pair<int, double>> dists;
            dists.reserve(ps.size());
            image_it q_beg = q_image.begin();
            image_it q_it = q_beg;

            for (int i=0; i<ps.size(); ++i) {
                q_it = q_beg;
                dists.push_back(std::pair{ i, distance(ps.beg(i), ps.end(i), q_it) });
            }

            std::partial_sort(dists.begin(), dists.begin() + k, dists.end(), [](const auto& a, const auto& b) { return a.second < b.second; });
            std::vector<std::pair<size_t, double>> ret;
            std::copy(dists.begin(), dists.begin() + k, std::back_inserter(ret));
            return ret;
        }
/**
 * Compute the distance between an external image and one contained in a point set.
 * The one in the point set is given in terms of its begin and end iterators.
 */
        double distance(auto pt_beg, auto pt_end, auto q_beg) {
            std::vector<double> tmp_res{};
            std::transform(pt_beg, pt_end, q_beg, back_inserter(tmp_res), [](const uint8_t a, const uint8_t b) {
                return static_cast<double>((int(a) - int(b))) * static_cast<double>((int(a) - int(b))) / 16384.0;
            });
            return std::accumulate(tmp_res.begin(), tmp_res.end(), 0);
            // return std::transform_reduce(pt_beg, pt_end, q_beg, 0,
            //              [](const auto a1, const auto a2) { return a1 + a2; },          // reduce fcn
            //              [](const auto a, const auto b) { return (a - b) * (a - b); }); // transform
        }

        double distance(const image_type& image_a, const image_type& image_b) {
            return distance(image_a.begin(), image_a.end(), image_b.begin());
        }

private:
    Point_Set& ps;

    std::vector<std::pair<int, size_t>>
    empty_counter() {
        std::vector<std::pair<int, size_t>> ret(n_labels);
        for (int i=0; i<n_labels; ++i) {
            ret[i].first = i; ret[i].second = 0;
        }
        return ret;
    }


};

} // namespace knn



#endif // KNN_H_
