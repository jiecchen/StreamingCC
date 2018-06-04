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

  int n_centers = 0;
  int n_outliers = 0;
  int coreset_size = 0;
  
  if (argc != 5) {
    if (world.rank() == ROOT) {
      cerr << "usage: mpirun -np n_partitions ./kmean_par k t suggested_coreset_size dataset" << endl;
    }
    return 0;
  } else {
    n_centers = atoi(argv[1]);
    n_outliers = atoi(argv[2]);
    coreset_size = atoi(argv[3]);
  }

  // load and distribute the dataset
  // do this in root process
  arma_vectors U;  // local dataset
  if (world.rank() == ROOT ) {
    cerr << "INFO: #processes = " << world.size() << endl;
    dataSet.name = argv[4];
    dataSet.load();
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
  


  const auto&& UU = to_vecs(U);


  int msg_size = 0;
  AlgoMPI2 algo_mpi;
  arma::vec weights;
  vector<arma::vec> coreset;
  timer.reset();
  kmean_par(world, ROOT, UU, coreset, weights, coreset_size, msg_size);
  msg_size *= world.size();
  auto tm = timer.get_time();
  if (world.rank() == ROOT) {
    FastWeightedKMeanMM kmmm;
    ClusteringInput input;
    kmeanpp(coreset, weights, coreset_size, input.data, input.weights);
    input.n_centers = n_centers;
    input.n_outliers = n_outliers;
    ClusteringOutput output;
    kmmm.perform(input, output);
    cout << "kmean" << ","
         << n_centers << "," << n_outliers << ","
         << coreset_size << ","
         << cost2(dataSet.data, output.centers, input.n_outliers)
         << ","
         << cost1(dataSet.data, output.centers, input.n_outliers)
         << "," << tm
         << "," << msg_size << "," << input.data.size()
         << endl;
  }
  return 0;
}












