#include "console_view.h"

#include <chrono>
#include <vector>
#include <iomanip>
#include <iostream>
#include <cstdint>
#include <thread>
#include <random>

#include "mnist.h"
#include "point.h"
#include "knn.h"


using namespace std::chrono_literals;

using image_t = std::vector<uint8_t>;
using image_it = image_t::const_iterator;
const size_t image_size = 784;
const int n_labels = 10;


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




/** Simply view the loaded images one by one. */
void view_training_images(const std::vector<uint8_t>& images) {
    for ( int i=0; i<5000; ++i ) {
        display::Single_viewer view(images.begin() + i * 784, images.begin() + (i + 1) * 784);
        std::this_thread::sleep_for(400ms);
    }
}

/**
 * The desired state is that Point_Set exposes what we need without hassle.
 */
void view_from_ps_images(const Point_Set& points) {
     for ( int i=0; i<5000; ++i ) {
         display::Single_viewer(points[i].begin(), points[i].end());
         std::this_thread::sleep_for(400ms);
     }
}

/** Test to see if the same behavior works in the case where we read twice to split the data. */
void display_split_dataset(image_t images_tr, image_t images_tst) {
    for (int i=0; i<10; ++i) {
        display::Single_viewer(images_tr.begin() + i * 784, images_tr.begin() + (i+1) * 784);
    }

    for (int i=0; i<10; ++i) {
        display::Single_viewer(images_tst.begin() + i * 784, images_tst.begin() + (i+1) * 784);
    }
}


/** No loss after moving the images into the point set */
void move_to_ps(image_t labels_tr, image_t images_tr) {
    for (int i=0; i<10; ++i) {
        display::Single_viewer(images_tr.begin() + i * 784, images_tr.begin() + (i+1) * 784);
    }
    Point_Set points(std::move(labels_tr), std::move(images_tr), image_size, n_labels);

    for (int i=0; i<10; ++i) {
        display::Single_viewer(points[i], image_t());
    }
}

/** Check that we can compute distances and retrieve the points from the accessors of Point_set */
void ok_with_pointset_accessors(image_t& images_tst, Point_Set& points, knn::KNN& knn) {
    image_t q_image(images_tst.begin(), images_tst.begin() + 784);
    display::View_iterator view_it(q_image);

    for (int i=0; i<100; ++i) {

        float distance = knn.distance(points[i], q_image);
        std::cout << " Distance : " << std::setprecision(2) << distance << "  Label : " << int(points.label(i)) << std::endl;
        view_it(points[i]);
    }
}



/**
  * Finally it works pretty fast for this small amount of test cases:
  */
void knn_small_numbers(image_t& q_image, Point_Set& points, knn::KNN& knn) {
    auto five_best = knn.compute_impl_sort(q_image, 5);
    display::View_iterator view_it(q_image);

    PRINT("The five nearest neighbors of the query image are");

    for (auto [n, dist] : five_best) {
        view_it(points[n]);
        std::cout << "DISTANCE : " << std::setprecision(2) << dist << std::endl;
    }
}



int main()
{
    using image_t = std::vector<uint8_t>;
    using label_t = uint8_t;

    //auto [labels, images] = load_csv("data/mnist/train.csv", 5000);

    const int n_training = 1000;
    const int n_test = 3000;

    load_csv_train_test load_csv_test;
    auto [labels_tr, images_tr, labels_tst, images_tst] = load_csv_test("../data/mnist/train.csv", n_training, n_test);

    /**
     * Because Point_Set will gain ownership of the vectors, we either need to work with
     * Poin_Set here, or make copies before creating it.
     */
    Point_Set points(std::move(labels_tr), std::move(images_tr), image_size, n_labels);
    knn::KNN knn(points);

    double avg = 0;
    image_t q_image;

    for (int i=0; i<n_test; ++i) {
        q_image = image_t(images_tst.begin() + i * 784, images_tst.begin() + (i+1) * 784);

        auto five_best = knn.compute_impl_sort(q_image, 5);

        display::View_iterator view_it(q_image);

        PRINT("The five nearest neighbors of the query image are");

        for (auto [n, dist] : five_best) {
            view_it(points[n]);
            std::cout << "DISTANCE : " << std::setprecision(2) << dist << std::endl;
        }

        int correct_label = *(labels_tst.begin() + i);
        int guessed_label = knn.guess_label(five_best);

        bool correct = correct_label == guessed_label;

        PRINT("GUESSED ", guessed_label);

        avg += (correct - avg) / (i + 1);

        if (i % 100 == 0) {
            PRINT("After ", i, "tries, Average result is ", avg);
        }
    }


    PRINT("Average result : ", avg);

    return EXIT_SUCCESS;
}
