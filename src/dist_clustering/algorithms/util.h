#ifndef __UTIL_H__
#define __UTIL_H__

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

#include <set>
#include <vector>
#include <exception>

namespace util {

std::vector<int> add_outliers(std::vector<arma::vec>& U, int n_outliers,
                              double upper_bound);
std::vector<double> arma_vec_to_vector(const arma::vec& vec);

std::vector<int> weighted_sample_with_repl(const std::vector<double>& weights, const int m);
std::vector<int> weighted_sample_without_repl(const std::vector<double>& weights, const int m);
 
std::vector<double> compute_weights(const std::vector<arma::vec>& U,
				    const std::vector<arma::vec>& S);

double dist(const arma::vec& a, const arma::vec& b);
double dist2(const arma::vec& a, const arma::vec& b);

double sum(const std::vector<double>& nums);
double sum2(const std::vector<double>& nums);

// Sample m numbers from 0 .. n-1 without replacement,
// keep all results in a vector.
std::vector<int> sample_without_repl(const int n, const int m);

// Sample m numbers from 0 .. n-1 with replacement,
// keep all results in a vector.
std::vector<int> sample_with_repl(const int n, const int m);

void convert_to_mat(arma::mat& destination,
                    const std::vector<arma::vec>& source);
void convert_from_mat(std::vector<arma::vec>& destination,
                      const arma::mat& source);

bool equal_mat(const std::vector<arma::vec>& a, const arma::mat& b);

void copy_to_array(double* destination, const std::vector<arma::vec>& source);
void copy_vec_to_array(double* destination, const arma::vec& source);

void compute_nearest(const std::vector<arma::vec>& U, const std::vector<int>& S,
                     std::vector<int>& res_idx, std::vector<double>& min_dist);

void compute_nearest(const std::vector<arma::vec>& U,
                     const std::vector<arma::vec>& S, std::vector<int>& res_idx,
                     std::vector<double>& min_dist);

template <typename T>
std::vector<T> subset(const std::vector<T>& U,
                      const std::vector<int>& indices) {
  std::set<int> idx(indices.begin(), indices.end());
  std::vector<T> res;
  for (int i : idx) {
    if (i < U.size() && i >= 0) {
      res.push_back(U[i]);
    }
  }
  return res;
}

template <typename T>
std::vector<T> union_sets(const std::vector<T>& A, const std::vector<T>& B) {
  std::set<T> U(A.begin(), A.end());
  for (const auto& b : B) {
    U.insert(b);
  }
  std::vector<T> ans(U.begin(), U.end());
  return ans;
}

// remove the subset indexed by idx from U.
// preserve the order of the remaining set
template <typename T>
void remove_subset(std::vector<T>& U, const std::vector<int>& idx) {
  std::vector<T> tmp;
  std::set<int> set_idx(idx.begin(), idx.end());
  for (int i = 0; i < U.size(); ++i) {
    if (set_idx.count(i) == 0) {
      tmp.push_back(U[i]);
    }
  }
  U = tmp;
}

// return U\idx_to_remove
template <typename T>
std::vector<T> backslash(const std::vector<T>& U,
                         const std::vector<int>& idx_to_remove) {
  auto copy_U = U;
  remove_subset(copy_U, idx_to_remove);
  return copy_U;
}

// return a vector which represents the indices of the smallest n numbers of
// values
// The index of the smallest number will be returned first ...
std::vector<int> indices_of_lower(const std::vector<double>& values,
                                  const int n);

// return a vector which represents the indices of the largest n numbers of
// values
// The index of the largest number will be returned first ...
std::vector<int> indices_of_upper(const std::vector<double>& values,
                                  const int n);

enum CostType { k_mean, k_median };

// This function will automatically compute the
// labels. Note call this function will be slow.
double cost(const std::vector<arma::vec>& U,
            const std::vector<arma::vec>& centers,
            CostType type = CostType::k_median);

// this function does not check whether the label is valid
double cost(const std::vector<arma::vec>& U,
            const std::vector<arma::vec>& centers,
            const std::vector<int>& labels, CostType type = CostType::k_median);

// this function == cost(U\outliser, centers, labels, type)
double cost_outliers(const std::vector<arma::vec>& U,
                     const std::vector<arma::vec>& centers,
                     const std::vector<int>& outliers,
                     const std::vector<int>& labels,
                     CostType type = CostType::k_median);

void find_indices(std::vector<int>& outputOutliers,
                  const std::vector<arma::vec>& baseData,
                  const std::vector<arma::vec>& secondData,
                  const std::vector<int>& secondOutliers_indices);

void print_vector(const std::vector<int>& vector);
void arrToStream(std::ostream& stream, double* arr, int length);

void randomList(std::vector<int>& output, const arma::vec& weights,
                const int k);
void average_centers(std::vector<arma::vec>& newCenters,
                     const std::vector<arma::vec>& data,
                     const arma::vec weights, const std::vector<int>& labels,
                     const std::vector<int>& outliersVec, const int n_centers,
                     const std::vector<arma::vec>& oldCenters);

;

// return the indices of outliers,
// also set the weights for outliers
std::vector<int> weighted_outliers(const std::vector<double>& dists,
                                   const arma::vec& weights,
                                   const int n_outliers,
                                   std::vector<double>& Ow);

void weights_of_upper(arma::vec& newWeights, const std::vector<double>& values,
                      const arma::vec& weights, const double bag);

std::string space2underscore(std::string text);

double jaccard(const std::vector<int>& A, const std::vector<int>& B);
// return |A\cap B| / |A|
double recall(const std::vector<int>& A, const std::vector<int>& B);
double precision(const std::vector<int>& A, const std::vector<int>& B);

template <typename T>
double avg(const std::vector<T>& vec) {
  if (vec.empty()) {
    throw std::runtime_error("avg: empty vec");
  }
  double sm = 0;
  for (const auto& v : vec) {
    sm += v;
  }
  return sm / (vec.size() + 0.);
}

//TODO: there is a linear time implementation, do it later
template <typename T>
T median(const std::vector<T>& vec) {
  if (vec.empty()) {
    throw std::runtime_error("median: empty vec");
  }

  auto vv = vec;
  
  std::sort(vv.begin(), vv.end());
  return vv[vv.size() / 2];
}
  
};
#endif


















