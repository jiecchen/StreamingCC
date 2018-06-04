#include "../algorithms/succ_sampling.h"
#include "../algorithms/kmean_plus_plus.h"
#include "../algorithms/simple_algo.h"
#include "../experiments/dataManager.h"
#include "../algorithms/algo_util.h"
#include "../algorithms/timer.h"
#include "../algorithms/kmean_minus_minus.h"
#include "../algorithms/local_search.h"

#include <iostream>
#include <set>
#include <vector>
#include <cstdlib>


#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"


using namespace std;

ClusteringInput to_weighted_input(const vector<arma::vec>& U,
				  const ClusteringOutput& output,
				  const int n_centers,
				  const int n_outliers,
				  vector<int>& indices) {
  indices =  output.centers_indices;
  ClusteringInput input;
  input.n_centers = n_centers;
  input.n_outliers = n_outliers;
  input.data = output.centers;
  vector<double>&& ws =  util::arma_vec_to_vector(output.weights);
  for (const int i : output.outliers_indices) {
    input.data.push_back(U[i]);
    indices.push_back(i);
    ws.push_back(1);
  }
  input.weights = arma::vec(ws);
  if (input.weights.size() != input.data.size()) {
    throw runtime_error("to_weighted_input: size error");
  }
  return input;
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
  set<string> valid_modes{"bg", "bg2", "rand", "kmean++"};
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
  if (modes.count("rand") > 0 || modes.count("kmean++") > 0) {
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


map<string, double> run_exp(const string& mode,
			    const int n_centers,
			    const int n_outliers,
			    const vector<arma::vec>& U,
			    const int n_repeats,
			    const int coreset_size,
			    const double upper_bound,
			    const string agg_mode) {
  if (n_repeats <= 0) {
    throw runtime_error("run_exp: n_repeats <= 0");
  }

  map<string, vector<double>> res;
  ClusteringInput input;
  input.n_centers = n_centers;
  input.n_outliers = n_outliers;
  input.data = U;

  auto&& actual_outliers = util::add_outliers(input.data, n_outliers, upper_bound);
  
  for (int i = 0; i < n_repeats; ++i) {
    Timer timer;
    ClusteringOutput output;
    if (mode == "bg") {
      SuccSampling algo;
      //TODO: tune those parameters
      algo.setAlpha(2);
      algo.setBeta(0.3);
      algo.setGamma(5);
      algo.perform(input, output);
      res["coreset_size"].push_back(output.centers.size() + output.outliers_indices.size());
      res["pre-recall"].push_back(util::recall(actual_outliers, output.outliers_indices));
    } else if (mode == "bg2") {
      SuccSampling2 algo;
      //TODO: tune those parameters
      algo.setAlpha(2);
      algo.setBeta(0.7);
      algo.setGamma(5);
      algo.perform(input, output);
      res["coreset_size"].push_back(output.centers.size() + output.outliers_indices.size());
      res["pre-recall"].push_back(util::recall(actual_outliers, output.outliers_indices));
      
    } else if (mode == "rand") {
      input.n_centers = coreset_size;
      RandClustering algo;
      algo.perform(input, output);
      res["coreset_size"].push_back(coreset_size);
      res["pre-recall"].push_back(util::recall(actual_outliers, output.centers_indices));
      
    } else if (mode == "kmean++") {
      input.n_centers = coreset_size;
      //TODO: update the implementation, it has bugs
      KMeanPP algo;
      algo.perform(input, output);
      res["coreset_size"].push_back(coreset_size);
      res["pre-recall"].push_back(util::recall(actual_outliers, output.centers_indices));
      
    } else if (mode == "identical") {
      IdentityClustering algo;
      algo.perform(input, output);
      res["coreset_size"].push_back(input.data.size());
      res["pre-recall"].push_back(1);
      
    } else {
      throw runtime_error("run_exp: invalid mode!");
    }

    // record running time
    res["tm"].push_back(timer.get_time());
    
    // construct weighted dataset
    vector<int> corr_indices;  // corresponding indices
    ClusteringInput&& input2 = to_weighted_input(input.data, output, n_centers, n_outliers, corr_indices);

    // do re-clustering
    FastWeightedKMeanMM fwmmm;
    if (mode == "identical") {
      fwmmm.setKmeanppSeeding(false);
    }
    ClusteringOutput output2;
    fwmmm.perform(input2, output2);
    
    // calc costs
    vector<int> outliers_indices;
    for (const int i : output2.outliers_indices) {
      outliers_indices.push_back(corr_indices[i]);
    }
    res["l1"].push_back(cost1(input.data, output2.centers, outliers_indices));
    res["l2"].push_back(cost2(input.data, output2.centers, outliers_indices));
    res["recall"].push_back(util::recall(actual_outliers, outliers_indices));
    res["precision"].push_back(util::precision(actual_outliers, outliers_indices));
  }
  map<string, double> final_res;
  
  // calculate the average
  if (agg_mode == "avg") {
    for (const auto& p : res) {
      final_res[p.first] = util::avg(p.second);
    }
  } else { // median
    for (const auto& p : res) {
      final_res[p.first] = util::median(p.second);
    }
  }
  return final_res;
}


int main(int argc, char* argv[]) {
  ///////////////////////////////////////////////////
  int n_centers;
  int n_outliers;
  int coreset_size;
  int n_repeats;
  double upper_bound;
  string dataset;
  set<string> modes;
  if (!parse_args(argc, argv, modes, n_centers, n_outliers, coreset_size, dataset, upper_bound, n_repeats)) {
    cerr << "usage: ./exp [bg|bg2|rand|kmean++]+ k t [coreset_size] dataset outliers_upper_bound [n_repeats = 1]" << endl;
    return 0;
  }


  DataSet ds;
  ds.name = dataset;
  ds.load();

  
  modes.insert("identical");
  bool is_first = true;
  for (const string& m : modes) {
    auto&& res = run_exp(m, n_centers, n_outliers, ds.data, n_repeats, coreset_size, upper_bound, "avg");
    if (is_first) {
      cout << "algo";
      for (const auto& p : res) {
	cout << "," << p.first;
      }
      cout << endl;
      is_first = false;
    }

    cout << m;
    for (const auto& p : res) {
      cout << "," << p.second; 
    }
    cout << endl;
  }

  
  return 0;
}
