// Successive Sampling
#ifndef __SUCC_SAMPLING_H__
#define __SUCC_SAMPLING_H__

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

#include <vector>
#include "algo_util.h"

class SuccSampling : public ClusteringAlgo {
 public:
  SuccSampling() : alpha_(10.), beta_(0.3), gamma_(8) {}
  SuccSampling(const double alpha, const double beta) {
    alpha_ = alpha;
    beta_ = beta;
    gamma_ = 8;
  }

  void setAlpha(double alpha) { alpha_ = alpha; }
  void setBeta(double beta) { beta_ = beta; }
  void setGamma(double gamma) { gamma_ = gamma; }
  void perform(const ClusteringInput& input, ClusteringOutput& output) override;
  int maxMsgSize(const ClusteringInput& input) override {
    return (input.n_outliers * 8 +
	    alpha_ * input.n_centers * log(input.getNrows()) / beta_) *
      (input.getNcols() + 1) +
        2;
  }
  std::string paramStr() override {
    std::stringstream ostream;
    ostream << alpha_ << " " << beta_;
    return ostream.str();
  }
private:
  double alpha_;
  double beta_;
  double gamma_;
};


class SuccSampling2 : public ClusteringAlgo {
 public:
  SuccSampling2() : alpha_(10.), beta_(0.3), gamma_(8) {}
  SuccSampling2(const double alpha, const double beta) {
    alpha_ = alpha;
    beta_ = beta;
    gamma_ = 8;
  }

  void setAlpha(double alpha) { alpha_ = alpha; }
  void setBeta(double beta) { beta_ = beta; }
  void setGamma(double gamma) { gamma_ = gamma; }
  void perform(const ClusteringInput& input, ClusteringOutput& output) override;
  int maxMsgSize(const ClusteringInput& input) override {
    return (input.n_outliers * 8 +
	    alpha_ * input.n_centers * log(input.getNrows()) / beta_) *
      (input.getNcols() + 1) +
        2;
  }
  std::string paramStr() override {
    std::stringstream ostream;
    ostream << alpha_ << " " << beta_;
    return ostream.str();
  }
private:
  double alpha_;
  double beta_;
  double gamma_;
};


#endif
