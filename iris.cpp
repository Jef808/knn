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

struct Data {
    void input(const std::string& filename, std::size_t training_size);
    std::vector<Entity> training;
    std::vector<Entity> testing;
};

void Data::input(const std::string& fn, std::size_t training_size) {
    std::array<Entity, N_data> data {};
    std::ifstream _if;
    _if.open(fn);
    if (!_if) {
        std::cerr << "Unable to open file " << fn << std::endl;
    }
    for (int i=0; i<N_data; ++i) {
        _if >> data[i];
    }

    std::random_shuffle(data.begin(), data.end());  // Data could be already partitionned into labels!

    for (auto [i, it] = std::pair{0, data.begin()}; it != data.end(); ++i, ++it) {
        it->ndx = i;
    }

    testing = std::vector<Entity>(data.begin(), data.begin() + training_size);
    training = std::vector<Entity>(data.begin() + training_size, data.end());
}


int main()
{
    Data data;
    data.input(filename, N_training_data);

    std::vector<Label> results;

    auto knn = KNN();
    knn.set_training_data(data.training.begin(), data.training.end());

    for (const auto& entity : data.testing) {
        results.push_back(knn.most_likely_label(entity));
    }

    int n_correct = 0;
    //int i = 0;
    for (auto ndx_zip_it = std::make_tuple(0, results.begin(), data.testing.begin());
         get<1>(ndx_zip_it) != results.end();
         ++get<0>(ndx_zip_it), ++get<1>(ndx_zip_it), ++get<2>(ndx_zip_it))
    {
        auto [ndx, guessed_label_it, known_entity_it] = ndx_zip_it;

        Label correct_type = known_entity_it->label;

        std::cout << ndx << "'th guess was " << *guessed_label_it
                  << " while answer was " << correct_type
                  << std::endl;

        n_correct += (*guessed_label_it == correct_type);
    }

    double percent_correct = 100.0 * n_correct / data.testing.size();

    printf("Percentage of correct guesses: %.2f", percent_correct);

    return EXIT_SUCCESS;
}
