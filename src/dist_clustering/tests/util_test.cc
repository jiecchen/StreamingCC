#include "../algorithms/util.h"
#include "../experiments/dataManager.h"
#include "../experiments/erfanUtil.h"

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

#include "../lest.hpp"

#include <iostream>
#include <set>
#include <vector>

using namespace std;

const lest::test specification[] = {CASE("dist(a, b)"){
    EXPECT(util::dist(arma::vec{1, 1}, arma::vec{1, 2}) == 1);
EXPECT(util::dist(arma::vec{1, 2}, arma::vec{1, 2}) == 0);
EXPECT(util::dist(arma::vec{3, 4}, arma::vec{0, 0}) == 5);
}
,
    // Erfan:
    CASE("convert to from mat") {
  arma::mat A;

  // endr indicates "end of row"
  A << 0.555950 << 0.274690 << 0.540605 << 0.798938 << arma::endr << 0.108929
    << 0.830123 << 0.891726 << 0.895283 << arma::endr << 0.948014 << 0.973234
    << 0.216504 << 0.883152 << arma::endr << 0.023787 << 0.675382 << 0.231751
    << 0.450332 << arma::endr;
  vector<arma::vec> B;
  util::convert_from_mat(B, A);
  bool equal = true;
  for (int i = 0; i < A.n_rows; i++)
    for (int j = 0; j < A.n_cols; j++) equal &= (A(i, j) == B.at(i)(j));
  EXPECT(equal);

  EXPECT(util::equal_mat(B, A));

  arma::mat C(size(A));
  util::convert_to_mat(C, B);
  EXPECT(util::equal_mat(B, C));
}
, CASE("convert to array") {
  arma::mat A;

  // endr indicates "end of row"
  A << 0.555950 << 0.274690 << 0.540605 << 0.798938 << arma::endr << 0.108929
    << 0.830123 << 0.891726 << 0.895283 << arma::endr << 0.948014 << 0.973234
    << 0.216504 << 0.883152 << arma::endr << 0.023787 << 0.675382 << 0.231751
    << 0.450332 << arma::endr;
  vector<arma::vec> B;
  util::convert_from_mat(B, A);
  double* D = (double*)malloc(A.n_rows * A.n_cols * sizeof(double));
  util::copy_to_array(D, B);
  bool equal = true;
  for (int i = 0; i < A.n_rows; i++)
    for (int j = 0; j < A.n_cols; j++) {
      equal &= (A(i, j) == D[i * A.n_cols + j]);
    }
  EXPECT(equal);
}
, CASE("sample_with_repl(n, m)") {
  vector<int> exp1{0, 0};
  EXPECT(util::sample_with_repl(1, 2) == exp1);
  auto vv = util::sample_with_repl(3, 100);
  EXPECT(vv.size() == 100);
  set<int> sv(vv.begin(), vv.end());
  EXPECT(sv.size() == 3);
}
,

    CASE("remove_subset(U, idx)") {
  vector<string> U{"xx", "aq", "zz", "sw", "o"};
  vector<int> indices{0, 1, 2, 3, 4};

  vector<int> idx{0, 1};
  util::remove_subset<string>(U, idx);
  util::remove_subset<int>(indices, idx);
  vector<string> exp_U{"zz", "sw", "o"};
  vector<int> exp_idx{2, 3, 4};
  EXPECT(U == exp_U);
  EXPECT(indices == exp_idx);

  util::remove_subset<string>(U, {1});
  util::remove_subset<int>(indices, {1});
  vector<string> exp_U1{"zz", "o"};
  vector<int> exp_idx1{2, 4};
  EXPECT(U == exp_U1);
  EXPECT(indices == exp_idx1);
}
,

    CASE("indices_of_lower(values, n)") {
  vector<double> vec{0.5, 1.1, 100., 0.7, 0.9};
  vector<int> exp1{0, 3, 4};
  EXPECT(util::indices_of_lower(vec, 3) == exp1);
  vector<int> exp2{0, 1, 2, 3, 4};
  EXPECT(util::indices_of_lower(vec, 6).size() == 5);
}
,

    CASE("indices_of_upper(values, n)") {
  vector<double> vec{0.5, 1.1, 100., 0.7, 0.9};
  vector<int> exp1{2, 1, 4};
  EXPECT(util::indices_of_upper(vec, 3) == exp1);
  vector<int> exp2{0, 1, 2, 3, 4};
  EXPECT(util::indices_of_upper(vec, 6).size() == 5);
}
,

    CASE("compute_nearest(U, S, res_idx, min_dist)") {
  vector<int> res_idx;
  vector<double> min_dist;
  vector<arma::vec> U;
  for (double d : {1, 3, 7, 9, 10}) {
    U.push_back(arma::vec({d}));
  }
  vector<int> S{0, 3};
  util::compute_nearest(U, S, res_idx, min_dist);
  vector<int> exp_idx{0, 0, 3, 3, 3};
  vector<double> exp_dist{0, 2, 2, 0, 1};
  EXPECT(res_idx == exp_idx);
  EXPECT(min_dist == exp_dist);
}
,

    CASE("compute_nearest(U, vector<arma::vec>, res_idx, min_dist)") {
  vector<int> res_idx;
  vector<double> min_dist;
  vector<arma::vec> U;
  for (double d : {1, 3, 7, 9, 10}) {
    U.push_back(arma::vec({d}));
  }
  vector<arma::vec> S{arma::vec{1}, arma::vec{9}};
  util::compute_nearest(U, S, res_idx, min_dist);
  vector<int> exp_idx{0, 0, 1, 1, 1};
  vector<double> exp_dist{0, 2, 2, 0, 1};
  EXPECT(res_idx == exp_idx);
  EXPECT(min_dist == exp_dist);
}
,

    CASE("subset(U, idx)") {
  vector<int> exp1{2, 3};
  vector<int> U{1, 2, 3, 4, 5, 6, 7, 8, 9};
  vector<int> idx{1, 2};
  auto res = util::subset(U, idx);
  EXPECT(res == exp1);
}
,

    CASE("sample_without_repl(n, m)") {
  auto v = util::sample_without_repl(1, 1);
  vector<int> exp1{0};
  EXPECT(v == exp1);
  v = util::sample_without_repl(10, 9);
  set<int> st(v.begin(), v.end());
  EXPECT(st.size() == 9);
}
,

    CASE("cost(U, centers, [labels], type)") {
  vector<arma::vec> U{arma::vec{1}, arma::vec{2}, arma::vec{5}};
  vector<arma::vec> S{arma::vec{1}, arma::vec{2}};
  vector<int> labels{0, 1, 1};
  EXPECT(util::cost(U, S, labels, util::CostType::k_median) == 3);
  EXPECT(util::cost(U, S, labels, util::CostType::k_mean) == 9);
  // test cost(U, centers, type)
  EXPECT(util::cost(U, S, util::CostType::k_median) == 3);
  EXPECT(util::cost(U, S, util::CostType::k_mean) == 9);
}
,

    CASE("cost_outliers(U, centers, outliers, labels, type)") {
  vector<arma::vec> U{arma::vec{1}, arma::vec{2}, arma::vec{50}};
  vector<arma::vec> S{arma::vec{1}, arma::vec{4}};
  vector<int> labels{0, 1, 1};
  vector<int> O{2};
  EXPECT(util::cost_outliers(U, S, O, labels, util::CostType::k_median) == 2);
  EXPECT(util::cost_outliers(U, S, O, labels, util::CostType::k_mean) == 4);
}
,

    CASE("void find_indices") {
  //    void find_indices(std::vector<int>& outputOutliers,
  //        const std::vector<arma::vec>& baseData,const std::vector<arma::vec>&
  //        secondData,
  //        const std::vector<int>& secondOutliers_indices){
  std::vector<int> outputOutliers;
  std::vector<arma::vec> baseData;
  std::vector<arma::vec> secondData;
  std::vector<int> secondOutliers_indices;

  baseData.push_back(arma::vec({2, 3}));
  baseData.push_back(arma::vec({4, 5}));
  baseData.push_back(arma::vec({6, 7}));
  baseData.push_back(arma::vec({8, 9}));
  baseData.push_back(arma::vec({-6, -7}));

  secondData.push_back(arma::vec({2, 3}));
  secondData.push_back(arma::vec({8, 9}));
  secondData.push_back(arma::vec({-6, -7}));

  secondOutliers_indices.push_back(0);
  secondOutliers_indices.push_back(2);
  util::find_indices(outputOutliers, baseData, secondData,
                     secondOutliers_indices);
  //    std::cout<<outputOutliers[0]<<std::endl;
  EXPECT(outputOutliers.size() == secondOutliers_indices.size());
  EXPECT(outputOutliers[0] == 0);
  EXPECT(outputOutliers[1] == 4);
}
, CASE("average_centers") {
  /*
   * void average_centers(vector<arma::vec>& newCenters,
const vector<arma::vec>& data,
const arma::vec weights,
      const vector<int>& labels,
      const std::vector<int>& outliersVec,
      const int n_centers)
   */
  vector<arma::vec> newCenters, oldCenters;
  DataSet dataSet;
  dataSet.name = "16points";
  dataSet.load();
  const arma::vec weights = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  const vector<int> labels = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3};
  const std::vector<int> outliersVec;
  const int n_centers = 4;
  util::average_centers(newCenters, dataSet.data, weights, labels, outliersVec,
                        n_centers, oldCenters);
  //    for(int i=0;i<newCenters.size();i++){
  //      newCenters[i].print("i: ");
  //    }
}
, CASE("randomList") {
  srand(time(NULL));
  arma::vec w = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  std::map<int, int> map;
  std::vector<int> output;
  int k = 1000000;
  util::randomList(output, w, k);
  int sum_of_elems = 0;
  for (auto& v : output) {
    sum_of_elems += v;
    map[v]++;
  }
  //      for (uint i=0;i<w.size();i++){
  //        std::cout<<i<<" "<<w[i]<<" "<<map[i]<<"
  //        "<<15*(double)map[i]/(double)k<< std::endl;
  //      }
  //    EXPECT(sum_of_elems>3*k);
  //    EXPECT(sum_of_elems<3.6*k);
}
,
/*
    CASE("indices_of_upper_weighted"){
      //    td::vector<int> indices_of_upper_weighted(const std::vector<double>&
values
      //        , const arma::vec& weights
      //        , const double bag) {
      std::vector<double> vec{0.5, 1.1, 100., 0.7, 0.9};
      std::vector<int> exp1{2, 1, 4};
      arma::vec weights(vec.size(),1);
      weights.fill(1.0);
//      weights.print("weights:");
      EXPECT( util::indices_of_upper_weighted(vec, weights,3) == exp1 );
      vector<int> exp2{0, 1, 2, 3, 4};
      EXPECT( util::indices_of_upper_weighted(vec, weights, 6).size() == 5 );

      weights[2]=100;
      EXPECT( util::indices_of_upper_weighted(vec, weights,100).size() == 1 );
      EXPECT( util::indices_of_upper_weighted(vec, weights,101).size() == 2 );
      EXPECT( util::indices_of_upper_weighted(vec, weights,1010).size() == 5 );

    }
*/
                                    CASE("jaccard") {
                                      vector<int> A{1, 2, 3, 4};
                                      vector<int> B{2, 3, 4, 5};
                                      EXPECT( util::jaccard(A, B) == 0.6 );
                                    }
}
;

int main() { return lest::run(specification); }
