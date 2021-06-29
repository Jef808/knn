#include <mlpack/core.hpp>
#include <mlpack/methods/neighbor_search/neighbor_search.hpp>

using namespace mlpack;

const std::string data_DIR = "/home/jfa/code/projects/knn/data/mnist";

int main()
{
    arma::mat X_train;
    arma::mat Y_train;
    arma::mat X_test;
    
    data::Load("data/mnist/mlpack/train_data.csv", X_train, true);
    data::Load("data/mnist/mlpack/train_labels.csv", Y_train, true);
    data::Load("data/mnist/mlpack/test.csv", X_test, true);

    neighbor::NeighborSearch<neighbor::NearestNeighborSort,
                             metric::SquaredEuclideanDistance> nn(X_train);

    arma::Mat<size_t> neighbors;
    arma::mat distances;
    
    //arma::mat cov = data * trans(data) / data.n_cols;
    //data::Save("data/mnist/cov.csv", cov, true);

    arma::mat test_data;
    data::Load("data/mnist/test.csv", test_data, true);

    nn.Search(X_test, 3, neighbors, distances);

    neighbors.transform( [&](auto val) { return Y_train[val]; } );

    data::Save("data/mnist/results.csv", neighbors, true);
}













