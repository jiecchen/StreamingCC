#include "util.h"

#include <cmath>
#include <iterator>
#include <random>
#include <set>
#include <unordered_map>
#include <vector>
#include <chrono>

#include <ctype.h>

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

#include "../experiments/erfanUtil.h"

using namespace std;

namespace util {


vector<double> compute_weights(const vector<arma::vec>& U,
			       const vector<arma::vec>& S) {
  vector<double> dists;
  vector<int> labels;
  util::compute_nearest(U, S, labels, dists);
  map<int, int> ws;
  for (const int i : labels) {
    ws[i]++;
  }
  vector<double> res(S.size());
  for (const auto& p : ws) {
    res[p.first] = p.second;
  }
  return res;
}


void add_noise(arma::vec& v, double upper_bound) {
  arma::arma_rng::set_seed_random();
  arma::vec vv(v.size());
  vv.randu();
  v = v + upper_bound * (vv - 0.5);
}

vector<int> add_outliers(vector<arma::vec>& U, int n_outliers,
                         double upper_bound) {
  auto&& idx = sample_without_repl(U.size(), n_outliers);
  for (int i : idx) {
    add_noise(U[i], upper_bound);
  }
  return idx;
}

// return the indices of outliers,
// also set the weights for outliers
vector<int> weighted_outliers(const vector<double>& dists,
                              const arma::vec& weights, const int n_outliers,
                              vector<double>& Ow) {
  Ow.clear();
  vector<pair<double, int>> p;
  for (int i = 0; i < dists.size(); ++i) {
    p.push_back({dists[i], i});
  }
  sort(p.begin(), p.end());
  int i = dists.size() - 1;
  double tot_weight = 0;
  vector<int> O;
  while (i >= 0 && tot_weight < n_outliers) {
    int idx = p[i].second;
    O.push_back(idx);
    if (weights[idx] + tot_weight > n_outliers) {
      Ow.push_back(n_outliers - tot_weight);
      tot_weight = n_outliers;
    } else {
      Ow.push_back(weights[idx]);
      tot_weight += weights[idx];
    }
    --i;
  }
  if (tot_weight < n_outliers) {
    throw runtime_error("weighted_indices_of_upper: tot_weight < n_outliers");
  }
  return O;
}

double dist2(const arma::vec& a, const arma::vec& b) {
  return arma::norm(a - b, 2);
}

double dist(const arma::vec& a, const arma::vec& b) {
  return std::sqrt(arma::accu(arma::square(a - b)));
}

std::vector<int> sample_with_repl(const int n, const int m) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, n - 1);
  std::vector<int> samples;
  for (int i = 0; i < m; ++i) {
    samples.push_back(dis(gen));
  }
  return samples;
}

std::vector<int> sample_without_repl(const int n, const int m) {
  std::vector<int> nums;
  for (int i = 0; i < n; ++i) {
    nums.push_back(i);
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  std::shuffle(nums.begin(), nums.end(), gen);
  auto e = nums.begin();
std:
  advance(e, std::min(n, m));
  return std::vector<int>(nums.begin(), e);
}

// Erfan:
void convert_to_mat(arma::mat& destination,
                    const std::vector<arma::vec>& source) {
  // assumes correct size on destination
  for (int i = 0; i < destination.n_rows; i++)
    for (int j = 0; j < destination.n_cols; j++)
      destination(i, j) = source.at(i)(j);
}
void convert_from_mat(std::vector<arma::vec>& destination,
                      const arma::mat& source) {
  for (int i = 0; i < source.n_rows; i++) {
    destination.push_back(source.row(i).t());
  }
}

void copy_vec_to_array(
    double* destination,
    const arma::vec& source) {  // TODO: find the natural way of doing this
  for (int j = 0; j < source.n_elem; j++) {
    destination[j] = source(j);
  }
}

void copy_to_array(double* destination, const std::vector<arma::vec>& source) {
  for (int i = 0; i < source.size(); i++) {
    int n_cols = source.at(i).n_elem;
    for (int j = 0; j < n_cols; j++) {
      destination[i * n_cols + j] = source.at(i)(j);
    }
  }
}

bool equal_mat(const std::vector<arma::vec>& a, const arma::mat& b) {
  bool equal = true;
  for (int i = 0; i < b.n_rows; i++)
    for (int j = 0; j < b.n_cols; j++) equal &= (b(i, j) == a.at(i)(j));
  return equal;
}

//:Erfan
void compute_nearest(const std::vector<arma::vec>& U, const std::vector<int>& S,
                     std::vector<int>& res_idx, std::vector<double>& min_dist) {
  vector<arma::vec> SS;
  for (int i : S) {
    SS.push_back(U[i]);
  }
  compute_nearest(U, SS, res_idx, min_dist);
}

void compute_nearest(const std::vector<arma::vec>& U,
                     const std::vector<arma::vec>& S, std::vector<int>& res_idx,
                     std::vector<double>& min_dist) {
  res_idx.clear();
  min_dist.clear();
  for (int i = 0; i < U.size(); ++i) {
    int idx = -1;
    double m = 1e10;
    for (int j = 0; j < S.size(); ++j) {
      auto tmp = util::dist(U[i], S[j]);
      if (tmp < m) {
        idx = j;
        m = tmp;
      }
    }
    res_idx.push_back(idx);
    min_dist.push_back(m);
  }
}

std::vector<int> indices_of_lower(const std::vector<double>& values,
                                  const int n) {
  std::vector<std::pair<double, int>> p;
  int i = 0;
  for (auto& v : values) {
    p.push_back({v, i});
    ++i;
  }

  sort(p.begin(), p.end());
  std::vector<int> res;
  for (i = 0; i < n && i < values.size(); ++i) {
    res.push_back(p[i].second);
  }
  return res;
}

std::vector<int> indices_of_upper(const std::vector<double>& values,
                                  const int n) {
  std::vector<double> neg_values;
  for (auto v : values) {
    neg_values.push_back(-v);
  }
  return indices_of_lower(neg_values, n);
}

double cost(const std::vector<arma::vec>& U,
            const std::vector<arma::vec>& centers, CostType type) {
  std::vector<double> m_dist;
  std::vector<int> labels;
  compute_nearest(U, centers, labels, m_dist);
  return cost(U, centers, labels, type);
}

double cost(const std::vector<arma::vec>& U,
            const std::vector<arma::vec>& centers,
            const std::vector<int>& labels, CostType type) {
  return cost_outliers(U, centers, {}, labels, type);
}

double cost_outliers(const std::vector<arma::vec>& U,
                     const std::vector<arma::vec>& centers,
                     const std::vector<int>& outliers,
                     const std::vector<int>& labels, CostType type) {
  std::set<int> O(outliers.begin(), outliers.end());
  auto f = [](double x) { return x * x; };
  double c = 0;

  if (type == CostType::k_median) {
    for (int i = 0; i < U.size(); ++i) {
      if (O.count(i) > 0) {
        continue;
      }
      c += dist(U[i], centers[labels[i]]);
    }
  } else {
    for (int i = 0; i < U.size(); ++i) {
      if (O.count(i) > 0) {
        continue;
      }
      c += f(dist(U[i], centers[labels[i]]));
    }
  }
  return c;
}

void find_indices(std::vector<int>& outputOutliers,
                  const std::vector<arma::vec>& baseData,
                  const std::vector<arma::vec>& secondData,
                  const std::vector<int>& secondOutliers_indices) {
  std::vector<arma::vec> listOfOutliers;
  for (int i = 0; i < secondOutliers_indices.size(); i++) {
    listOfOutliers.push_back(secondData[secondOutliers_indices[i]]);
  }
  std::vector<int> res_idx;
  std::vector<double> min_dist;
  // TODO:Error!
  //  for(auto v: baseData){
  //    v.print("v:");
  //  }
  //  std::cout<<"secondData:"<<std::endl;
  //  for(auto v: secondData){
  //    v.print("v:");
  //  }
  //
  //  std::cout<<"listOfOutliers:"<<std::endl;
  //  for(auto v: listOfOutliers){
  //    v.print("v:");
  //  }

  compute_nearest(baseData, listOfOutliers, res_idx, min_dist);
  for (int i = 0; i < baseData.size(); i++)
    if (min_dist[i] < 0.001) {
      outputOutliers.push_back(i);
    }
}
// TODO: make this template
void print_vector(const std::vector<int>& vector) {
  for (auto i = vector.begin(); i != vector.end(); ++i) std::cout << *i << ' ';
  std::cout << std::endl;
}
void arrToStream(std::ostream& stream, double* arr, int length) {
  for (int i = 0; i < length - 1; i++) stream << arr[i] << ",";
  stream << arr[length - 1];
}
void randomList(std::vector<int>& output, const arma::vec& weights,
                const int k) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::vector<double> cum;
  std::set<double> outputSet;
  int n = weights.n_elem;
  cum.push_back(weights[0]);
  for (int i = 1; i < n; i++) {
    cum.push_back(cum[i - 1] + weights[i]);
  }
  double total = cum[n - 1];
  std::uniform_real_distribution<> dis(0, total);
  //  print_vectord(cum);
  //  VARd(total);
  for (int i = 0; outputSet.size() < k && i < 10000000; i++) {
    double r = ((double)rand() / RAND_MAX) * total;
    //    r=dis(gen);
    auto p = std::lower_bound(cum.begin(), cum.end(), r);
    int candidate = p - cum.begin();
    if (outputSet.count(candidate) == 0) outputSet.insert(candidate);
  }
  output.clear();
  output.insert(output.begin(), outputSet.begin(), outputSet.end());
}

void average_centers(std::vector<arma::vec>& newCenters,
                     const std::vector<arma::vec>& data,
                     const arma::vec weights, const std::vector<int>& labels,
                     const std::vector<int>& outliersVec, const int n_centers,
                     const std::vector<arma::vec>& oldCenters) {
  std::set<int> outliers(outliersVec.begin(), outliersVec.end());
  int dim = data[0].n_elem;
  arma::vec zero = arma::zeros<arma::vec>(dim);
  arma::vec avg = zero;
  std::vector<double> totalWeight(n_centers);
  //  newCenters.clear();
  //  std::fill(newCenters.begin(),newCenters.end(),zero);
  newCenters.clear();
  newCenters.resize(n_centers);
  for (int i = 0; i < n_centers; ++i) {
    newCenters[i].resize(dim);
    newCenters[i].fill(0);
    //    newCenters[i].print("here:");
  }
  // std::cout<<newCenters.size()<<std::endl;
  for (int i = 0; i < data.size(); ++i) {
    if (outliers.count(i) > 0) {
      continue;
    }
    //    newCenters[labels[i]].print("before:");
    //    std::cout<<weights[i]<<","<<data[i]<<std::endl;
    newCenters[labels[i]] += arma::vec(weights[i] * data[i]);
    //    newCenters[labels[i]].print("after:");
    totalWeight[labels[i]] += weights[i];
  }

  for (int i = 0; i < n_centers; ++i) {
    avg += newCenters[i] / n_centers;
  }
  static int c1 = 0, c2 = 0, c3 = 0;
  std::vector<int> empties;
  for (int i = 0; i < n_centers; ++i) {
    if (totalWeight[i] > 0) {
      newCenters[i] /= totalWeight[i];
    } else {
      newCenters[i] = zero;
      if (false && oldCenters.size() > i)
        newCenters[i] = oldCenters[i];
      else
        newCenters[i] = avg;
    }
  }
  //  if(empties.size()>0){
  //    std::vector<int> res_idx;
  //    std::vector<double> min_dist;
  //    arma::vec newWeights(data.size());
  //    util::compute_nearest(data,newCenters,res_idx,min_dist);
  //    for(int i=0;i<data.size();i++){
  //      newWeights[i]=weights[i]*min_dist[i];
  //    }
  //    std::vector<int> rInds;
  //    util::randomList(rInds,weights,empties.size());
  //    for (int i = 0; i < rInds.size(); ++i) {
  //      newCenters[empties[i]]=data[rInds[i]];
  //    }
  //  }
  //  VARi(c1);
  //  VARi(c2);
  //  VARi(c3);
}

void weights_of_upper(arma::vec& newWeights, const std::vector<double>& values,
                      const arma::vec& weights, const double bag) {
  newWeights = weights;
  if (bag < 0.99) return;
  std::vector<std::pair<double, int>> p;
  int i = 0;
  for (auto& v : values) {
    p.push_back({-v, i});
    ++i;
  }

  std::sort(p.begin(), p.end());
  double currentWeight = 0;
  for (i = 0; i < values.size(); ++i) {
    double neededWeight = std::min(bag - currentWeight, weights[p[i].second]);
    newWeights[p[i].second] = weights[p[i].second] - neededWeight;
    currentWeight += neededWeight;
    //    std::cout<<p[i].first<<" "<<p[i].second<<" "<<weights[p[i].second]<<"
    //    "<<currentWeight<<std::endl;
    if (currentWeight > bag - 0.0001) break;
  }
}
std::string space2underscore(std::string text) {
  for (std::string::iterator it = text.begin(); it != text.end(); ++it) {
    if (!std::isalnum(*it)) {
      *it = '_';
    }
  }
  return text;
}


double sum(const vector<double>& nums) {
  double sm = 0;
  for (auto a : nums) {
    sm += a;
  }
  return sm;
}

double sum2(const vector<double>& nums) {
  double sm = 0;
  for (auto a : nums) {
    sm += a * a;
  }
  return sm;
}

double jaccard(const std::vector<int>& A, const std::vector<int>& B) {
  set<int> AA(A.begin(), A.end());
  set<int> BB(B.begin(), B.end());
  int n_A = AA.size();
  int n_B = BB.size();
  AA.insert(B.begin(), B.end());
  return  (n_A + n_B - AA.size()) / (AA.size() + 0.);
}

double recall(const std::vector<int>& A, const std::vector<int>& B) {
  set<int> AA(A.begin(), A.end());
  set<int> BB(B.begin(), B.end());
  int n_A = AA.size();
  int n_B = BB.size();
  AA.insert(B.begin(), B.end());
  return  (n_A + n_B - AA.size()) / (A.size() + 1e-20);
}

double precision(const vector<int>& A, const vector<int>& B) {
  set<int> AA(A.begin(), A.end());
  set<int> BB(B.begin(), B.end());
  int n_A = AA.size();
  int n_B = BB.size();
  AA.insert(B.begin(), B.end());
  return  (n_A + n_B - AA.size()) / (B.size() + 1e-20);
}

  
vector<double> arma_vec_to_vector(const arma::vec& vec) {
  vector<double> res;
  for (size_t i = 0; i < vec.size(); ++i) {
    res.push_back(vec[i]);
  }
  return res;
}


// TODO: add test
vector<int> weighted_sample_with_repl(const vector<double>& weights, const int m) {
  // construct a trivial random generator engine from a time-based seed:
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator (seed);

  std::discrete_distribution<int> sampler(weights.begin(), weights.end());
  vector<int> res;
  for (int i = 0; i < m; ++i) {
    res.push_back(sampler(generator));
  }
  return res;

  // double w = 0;
  // for (auto a : weights) {
  //   if (a < 0) {
  //     throw runtime_error("weighted_sample_with_repl: invalid weights");
  //   }
  //   w += a;
  // }
  // if (w <= 0) {
  //   throw runtime_error("weighted_sample_with_repl: invalid weights");
  // }

  // double scale = 1.;
  // if (w < 1000) {
  //   scale = 1000 / w;
  //   w = 1000;
  // }
  
  // auto&& indices = util::sample_with_repl(floor(w + 1e-20), m);
  // sort(indices.begin(), indices.end());
  // vector<int> res;
  // int idx = 0;
  // w = 0;
  // for (int i : indices) {
  //   while (w + scale * weights[idx] < i) {
  //     w += scale * weights[idx];
  //     ++idx;
  //   }
  //   res.push_back(idx);
  // }
  // return res;
}

vector<int> weighted_sample_without_repl(const vector<double>& weights, const int m) {
  set<int> sampled;
  if (m > weights.size()) {
    throw runtime_error("w_sample_without_repl: m > n");
  }
  // construct a trivial random generator engine from a time-based seed:
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator (seed);

  std::discrete_distribution<int> sampler(weights.begin(), weights.end());
  while (sampled.size() < m) {
    sampled.insert(sampler(generator));
  }
  vector<int> res(sampled.begin(), sampled.end());
  return res;
}
  
}
