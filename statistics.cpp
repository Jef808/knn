#include <array>
#include <algorithm>
#include <numeric>
#include <vector>
#include <cmath>
#include <iomanip>
#include <functional>
#include <utility>
#include <tuple>
#include <iterator>
#include <iostream>

#include "mnist.h"
#include "console_view.h"


namespace Stats {

/**
 *
 *
 */
struct stats_buffer {
    int index{0};
    float mean{0.0F};
    float square_mean{0.0F};

    void inc_mean(float val) {
        mean = mean * (1.0F - 1.0F/float(index + 1)) + val/float(index + 1);
    }
    void inc_square_mean(float val) {
        square_mean = square_mean * (1.0F - 1.0F/float(index + 1)) + val * val / float(index + 1);
    }
    void inc_stats(float val) {
        inc_mean(val); inc_square_mean(val); ++index;
    }
    auto operator()() {
        float variance = std::abs(square_mean - (mean * mean));
        return std::pair{ mean, variance };
    }
    void reset() {
        index = 0; mean = 0.0F; square_mean = 0.0F;
    }
};

/**
 *
 *
 */
class accumulate_stats :
    std::iterator<std::output_iterator_tag, float>
{
public:
    accumulate_stats(stats_buffer* sb) :
        m_buffer(sb) { }
    template < typename T >
    accumulate_stats& operator=(T a)
        { m_buffer->inc_stats(a); return *this; }
    accumulate_stats& operator*()
        { return *this; }
    accumulate_stats& operator++()
        { return *this; }
    accumulate_stats& operator++(int)
        { return *this; }
private:
    stats_buffer* m_buffer;
};


/**
 *
 *
 */
class FeatureStatistics {
public:
    using image_t = std::vector<uint8_t>;

    FeatureStatistics(int _n_images, const image_t& _images) :
        r_images(_images), n_images(_n_images) { }

    void init()
        { get_variances(); }

    void display_variances(std::ostream& out = std::cout)
        //{ Outil outil(m_variances); outil.display(out); }
        { Outil outil(m_variances); outil.display_grayscale(out); } // outil.display(out); }

private:
    stats_buffer m_buf;
    std::array<float, 784> m_variances;
    const image_t& r_images;
    int n_images;

    std::pair<float, float> get_pixel_stats(int ndx) {
        m_buf.reset();
        uint8_t val = r_images[ndx];
        for (int i=1; i<n_images; ++i) {
            m_buf.inc_stats(val);
            val = r_images[ndx += n_images];
        }
        return m_buf();
    }

    void get_variances() {
        for (int i=0; i<m_variances.size(); ++i) {
            auto [mean, variance] = get_pixel_stats(i);
            m_variances[i] = variance;
        }
    }

    struct Outil {
        std::vector<int> ccode
            { 65, 66, 67, 68, 69, 71, 72, 73, 74, 75 };
        struct {
            int operator[](int i) {
                if (i<1)
                    return 232;
                if (i>22)
                    return 255;
                return 232 + i;
            }
            int size() { return 23; }
        } gscode;
            //{ 52, 54, 56, 62, 68, 74, 79, 82, 148, 184 };
        const std::array<float, 784>& r_variances;

        Outil(const std::array<float, 784>& variances) :
            r_variances(variances)
        { }

        void display_grayscale(std::ostream& out) {
            auto index_map = make_partitions(gscode.size());

            for (int i=0; i<28; ++i) {
                for (int j=0; j<28; ++j) {
                    int color = gscode[index_map[j + i * 28].second];
                    out << "\e[38;5;" << color << "m"
                        << "\u25A0";
                }
                out << '\n';
            }
            out << "\e[m" << std::endl;
        }

        void display(std::ostream& out) {
            auto index_map = make_partitions(ccode.size());

            for (int i=0; i<28; ++i) {
                for (int j=0; j<28; ++j) {
                    int color = ccode[index_map[j + i * 28].second];
                    out << "\e[38;5;" << color << "m"
                        << "\u25A0";
                }
                out << '\n';
            }
            out << "\e[0m" << std::endl;
        }

        /**
         * Return a 'partitition index' which indicates which bucket the ith pixel goes in
         * according to its variance ( { index, bucket } ).
        */
         std::array<std::pair<int, int>, 784>
         make_partitions(int n_buckets) {
             if (n_buckets < 1)
                 n_buckets = 1;

             const int bucket_size = std::max(1, int(r_variances.size() / n_buckets));
             std::array<std::pair<int, int>, 784> index{};

             for (int i=0; i<784; ++i) {
                 index[i].first = i;
             }
             auto cmp = [&](const auto& a, const auto& b){
                 return r_variances[a.first] < r_variances[b.first];
             };
             std::sort(index.begin(), index.end(), cmp);

             int cnt = 1;
             for (auto it = index.begin(); it != index.end(); ++it, ++cnt) {
                 it->second = (cnt / bucket_size) - 1;
             }

             auto cmp_first = [](const auto& a, const auto& b) {
                 return a.first < b.first;
             };
             std::sort(index.begin(), index.end(), cmp_first);

             return index;
        }
    };

};



template < typename Container >
stats_buffer compute_stats(const Container& cont) {
    stats_buffer sb;
    std::copy(cont.begin(), cont.end(), accumulate_stats(&sb));
    return sb;
}

template < typename InputIterator >
stats_buffer compute_stats(InputIterator beg, InputIterator end) {
    stats_buffer sb;
    std::copy(beg, end, accumulate_stats(&sb));
    return sb;
}


} // namespace Stats


const int image_size = 784;
const int n_images = 42000;

int main()
{
    auto [labels, images] = load_csv("../data/mnist/train.csv", n_images);

    Stats::FeatureStatistics stats(784, images);
    stats.init();

    stats.display_variances(std::cout);

    return EXIT_SUCCESS;





    // std::cout << std::setprecision(3);
    // for (int i=0; i<28; ++i) {
    //     for (int j=0; j<28; ++j) {
    //         std::cout << variances[j + 28 * i] << ' ';
    //     }
    //     std::cout << '\n';
    // }
    // std::cout << std::endl;


    return EXIT_SUCCESS;
    
    // std::cout << "\e[0;32mBEGINNING OF HISTOGRAM....\e[0m" << std::endl;
    // for (auto it = svb.means_square_means.begin(); it != svb.means_square_means.end(); ++it) {
    //     if (it->second != 0) {
    //         int variance_i = static_cast<int>(48 * static_cast<int>(it->second));
    //         if (variance_i != 0) { variance_i = log(variance_i); }
    //         std::cout << "\e[0;32m";
    //         for (int i=0; i<variance_i; ++i) {
    //             std::cout << '|';
    //         }
    //         std::cout << "\e[0m";
    //     }
    //     std::cout << '\n';
    // }
    // std::cout << "\e[0;32mEND OF HISTOGRAM....\e[0m" << std::endl;

}
