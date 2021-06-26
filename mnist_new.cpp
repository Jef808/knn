#include "mnist_new.h"

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
    auto [labels0, images0] = load_csv_bare()(fn, n_images);
    auto end0 = now();
    time(start0, end0);
    labels0.clear(); images0.clear();

    PRINT("fancy method");
    auto start = now();
    auto ret = load_csv()(fn, n_images);
    auto end = now();
    time(start, end);



}
