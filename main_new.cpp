// main
#include "point.h"
#include "mnist_new.h"
#include "knn.h"
#include "console_view.h"

const int n_train = 300;
const int n_test  = 100;


using namespace knn;

int main()
{
    auto [lab_trn, img_trn, lab_tst, img_tst] = load_csv_train_test()("data/mnist/train.csv", n_train, n_test);

    PRINT("Loaded data");

    Point_Set points(std::move(lab_trn), std::move(img_trn));
    KNN knn;

    PRINT("Populated Point_Set");

    float avg = 0;
    std::array<bool, n_test> res;

    auto img_it = img_tst.begin(), img_end = img_tst.end();
    auto lab_it = lab_tst.begin();

    float ndx = 0;
    PRINT("About to start testing");
    while (img_it != img_end) {

        int correct_label = *lab_it;
        int guessed_label = knn.guess_label(points, img_it);

        PRINT("Test done, result is", (correct_label == guessed_label ? " RIGHT" : " WRONG"));
        // avg_old = #success/ndx
        // avg_new = (ndx * avg_old +[1 or 0]) / (ndx + 1)
        //
        // avg_new = avg_old (+[1 or 0] - avg_old)/(ndx+1)
        res[ndx] = guessed_label == correct_label;
        avg += (res[ndx] - avg) / (ndx+=1);

         ++lab_it;
    }

    PRINT("Average result : ", avg);



    return EXIT_SUCCESS;
}
