#include "simple_algo.h"
#include "../experiments/dataManager.h"
#include "util.h"

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

void RandomClustering::perform(const ClusteringInput &input,
                               ClusteringOutput &output) {
  int inputSize = input.data.size();

  auto idx = util::sample_with_repl(input.data.size(), input.n_centers);
  for (int i : idx) {
    output.centers.push_back(input.data[i]);
  }
  output.outliers_indices =
      util::sample_with_repl(input.data.size(), input.n_outliers);
  output.make_sure_labels_weights_outliers(input);
}



void RandClustering::perform(const ClusteringInput &input,
			     ClusteringOutput &output) {

  auto&& idx = util::sample_without_repl(input.data.size(), input.n_centers);
  for (const int i : idx) {
    output.centers.push_back(input.data[i]);
  }
  output.centers_indices = idx;
  auto&& ws = util::compute_weights(input.data, output.centers);
  output.weights = arma::vec(ws);
}



void ArmaKmeansClustering::perform(const ClusteringInput &input,
                                   ClusteringOutput &output) {
  arma::mat centers;
  arma::mat data(input.data.size(), input.data.at(0).n_elem);
  util::convert_to_mat(data, input.data);
  if (verbose) data.print("data:");
  bool status = arma::kmeans(centers, data.t(), input.n_centers,
                             arma::random_spread, n_iter, verbose);

  if (status == false) {
    std::cout << "clustering failed" << std::endl;
  }
  if (verbose) centers.print("centers:");
  util::convert_from_mat(output.centers, centers.t());

  output.make_sure_labels_weights_outliers(input);
}

void IdentityClustering::perform(const ClusteringInput &input,
                                 ClusteringOutput &output) {
  output.centers = input.data;
  output.centers_indices.clear();
  output.weights.resize(input.getNrows());
  output.weights.fill(1);
  for (int i = 0; i < input.getNrows(); i++) {
    output.centers_indices.push_back(i);
    output.labels.push_back(i);
  }
}
/*
void KmeansmmClustering::perform(
    const ClusteringInput &input,
    ClusteringOutput &output) {//TODO: still working!

  arma::mat centers;
  arma::mat data(input.data.size(),input.data.at(0).n_elem);
  util::convert_to_mat(data,input.data);
  data.print("data:");
  std::cout<<input.n_centers<<" "<<n_iter<<std::endl;
  bool status = arma::kmeans(centers, data.t(), input.n_centers,
      arma::random_subset, n_iter, true);

  if(status == false)
  {
    std::cout << "clustering failed" << std::endl;
  }

  centers.print("centers:");
  util::convert_from_mat(output.centers,centers.t());
}

*/
