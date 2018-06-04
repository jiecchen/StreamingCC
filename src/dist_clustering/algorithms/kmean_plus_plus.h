#ifndef _KMEAN_PP_H_
#define _KMEAN_PP_H_

#include "algo_util.h"

class KMeanPP : public ClusteringAlgo {
public:
  void perform(const ClusteringInput& input,
	       ClusteringOutput& output) override;
};

#endif
