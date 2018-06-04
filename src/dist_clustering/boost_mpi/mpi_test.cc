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

  vector<string> algos;
  
  if (argc < 4) {
    if (world.rank() == ROOT) {
      cerr << "usage: mpirun -np a ./mpi_test k t [bg|rand|bg2|rand2|kmean]+" << endl;
    }
    return 0;
  } else {
    n_centers = atoi(argv[1]);
    n_outliers = atoi(argv[2]);
    for (int i = 3; i < argc; ++i) {
      algos.push_back(argv[i]);
    }
  }

  // load and distribute the dataset
  // do this in root process
  arma_vectors U;  // local dataset
  if (world.rank() == ROOT ) {
    cerr << "INFO: #processes = " << world.size() << endl;
    dataSet.name = "kdd10000";
    dataSet.load();
    util::add_outliers(dataSet.data, 500, 10000000);
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
  
  for (const auto& algo : algos) {
    vector<arma::vec> coreset;
    arma::vec weights;
    int msg_size = 0;
    if (algo == "rand") {
      AlgoMPI algo_mpi;    
      algo_mpi.perform(world, ROOT, UU, coreset, weights, "rand", 208, 0);
    } else if (algo == "bg") {
      AlgoMPI algo_mpi;    
      algo_mpi.perform(world, ROOT, UU, coreset, weights, "bg", n_centers, n_outliers);
    } else if (algo == "kmean") {
      kmean_par(world, ROOT, UU, coreset, weights, n_outliers + n_centers, msg_size);
    } else if (algo == "bg2") {
      AlgoMPI2 algo_mpi;    
      algo_mpi.perform(world, ROOT, UU, coreset, weights, "bg", n_centers, n_outliers);      
    } else if (algo == "rand2") {
      AlgoMPI2 algo_mpi;    
      algo_mpi.perform(world, ROOT, UU, coreset, weights, "rand", 545, 0);
    } else {
      cerr << "ERROR: invalid algo type: " << algo << endl;
      return 0;
    }
    
    if (world.rank() == ROOT) {
      cerr << algo << ": |coreset| = " << coreset.size() << endl;
      cerr << algo << ": sent_msg_size = " << msg_size << endl;
      cerr << "INFO: processing ... " << timer.get_time() << endl;
      //      cerr << "INFO: sum(w) = " << arma::sum(weights) << endl;
      FastWeightedKMeanMM kmmm;
      ClusteringInput input;
      input.data = coreset;
      input.weights = weights;
      input.n_centers = n_centers;
      input.n_outliers = n_outliers;
      ClusteringOutput output;
      kmmm.perform(input, output);
      cerr << algo << ": l2 = " <<
	cost2(dataSet.data, output.centers, input.n_outliers) << endl;
    }
  }
  
  return 0;
}












