#ifndef _ALGO_LIST_H
#define _ALGO_LIST_H

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

#include "../distAlgo/multiRoundSampling.h"
#include "../distAlgo/partitioning.h"
#include "algo_util.h"
#include "local_search.h"
#include "util.h"
#include "succ_sampling.h"
#include "centeralizedMultiRoundSampling.h"
#include "kmean_minus_minus.h"
#include "simple_algo.h"

#include <cassert>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

void addRandPartSweep(std::vector<std::unique_ptr<ClusteringAlgo>>& algos, int np,
    int id, int root, int n_clusters, int n_outliers, int n) {
  int n_local_cluster=((int)((double)n_clusters+n_outliers)*log(n));
  for(int i=n_clusters+n_outliers;i<n_local_cluster*5;i++){
    addAlgo(algos,
        new PartitionClustering(new RandomClustering(), new FastWeightedKMeanMM(),
          np, id, root, n_local_cluster,true));
  }
}

void addSSsweep(std::vector<std::unique_ptr<ClusteringAlgo>>& algos, int np,
    int id, int root, int n_clusters) {
  double alpha = 0;
  double beta = 0;
  for (alpha = 0.1; alpha < 1.6; alpha += 0.09) {
    for (beta = 0.2; beta < 0.61; beta += 0.09) {
      addAlgo(algos, new PartitionClustering(new SuccSampling(alpha, beta),
            new FastWeightedKMeanMM(), np, id,
            root, n_clusters));
    }
  }
}

void addDistComp(std::vector<std::unique_ptr<ClusteringAlgo>>& algos, int np,
    int id, int root, int n_clusters, int n) {
  double alpha = 0.9;
  double beta = 0.3;
  int n_rounds =  log(n);
  int n_cenPerRound = n_clusters;

  addAlgo(algos, new PartitionClustering(new SuccSampling(alpha, beta),
        new FastWeightedKMeanMM(), np, id,
        root, n_clusters));

  addAlgo(algos, new TwoPhaseClustering(
        new MultiRoundSamplingClustering(n_rounds, true, np, id,
          root, n_cenPerRound),
        new FastWeightedKMeanMM(), np, id, root));
}

void addKmeansPar(std::vector<std::unique_ptr<ClusteringAlgo>>& algos, int np,
    int id, int root, int n_clusters, int n) {
  int n_rounds = 2 * log(n);
  int n_cenPerRound = n_clusters;

  addAlgo(algos, new TwoPhaseClustering(
        new MultiRoundSamplingClustering(n_rounds, true, np, id,
          root, n_cenPerRound),
        new FastWeightedKMeanMM(), np, id, root));
}

void addThirdsweep(std::vector<std::unique_ptr<ClusteringAlgo>>& algos, int np,
    int id, int root, int n_clusters, int n) {
  for (int numberOfRounds = 1; numberOfRounds < 3 * log(n); numberOfRounds++) {
    for (int numberOfCentersPerRound = 1;
        numberOfCentersPerRound < 2 * n_clusters; numberOfCentersPerRound++) {
      if (numberOfCentersPerRound * numberOfRounds > n_clusters) {
        addAlgo(algos,
            new PartitionClustering(
              new CenteralizedMultiRoundSamplingClustering(
                numberOfRounds, true, numberOfCentersPerRound),
              new FastWeightedKMeanMM(), np, id, root, n_clusters));
      }
    }
  }
}

void addPPsweep(std::vector<std::unique_ptr<ClusteringAlgo>>& algos, int np,
    int id, int root, int n_clusters, int n) {
  for (int numberOfRounds = 1; numberOfRounds < 3 * log(n);
      numberOfRounds += 2) {
    for (int numberOfCentersPerRound = 1;
        numberOfCentersPerRound < 2.2 * n_clusters;
        numberOfCentersPerRound += 7) {
      //      VARi(numberOfCentersPerRound);
      //      VARi(numberOfRounds);
      if (numberOfCentersPerRound * numberOfRounds > n_clusters) {
        addAlgo(algos, new TwoPhaseClustering(
              new MultiRoundSamplingClustering(
                numberOfRounds, true, np, id, root,
                numberOfCentersPerRound),
              new FastWeightedKMeanMM(), np, id, root));
      }
    }
  }
}
void addCenAlgos(std::vector<std::unique_ptr<ClusteringAlgo>>& algos) {
  addAlgo(algos, new Kmeanspp());
  addAlgo(algos, new KMeanMinusMinus());
  addAlgo(algos, new RandomClustering());
  addAlgo(algos, new WeightedKMeanMM());
  addAlgo(algos, new ErfanMM(false));
  addAlgo(algos, new ErfanMM(true));
  addAlgo(algos, new FastWeightedKMeanMM());
  addAlgo(algos, new FLSAlgo());
}

void addAlgosExp1(std::vector<std::unique_ptr<ClusteringAlgo>>& algos, int np,
    int id, int root, int n_clusters, int n) {
  addAlgo(algos, new Kmeanspp());
  addAlgo(algos, new RandomClustering());
  addAlgo(algos, new FLSAlgo());
  //  addAlgo( algos,new FLSAlgo());
  //  ((FLSAlgo*)algos.back().get())->setMaxIters(30);
  addAlgo(algos, new FastWeightedKMeanMM());

  double alpha = 0.9;
  double beta = 0.3;

  addAlgo(algos,
      new PartitionClustering(new SuccSampling(alpha, beta), new FLSAlgo(),
        np, id, root, n_clusters));

  addAlgo(algos,
      new PartitionClustering(new SuccSampling(alpha, beta), new FastWeightedKMeanMM(),
        np, id, root, n_clusters));
  addAlgo(algos,
      new PartitionClustering(new SuccSampling(alpha, beta), new WeightedLocalSearchAlgo(),
        np, id, root, n_clusters));
}
void addAlgosExp2(std::vector<std::unique_ptr<ClusteringAlgo>>& algos, int np,
    int id, int root, int n_clusters, int n) {
  addAlgo(algos, new Kmeanspp());
  addAlgo(algos, new RandomClustering());
  addAlgo(algos, new FastWeightedKMeanMM());

  double alpha = 0.9;
  double beta = 0.3;
  int n_outliers=100;
  int n_local_cluster=((int)((double)n_clusters+n_outliers)*log(n)*alpha*(-log(beta))*0.8);
  //  VARi(n_local_cluster);
  //  addAlgo(algos,
  //      new PartitionClustering(new Kmeanspp(), new FastWeightedKMeanMM(),
  //        np, id, root, n_local_cluster,true));

  addAlgo(algos,
      new PartitionClustering(new SuccSampling(alpha, beta), new FastWeightedKMeanMM(),
        np, id, root, n_clusters));
  addAlgo(algos,
      new PartitionClustering(new RandomClustering(), new FastWeightedKMeanMM(),
        np, id, root, n_local_cluster,true));
}
#endif
