#include "algo_util.h"

void ClusteringOutput::fillExtraInfo(const ClusteringInput& input) {
  make_sure_labels_weights_outliers(input);
}
void ClusteringOutput::make_sure_labels_weights_outliers(
    const ClusteringInput& input) {
  std::vector<double> min_dist;
  if (labels.size() == 0 || outliers_indices.size() == 0) {
    util::compute_nearest(input.data, centers, labels, min_dist);
    if (outliers_indices.size() == 0)
      outliers_indices = util::indices_of_upper(min_dist, input.n_outliers);
  }
  if (weights.size() == 0) compute_weights();
}

void ClusteringOutput::compute_weights() {
  std::unordered_map<int, int> map_count;
  std::set<int> outlierSet(outliers_indices.begin(), outliers_indices.end());
  for (int i = 0; i < labels.size(); i++) {
    //      std::cout<<i<<" for "<<labels[i]<<"
    //      cont"<<outlierSet.count(i)<<std::endl;
    if (outlierSet.count(i) == 0) map_count[labels[i]]++;
  }
  weights.resize(getNcenters());
  for (int p = 0; p < getNcenters(); p++) {
    weights[p] = (map_count[p]);
  }
}

void addAlgo(std::vector<std::unique_ptr<ClusteringAlgo>>& algos,
             ClusteringAlgo* newAlgPtr) {
  algos.push_back(std::unique_ptr<ClusteringAlgo>(newAlgPtr));
}
