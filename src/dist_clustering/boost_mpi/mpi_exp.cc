#include "../algorithms/timer.h"
#include "../algorithms/local_search.h"
#include "../experiments/dataManager.h"
#include "../algorithms/kmean_minus_minus.h"
#include "algo.hpp"

namespace mpi = boost::mpi;
using namespace std;

map<string, double> run_exp(const mpi::communicator& world,
			    const int ROOT,
			    const string& mode,
			    const arma_vectors& U,
			    const DataSet& dataSet,
			    const int n_centers,
			    const int n_outliers,
			    const int coreset_size,
			    const vector<int>& actual_outliers,
			    const int n_repeats) {
  map<string, vector<double>> res;
  for (int ith = 0; ith < n_repeats; ++ith) {
    int msg_size = 0;
    arma_vectors coreset;
    Timer timer;
    if (mode == "bg") {
      AlgoMPI4 algo_mpi;
      algo_mpi.perform(world, ROOT, U, coreset, "bg", n_centers, n_outliers);
      msg_size = coreset.data.size() * world.size();
    } else if (mode == "bg2") {
      AlgoMPI3 algo_mpi;
      algo_mpi.perform(world, ROOT, U, coreset, "bg", n_centers, n_outliers);
      msg_size = coreset.data.size();
    } else if (mode == "km") {
      AlgoMPI3 algo_mpi;
      algo_mpi.perform(world, ROOT, U, coreset, "km", coreset_size, 0);
      msg_size = coreset.data.size();
    } else if (mode == "kmean--") {
      AlgoMPI3 algo_mpi;
      algo_mpi.perform(world, ROOT, U, coreset, "kmean--", n_centers, coreset_size);
      msg_size = coreset.data.size();
    } else if (mode == "rand") {
      AlgoMPI3 algo_mpi;
      algo_mpi.perform(world, ROOT, U, coreset, "rand", coreset_size, 0);
      msg_size = coreset.data.size();
    } else if (mode == "kmean++") {
      mpi_kmean(world, ROOT, U, coreset, coreset_size / 4.8, msg_size);
      msg_size *= world.size();
    } else {
      throw runtime_error("run_exp: invalid mode!");
    }

    res["tm"].push_back(timer.get_time());
    timer.reset();
    res["coreset_size"].push_back(coreset.data.size());
    
    // do re-clustering
    if (world.rank() == ROOT) {
      FastWeightedKMeanMM kmmm;
      kmmm.setKmeanppSeeding(false);
      ClusteringInput input;
      vector<int> indices;
      input.data = to_vecs(coreset, indices);
      
      res["pre-recall"].push_back(util::recall(actual_outliers, indices));
      
      input.weights = arma::vec(coreset.weights.data);
      input.n_centers = n_centers;
      input.n_outliers = n_outliers;
      ClusteringOutput output;
      kmmm.perform(input, output);
      
      indices.clear();
      for (int i : output.outliers_indices) {
	indices.push_back(coreset.data[i].idx);
      }
      res["recall"].push_back(util::recall(actual_outliers, indices));
      res["precision"].push_back(util::precision(actual_outliers, indices));
      res["l2"].push_back(cost2(dataSet.data, output.centers, indices));
      res["l1"].push_back(cost1(dataSet.data, output.centers, indices));
      res["msg_size"].push_back(msg_size);
    }
  }
  map<string, double> final_res;
  for (const auto& p : res) {
    final_res[p.first] = util::avg(p.second);
  }
  return final_res;
}


bool parse_args(int argc, char* argv[],
		set<string>& modes,
		int& n_centers,
		int& n_outliers,
		int& coreset_size,
		string& dataset,
		double& upper_bound,
		int& n_repeats) {
  ///////////// parsing arguments //////////////////
  if (argc < 5) {
    return false;
  }
  modes.clear();
  set<string> valid_modes{"bg", "bg2", "km", "rand", "kmean++", "kmean--"};
  int i = 1;
  while (valid_modes.count(argv[i])) {
    modes.insert(argv[i++]);
  }
  if (modes.size() == 0) {
    return false;
  }
  n_centers = atoi(argv[i++]);
  n_outliers = atoi(argv[i++]);
  coreset_size = -1;
  if (modes.count("rand") > 0 || modes.count("kmean++") > 0
      || modes.count("km") > 0 || modes.count("kmean--") > 0) {
    coreset_size = atoi(argv[i++]);
  }
  dataset = argv[i++];
  upper_bound = atof(argv[i++]);
  if (i >= argc) {
    n_repeats = 1;
  } else {
    n_repeats = atoi(argv[i]);
  }
  return true;
}


int main(int argc, char* argv[]) {
  mpi::environment env(argc, argv);
  mpi::communicator world;
  Timer timer;  
  DataSet dataSet;

  ///////////////////////////////////////////////////
  int n_centers;
  int n_outliers;
  int coreset_size;
  int n_repeats;
  double upper_bound;
  string dataset;
  set<string> modes;
  if (!parse_args(argc, argv, modes, n_centers, n_outliers, coreset_size, dataset, upper_bound, n_repeats)) {
    if (world.rank() == ROOT) {
      cerr << "usage: mpirun -np n_partitions ./mpi_exp [bg|bg2|km|rand|kmean++|kmean--]+ k t [coreset_size] dataset outliers_upper_bound [n_repeats = 1]" << endl;
    }
    return 0;
  }



  // load and distribute the dataset
  // do this in root process
  vector<int> actual_outliers;
  arma_vectors U;  // local dataset
  if (world.rank() == ROOT ) {
    cerr << "INFO: #processes = " << world.size() << endl;
    dataSet.name = dataset;
    dataSet.load();
    actual_outliers = util::add_outliers(dataSet.data, n_outliers, upper_bound);
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

  bool is_first = true;
  for (const string& mode : modes) {
    auto&& res = run_exp(world, ROOT, mode, U, dataSet, n_centers, n_outliers, coreset_size, actual_outliers, n_repeats);

    if (world.rank() == ROOT) {
      if (is_first) {
	cout << "algo";
	for (const auto& p : res) {
	  cout << "," << p.first;
	}
	cout << endl;
	is_first = false;
      }
      cout << mode;
      for (const auto& p : res) {
	cout << "," << p.second;
      }
      cout << endl;
    }
  }

  return 0;
}












