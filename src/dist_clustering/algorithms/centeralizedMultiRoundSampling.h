// kmeanspp
#ifndef __CENTERALIZEDMUTLIROUNDSAMPLING_H__
#define __CENTERALIZEDMUTLIROUNDSAMPLING_H__

#include "algo_util.h"

class CenteralizedMultiRoundSamplingClustering : public ClusteringAlgo {
 public:
  CenteralizedMultiRoundSamplingClustering() {}
  CenteralizedMultiRoundSamplingClustering(const int n_rounds,
                                           const bool squareDist,
                                           int n_centerPerRound) {
    this->n_rounds = n_rounds;
    this->squareDist = squareDist;
    this->n_centerPerRound = n_centerPerRound;
  }
  void perform(const ClusteringInput& input, ClusteringOutput& output) override;
  std::string paramStr() override {
    std::stringstream ostream;
    ostream << n_rounds << " " << squareDist << " " << n_centerPerRound;
    return ostream.str();
  }
  //  int n_nodes; exists for all algs
  int n_centerPerRound;
  int n_rounds;
  bool squareDist;
};


class Kmeanspp: public  ClusteringAlgo {
  public:
    Kmeanspp():squareDist(true){}
    Kmeanspp(bool sd){
      this->squareDist=sd;
    }

  void perform(const ClusteringInput& input, ClusteringOutput& output) override;

  std::string paramStr() override {
    std::stringstream ostream;
    ostream << squareDist;
    return ostream.str();
  }

  bool squareDist;
};

#endif
