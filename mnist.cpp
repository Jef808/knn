#include "mnist.h"

#include <array>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <sstream>
#include <utility>
#include <vector>

#include "knn.h"

const std::string filename = std::string("../data/mnist/train.csv");
constexpr auto N_data = 4200;
constexpr auto N_training_data = 3500;
constexpr auto Nbh_size = 5;

using mnist::Entity;
using Label = Entity::Label;
using KNN = knn::KNN<mnist::Entity, Nbh_size, N_data>;

struct Data {
    void input(const std::string& filename, int training_size);
    std::vector<Entity> data;
    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }
    auto begin() { return data.begin(); }
    auto end() { return data.end(); }
};

void Data::input(const std::string& fn, int training_size) {
    data = std::vector<Entity>(N_data);

    std::ifstream _if;
    _if.open(fn);
    if (!_if) {
        std::cerr << "Unable to open file " << fn << std::endl;
    }

    std::string buf;
    std::getline(_if, buf);

    //TODO: Count the different labels to make sure data isn't skewed

    int i_buf = 0;
    for (int i=0; i<N_data; ++i) {
        std::getline(_if, buf);
        std::istringstream ss{buf};
        ss >> i_buf;
        data[i].label = Label(i_buf);
        //std::getline(ss, buf, ',');
        //data[i].label = Label(stoi(buf));
        for (int j=0; j<784; ++j) {
            //std::getline(ss, buf, ',');

            ss >> i_buf;
            data[i][j] = static_cast<uint8_t>(i_buf);// = static_cast<uint8_t>(label_buf);
            // = static_cast<uint8_t>(std::stoi(buf));
        }
    }

    std::random_shuffle(data.begin(), data.end());  // Data could be already partitionned into labels!

    for (auto [i, it] = std::pair{0, data.begin()}; it != data.end(); ++i, ++it) {
        it->ndx = i;
    }
}

auto const log_file = std::string("mnist.log");
void write_log(const std::string& lf, const std::vector<Label>& results, const Data& data) {
    std::ofstream _of;
    _of.open(lf);
    if (!_of) {
        std::cerr << "Error: could not open file " << lf << std::endl;
    }
    int n_correct = 0;
    int ndx = 0;
    for (auto ndx_zip_it = std::make_tuple(0, results.begin(), data.begin());
         get<1>(ndx_zip_it) != results.end();
         ++get<0>(ndx_zip_it), ++get<1>(ndx_zip_it), ++get<2>(ndx_zip_it))
    {
        auto [ndx, guessed_label_it, known_entity_it] = ndx_zip_it;

        Label correct_type = known_entity_it->label;

        _of << ndx << "'th guess = " << *guessed_label_it
            << " correct = " << correct_type << '\n';

        n_correct += (*guessed_label_it == correct_type);
    }
    double percentage = 100.0 * n_correct / results.size();
    printf("Success rate after all %d tests: %.2f", N_data - N_training_data, percentage);
}

int main()
{
    Data data {};
    data.input(filename, N_training_data);

    auto knn = KNN();
    knn.set_training_data(data.begin() + N_data - N_training_data, data.end());
    data.data.resize(N_data - N_training_data);

    std::vector<Label> results;
    results.reserve(data.data.size());

    int n_correct = 0;
    double percent_correct = 100.0;
    int count = 0;
    for (const auto& entity : data) {
        results.push_back(knn.most_likely_label(entity));
        n_correct += (results.back() == data.data[count].label);
        ++count;
        if (count % 100 == 0) {
            printf("After %d guesses, got %.2f percent right", count, static_cast<double>(n_correct)/count);
            std::cout.flush();
        }
    }

    //percent_correct = 100.0 * n_correct / data.data.size();

    //printf("Success rate after all %d tests: %.2f", N_data - N_training_data, percent_correct);
    write_log(log_file, results, data);


    return EXIT_SUCCESS;
}
    //int i = 0;
    // for (auto ndx_zip_it = std::make_tuple(0, results.begin(), data.testing.begin());
    //      get<1>(ndx_zip_it) != results.end();
    //      ++get<0>(ndx_zip_it), ++get<1>(ndx_zip_it), ++get<2>(ndx_zip_it))
    // {
    //     auto [ndx, guessed_label_it, known_entity_it] = ndx_zip_it;

    //     Label correct_type = known_entity_it->label;

    //     // std::cout << ndx << "'th guess was " << *guessed_label_it
    //     //           << " while answer was " << correct_type
    //     //           << std::endl;

    //     n_correct += (*guessed_label_it == correct_type);
    // }
