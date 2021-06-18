/// knn classification for the iris dataset
#include <array>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <numeric>
#include <random>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "knn.h"
#include "iris.h"


const std::string filename = std::string("../data/iris.data");
constexpr auto N_data = 150;
constexpr auto N_training_data = 25;
constexpr auto Nbh_size = 5;

using iris::Entity;
using Label = Entity::Label;
using KNN = knn::KNN<iris::Entity, Nbh_size, N_data>;

auto get_input_data(const std::string& fn) {
    std::array<Entity, N_data> data {};
    std::ifstream _if;
    _if.open(fn);
    if (!_if) {
        std::cerr << "Unable to open file " << fn << std::endl;
        return data;
    }
    for (int i=0; i<N_data; ++i) {
        _if >> data[i];
    }
    return data;
}

auto split_at(auto _beg, auto _end, const auto split_ndx) {
    return std::make_pair(std::vector<Entity>(_beg, _beg + split_ndx),
                          std::vector<Entity>(_beg + split_ndx, _end));
}

int main()
{
    auto data = get_input_data(filename);
    std::random_shuffle(data.begin(), data.end());  // Data could be already partitionned into labels!
    int ndx = 0;
    for (auto it=data.begin(); it!=data.end(); ++it, ++ndx) {
        it->ndx = ndx;
    }

    auto [training_data, testing_data] = [n = N_training_data](auto _beg, auto _end) {
        return std::make_pair(std::vector<Entity>(_beg, _beg + n),
                              std::vector<Entity>(_beg + n, _end));
    }(data.begin(), data.end());

    std::vector<Label> results;

    auto knn = KNN();
    knn.set_training_data(training_data.begin(), training_data.end());

    for (const auto& entity : testing_data) {
        results.push_back(knn.most_likely_label(entity));
    }

    int n_correct = 0;
    int i = 0;
    for (auto zip_it = std::make_pair(results.begin(), testing_data.begin());
         zip_it.first != results.end();
         ++zip_it.first, ++zip_it.second, ++i)
    {
        // auto [result_it, testing_it] = zip_it;

        // Label guessed_type = *result_it;
        // Label correct_type = *testing_it;

        Label guessed_type = *zip_it.first;
        Label correct_type = zip_it.second->label;

        std::cout << i << "'th guess was " << guessed_type
                  << " while answer was " << correct_type
                  << std::endl;

        n_correct += (guessed_type == correct_type);
    }

    double percent_correct = 100.0 * n_correct / testing_data.size();

    printf("Percentage of correct guesses: %.2f", percent_correct);

    return EXIT_SUCCESS;
}
