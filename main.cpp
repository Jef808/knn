// main
#include "console_view.h"
#include "knn.h"
#include "mnist.h"
#include "point.h"
#include <chrono>
#include <iomanip>
#include <vector>

const int n_train = 10000;
const int n_test = 3000;
const int n_nbh = 5;
const int n_labels = 10;
const int img_size = 784;

using namespace knn;

auto now()
{
    return std::chrono::steady_clock::now();
}

auto time(auto start, auto end)
{
    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}

int main()
{
    using image_t = std::vector<uint8_t>;
    using label_t = uint8_t;

    //auto [labels, images] = load_csv("data/mnist/train.csv", 5000);

    load_csv_train_test load_csv_test;
    auto [labels_tr, images_tr, labels_tst, images_tst] = load_csv_test("../data/mnist/train.csv", n_train, n_test);

    /**
     * Because Point_Set will gain ownership of the vectors, we either need to work with
     * Poin_Set here, or make copies before creating it.
     */
    Point_Set points(std::move(labels_tr), std::move(images_tr), img_size, n_labels);
    knn::KNN knn(points);

    double avg = 0;
    image_t q_image;

    auto start = now();

    for (int i = 0; i < n_test; ++i) {
        q_image = image_t(images_tst.begin() + i * 784, images_tst.begin() + (i + 1) * 784);

        auto five_best = knn.compute_impl_sort(q_image, 5);

        display::View_iterator view_it(q_image);

        PRINT("The five nearest neighbors of the query image are");

        for (auto [n, dist] : five_best) {
            view_it(points[n]);
            std::cout << "DISTANCE : " << std::setprecision(2) << dist << std::endl;
        }

        int correct_label = *(labels_tst.begin() + i);
        int guessed_label = knn.get(q_image, n_nbh);

        bool correct = correct_label == guessed_label;

        PRINT("GUESSED ", guessed_label);

        avg += (correct - avg) / (i + 1);
    }
    auto end = now();

    PRINT("Training set size: ", n_train, " Testing set size: ", n_test, " knn number: ", n_nbh);
    PRINT("Success rate: ", avg, " avg time per test: ", std::setprecision(2), time(start, end) / n_test, "s.");

    return EXIT_SUCCESS;
}
