#include "mnist.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

std::ostream& operator<<(std::ostream& out, const std::vector<uint8_t>& vec)
{
    for (auto v : vec) {
        std::cout << int(v) << ' ';
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const std::vector<int>& vec)
{
    for (auto v : vec) {
        std::cout << v << ' ';
    }
    return out;
}

template <typename... Args>
std::ostream& OUT(std::ostream& out, Args&&... args)
{
    return (out << ... << args);
}

auto now()
{
    return std::chrono::steady_clock::now();
}

void time(auto start, auto end)
{
    std::chrono::duration<double> elapsed = end - start;
    PRINT("elapsed time: ", elapsed.count(), "s\n");
}

int main()
{
    std::string fn = "data/mnist/train.csv";

    PRINT("document open");

    std::string buf;
    // first line

    PRINT("bare method");
    auto start0 = now();
    load_csv_bare loader {};
    auto [labels0, images0] = loader(fn, 42000);
    auto end0 = now();
    time(start0, end0);
    labels0.clear();
    images0.clear();

    PRINT("fancy method");
    auto start = now();
    auto [labels, ret] = load_csv(fn, 42000);
    auto end = now();
    time(start, end);
    labels.clear();
    ret.clear();

    PRINT("Splitting the labaled images for training");
    auto start1 = now();
    load_csv_train_test load_train;
    auto [labels_tr, images_tr, labels_tst, images_tst] = load_train(fn, 42000 / 3, 2 * 42000 / 3);
    auto end1 = now();
    time(start1, end1);

    PRINT("All labeled images");
    auto start3 = now();
    auto [labels3, images3] = load_csv(fn, 42000);
    auto end3 = now();
    time(start3, end3);

    PRINT("File without labels");
    auto start2 = now();
    auto images = load_csv_no_labels("data/mnist/test.csv", 28000);
    auto end2 = now();
    time(start2, end2);
}
