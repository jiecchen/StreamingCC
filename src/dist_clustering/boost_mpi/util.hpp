#ifndef _UTIL_HPP_
#define _UTIL_HPP_

#include "../algorithms/util.h"
#include "../algorithms/algo_util.h"


#include <boost/serialization/string.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <vector>
#include <exception>

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"


std::vector<double> compute_weights(const std::vector<arma::vec>& U,
				    const std::vector<arma::vec>& C) {
  std::vector<double> dists;
  std::vector<int> labels;
  util::compute_nearest(U, C, labels, dists);
  std::vector<double> weights(C.size());
  std::fill(weights.begin(), weights.end(), 0);
  for (auto i : labels) {
    weights[i]++;
  }
  return weights;
}


class arma_vec {
private:
    friend class boost::serialization::access;
  
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
      ar & idx;
      ar & data;
    }
public:
  arma_vec() {}
  arma_vec(const vec_idx& vi) {
    idx = vi.idx;
    setData(vi.data);
  }
  arma_vec(const arma::vec& vec, int index=0) : idx(index)  {
    setData(vec);
  }

  void clear() {
    data.clear();
    idx = 0;
  }
  
  int idx;
  std::vector<double> data;
private:
  void setData(const arma::vec& vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
      data.push_back(vec[i]);
    }
  }
};


class arma_vectors {
private:
    friend class boost::serialization::access;
  
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
      ar & data;
      ar & weights;
    }
public:
  arma_vectors() {}
  arma_vectors(const std::vector<arma::vec>& vecs,
	       const std::vector<int>& vecs_idx) {
    if (vecs.size() != vecs_idx.size()) {
      throw std::runtime_error("arma_vectors: size error");
    }
    for (size_t i = 0; i < vecs.size(); ++i) {
      data.push_back(arma_vec(vecs[i], vecs_idx[i]));
    }
  }
  arma_vectors(const std::vector<arma::vec>& vecs) {
    setData(vecs);
  }

  
  void clear() {
    data.clear();
    weights.clear();
  }
  
  std::vector<arma_vec> data;
  arma_vec weights;
  
private:
  void setData(const std::vector<arma::vec>& vecs) {
    for (const auto& v : vecs) {
      data.push_back(arma_vec(v));
    }
  }
};


std::vector<arma::vec> to_vecs(const arma_vectors& vecs, std::vector<int>& indices) {
  std::vector<arma::vec> res;
  indices.clear();
  for (const auto& v : vecs.data) {
    res.push_back(arma::vec(v.data));
    indices.push_back(v.idx);
  }
  return res;
}


std::vector<arma::vec> to_vecs(const arma_vectors& vecs) {
  std::vector<arma::vec> res;
  for (const auto& v : vecs.data) {
    res.push_back(arma::vec(v.data));
  }
  return res;
}


std::vector<arma_vectors> random_partition(const std::vector<arma::vec>& U,
					   const int n_partitions) {
  std::map<int, std::vector<arma::vec>> tmp;
  std::map<int, std::vector<int>> tmp_idx;
  
  auto&& idx = util::sample_without_repl(U.size(), U.size());
  for (int i = 0; i < U.size(); ++i) {
    tmp[i % n_partitions].push_back(U[idx[i]]);
    tmp_idx[i % n_partitions].push_back(idx[i]);
  }
  std::vector<arma_vectors> res;
  for (int i = 0; i < n_partitions; ++i) {
    res.push_back(arma_vectors(tmp[i], tmp_idx[i]));
  }
  return res;
}



#endif
