#include "../algorithms/timer.h"
#include "../algorithms/local_search.h"
#include "../experiments/dataManager.h"
#include "../algorithms/kmean_minus_minus.h"
#include "algo.hpp"

namespace mpi = boost::mpi;
using namespace std;


int main(int argc, char* argv[]) {
  mpi::environment env(argc, argv);
  mpi::communicator world;
  Timer timer;  
  DataSet dataSet;
  vector<int> actual_outliers;
  int n_centers = 0;
  int n_outliers = 0;
  
  if (argc != 4) {
    if (world.rank() == ROOT) {
      cerr << "usage: mpirun -np n_partitions ./bg2 k t dataset" << endl;
    }
    return 0;
  } else {
    n_centers = atoi(argv[1]);
    n_outliers = atoi(argv[2]);
  }

  // load and distribute the dataset
  // do this in root process
  arma_vectors U;  // local dataset
  if (world.rank() == ROOT ) {
    cerr << "INFO: #processes = " << world.size() << endl;
    dataSet.name = argv[3];
    dataSet.load();
    actual_outliers = util::add_outliers(dataSet.data, n_outliers, 100000);
    auto&& msg = random_partition(dataSet.data, world.size());
    mpi::scatter(world, msg, U, ROOT);
    //    dataSet.data.clear(); // free the space
  } else {
    mpi::scatter(world, U, ROOT);
  }
  
  if (world.rank() == ROOT) {
    cerr << "INFO: loading data ... " << timer.get_time() << endl;
    timer.reset();
  }

  //  cerr << world.rank() << ": |U| = " << U.data.size() << endl;
  timer.reset();
  


  int msg_size = 0;
  AlgoMPI3 algo_mpi;
  arma::vec weights;
  arma_vectors coreset;
  timer.reset();
  algo_mpi.perform(world, ROOT, U, coreset, "bg", n_centers, n_outliers);
  msg_size = coreset.data.size();
  auto tm = timer.get_time();
  if (world.rank() == ROOT) {
    FastWeightedKMeanMM kmmm;
    ClusteringInput input;
    vector<int> indices;
    input.data = to_vecs(coreset, indices);
    cerr << "|actual_outliers| = " << actual_outliers.size() << endl;
    cerr << "|indices| = " << indices.size() << endl;
    cerr << "recall = " << util::recall(actual_outliers, indices) << endl;
    input.weights = arma::vec(coreset.weights.data);
    input.n_centers = n_centers;
    input.n_outliers = n_outliers;
    ClusteringOutput output;
    kmmm.perform(input, output);
    indices.clear();
    for (int i : output.outliers_indices) {
      indices.push_back(coreset.data[i].idx);
    }
    cerr << "n_outliers = " << n_outliers << endl;
    cerr << "total_weight = " << util::sum(coreset.weights.data) << endl;
    cerr << "|outliers_indices| = " << output.outliers_indices.size() << endl;
    cerr << "after kmean--: recall = " << util::recall(actual_outliers, indices) << endl;
    cout << "bg" << ","
         << n_centers << "," << n_outliers << ","
         << cost2(dataSet.data, output.centers, indices)
         << ","
         << cost1(dataSet.data, output.centers, indices)
	 << "," << tm
         << "," << msg_size
         << endl;
  }
  return 0;
}












