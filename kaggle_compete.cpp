#include <vector>
#include <fstream>
#include <iostream>

#include "mnist.h"
#include "point.h"
#include "knn.h"
#include "console_view.h"

void kaggle_compete() {
    using image_t = std::vector<uint8_t>;

    const std::string fn_train_in = "data/mnist/train.csv";
    const std::string fn_test_in = "data/mnist/test.csv";
    const std::string fn_out = "data/mnist/kaggle_submission.csv";


    auto [labels, training] = load_csv(fn_train_in, 42000);
    auto testing = load_csv_no_labels(fn_test_in, 28000);

    Point_Set points(std::move(labels), std::move(training), 784, 10);
    knn::KNN knn(points);

    double avg = 0;
    image_t q_image;

    std::vector<int> results;

    for (int i=0; i<28000; ++i) {
        q_image = image_t(testing.begin() + i * 784, testing.begin() + (i+1) * 784);

        int guessed_label = knn.get(q_image, 3);
        results.push_back(guessed_label);
    }

    std::ofstream ofs(fn_out);
    if (!ofs) {
        PRINT("Cannot open output file!");
        return;
    }

    for (int i=0; i<28000; ++i) {
        ofs << i << ',' << results[i] << '\n';
    }
}


int main()
{
    auto tests = load_csv_no_labels("data/mnist/test.csv");

    auto it = tests.begin();

    char c;

    while (it != tests.end()) {
        display::Single_viewer(it, it + 784);
        it += 784;
        std::cin >> c;
    }
}
