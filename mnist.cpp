#include "mnist.h"

#include <chrono>
#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <sstream>
#include <utility>
#include <type_traits>


std::ostream& operator<<(std::ostream& out, const std::vector<uint8_t>& vec) {
    for (auto v : vec) {
        std::cout << int(v) << ' ';
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const std::vector<int>& vec) {
    for (auto v : vec) {
        std::cout << v << ' ';
    }
    return out;
}


template < typename... Args >
std::ostream& OUT (std::ostream& out, Args&&... args) {
    return (out << ... << args);
}

auto now() {
    return std::chrono::steady_clock::now();
}

void time(auto start, auto end) {
    std::chrono::duration<double> elapsed = end - start;
    PRINT("elapsed time: ", elapsed.count(), "s\n");
}


int main()
{
    std::string fn =  "data/mnist/train.csv";
    int n_images = 12000;


    PRINT("document open");

    std::string buf;
    // first line

    PRINT("bare method");
    auto start0 = now();
    load_csv_bare loader{};
    auto [labels0, images0] = loader(fn, n_images);
    auto end0 = now();
    time(start0, end0);
    labels0.clear(); images0.clear();

    PRINT("fancy method");
    auto start = now();
    auto [labels, ret] = load_csv(fn, n_images);
    auto end = now();
    time(start, end);
    labels.clear(); ret.clear();

    PRINT("Splitting training and testing data");
    PRINT("-----------------------------------");
    // PRINT("bare method");
    // auto start0 = now();
    // auto [labels0, images0] = load_csv_bare(fn, n_images);
    // auto end0 = now();
    // time(start0, end0);
    // labels0.clear(); images0.clear();

    PRINT("fancy method");
    auto start1 = now();
    load_csv_train_test load_train;
    auto [labels_tr, images_tr, labels_tst, images_tst] = load_train(fn, n_images / 3, 2 * n_images / 3);
    auto end1 = now();
    time(start1, end1);



}
