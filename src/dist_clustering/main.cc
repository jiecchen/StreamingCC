#include <mpi.h>
#include <time.h>
#include <algorithm>  // std::random_shuffle
#include <cstdlib>
#include <iostream>

#include <memory>
#include "algorithms/algoList.h"
#include "algorithms/algo_util.h"
#include "algorithms/local_search.h"
#include "algorithms/centeralizedMultiRoundSampling.h"
#include "algorithms/kmean_minus_minus.h"
#include "algorithms/simple_algo.h"
#include "algorithms/succ_sampling.h"
#include "distAlgo/distAlgo_util.h"
#include "distAlgo/multiRoundSampling.h"
#include "distAlgo/partitioning.h"
#include "experiments/dataManager.h"
#include "experiments/experimentUtil.h"

#define ARMA_DONT_USE_WRAPPER
#include "arma/include/armadillo"

using namespace std;
class ResultRow {
  public:
    double elapsedTime = 0;
    double n_centers = 0;
    double n_outliers = 0;
    double meansCost = 0;
    double mediansCost = 0;
    double maxMsgSize;
    double num_intermediate_centers;
    int count = 0;
    string str() const {
      std::stringstream ostream;

      ostream << elapsedTime / count << "," << meansCost / count << ","
        << mediansCost / count << "," << num_intermediate_centers / count
        //                                      <<","<<count
        ;
      return ostream.str();
    }
};
void writeFile(string fileName);

void usage(char* program_name) {
  printf("usage: ./%s  -d dataSetName -o numOutliers -c numCenters\n",
      program_name);
  printf("example: ./%s -d r1 -o 0 -c 1 \n", program_name);
  /*    printf("Explanation1. \n");
        printf("Explanation2. \n"); */
  exit(-1);
}

void parseArgument(Experiment& experiment, int argc, char* argv[]);

void initDataSetStatic();

int main(int argc, char* argv[]) {
  Experiment experiment;
  if (argc < 1) {
    usage(argv[0]);
  } else {
    parseArgument(experiment, argc, argv);
  }
  /* input argument parsing*/
  bool algVerbose=false;
  // MPI init section
  int ierr;
  int np;
  int id;
  const int root = 0;  // root id aka coordinator aka master process
  double startTime = 0;
  double endTime = 0;

  ierr = MPI_Init(&argc, &argv);

  //  The number of processes:
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &np);

  //  My process ID:
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &id);
  if (id == root) {
    startTime = MPI::Wtime();
    if (experiment.verbose) {
      cout << "MPI initialized!" << endl;
      printf("np=%d nc=%d no=%d nr=%d", np, experiment.n_clusters,
          experiment.n_outliers, experiment.n_repeats);
      cout << endl;
    }
  }

  /* prepare/load dataset... */

  DataSet dataSet;
  dataSet.name = experiment.dataSetName;
  dataSet.folderName = experiment.address;
  //  cout<<"name: "<<dataSet.name<<endl;
  dataSet.load();
  if (id == root) {
    if (experiment.verbose) {
      printf("Loading dataset: ");
      cout << dataSet.name << " with dimensions ( " << dataSet.data.size()
        << "," << dataSet.data.at(0).n_elem << " )\n"
        << endl;
    }
  }
  int min_cluster = experiment.n_clusters;
  int max_cluster = experiment.n_clusters + 1;
  int min_outlier = experiment.n_outliers;
  int max_outlier = experiment.n_outliers + 1;
  int step_outlier = 1;
  for (experiment.n_clusters = min_cluster; experiment.n_clusters < max_cluster;
      experiment.n_clusters++) {
    for (experiment.n_outliers = min_outlier;
        experiment.n_outliers < max_outlier;
        experiment.n_outliers += step_outlier) {
      // ************************************************ ALGORITHMS:
      vector<unique_ptr<ClusteringAlgo>> algos;
      for (int rr = 0; rr < experiment.n_repeats; rr++) {
        // Adding Centralized algorithms:

//        addAlgo(algos, new TwoPhaseClustering(
//              new MultiRoundSamplingClustering(
//                log(dataSet.data.size()), true, np, id, root,
//                experiment.n_clusters),
//              new FastWeightedKMeanMM(), np, id, root));
//double alpha=2;
//double beta=0.3;
//int n_clusters=experiment.n_clusters;
//        addAlgo(algos, new PartitionClustering(new SuccSampling(alpha, beta),
//              new WeightedLocalSearchAlgo(), np, id,
//              root, n_clusters));


//               addDistComp(algos,np,id,root,experiment.n_clusters,dataSet.data.size());
               addSSsweep(algos,np,id,root,experiment.n_clusters);
               addRandPartSweep(algos,np,id,root,experiment.n_clusters,experiment.n_outliers,dataSet.data.size());
        //        addPPsweep(algos,np,id,root,experiment.n_clusters,dataSet.data.size());
        //        addThirdsweep(algos,np,id,root,experiment.n_clusters,dataSet.data.size());
//                                addCenAlgos(algos);
//              addAlgosExp2(algos,np,id,root,experiment.n_clusters,dataSet.data.size());
        //        addAlgo(algos,new RandomClustering());

        continue;
      }

      //*********************** :ALGORITHMS

      if (experiment.verbose && id == root) printf("Experiments started.\n");
      //  syncPrint("Hello, world",id,np);
      ClusteringInput input(dataSet.data, experiment.n_clusters,
          experiment.n_outliers);
      ClusteringOutput* output = new ClusteringOutput[algos.size()];
      if (id == root) 
        cout << "n_algos= " << algos.size() << endl;

      for (uint i = 0; i < algos.size(); i++) {
        MPI_Barrier(MPI_COMM_WORLD);   // everybody starts together
        if (id < algos[i]->n_nodes) {  // for now all distAglos need to use all
          // the machines for MPI_barrier to work
          if (id == root && experiment.verbose)
            cout << "Running " << i << " :"
              << algos[i]->getName() + "(" + algos[i]->paramStr() + ")"
              << endl;
          std::random_shuffle(input.data.begin(), input.data.end());
          algos[i]->verbose=algVerbose;
//            algVerbose;
          double algStartTime = MPI::Wtime();
          algos[i]->perform(input, output[i]);
          output[i].elapsedTime =
            (MPI::Wtime() -
             algStartTime);  // only root will use read this time.
        }
      }

      if (id == root) {
        std::map<string, ResultRow> sumResults;
        cout << "Done Running!" << endl;
        time_t timer;
        time(&timer);
        auto timeinfo = localtime(&timer);
        std::string strTime = std::asctime(timeinfo);
        strTime = util::space2underscore(strTime);
        std::stringstream ostreamFileName;
        ostreamFileName << dataSet.name << "_" << input.getNrows() << "_"
          << input.getNcols() << "_" << algos.size() << "_"
          << input.n_centers << "_" << input.n_outliers << "_"
          << experiment.n_repeats << "_" << np << "_" << strTime;
        string CSVName = ostreamFileName.str() + ".csv";
        string TXTName = ostreamFileName.str() + ".txt";
        ofstream fout(experiment.outputFolder+"all_" + CSVName);

        std::vector<std::ostream*> outs;
        outs.push_back(&fout);
        //    outs.push_back(&cout);
        for (auto& outStream : outs) {
          (*outStream) << "AlgName,elapsedTime,ncenters,noutliers,measCost,"
            "mediansCost,maxMsgSize,num_intermediate_centers"
            << endl;  // Means<<medians<<centersObj

          for (uint i = 0; i < algos.size(); i++) {
            output[i].labels.clear();  // This way we only care about the
            // centers
            output[i].outliers_indices.clear();
            output[i].weights.clear();
            output[i].make_sure_labels_weights_outliers(input);

            std::vector<double> min_dist;  // probably not used
            std::vector<int>
              labels;  // The costs are for just centers not output labels
            util::compute_nearest(input.data, output[i].centers, labels,
                min_dist);
            double meansCost = util::cost_outliers(
                input.data, output[i].centers, output[i].outliers_indices,
                labels, util::CostType::k_mean);
            double mediansCost = util::cost_outliers(
                input.data, output[i].centers, output[i].outliers_indices,
                labels, util::CostType::k_median);
            string entry =
              algos[i]->getName() + "(" + algos[i]->paramStr() + ")";
            sumResults[entry].elapsedTime += output[i].elapsedTime;
            //        sumResults[entry].n_centers=          output[i].n_centers;
            //        sumResults[entry].n_outliers= output[i].n_outliers;
            sumResults[entry].meansCost += meansCost;
            sumResults[entry].mediansCost += mediansCost;
            sumResults[entry].maxMsgSize = algos[i]->maxMsgSize(input);
            sumResults[entry].num_intermediate_centers +=
              output[i].num_intermediate_centers;
            sumResults[entry].count++;

            (*outStream) << algos[i]->getName() + "(" + algos[i]->paramStr() +
              ")"
              << "," << output[i].elapsedTime << ","
              << output[i].getNcenters() << ","
              << output[i].getNoutliers() << "," << meansCost << ","
              << mediansCost << "," << algos[i]->maxMsgSize(input)
              << "," << output[i].num_intermediate_centers << endl;
          }
          ofstream avgout(experiment.outputFolder+"avg_" + CSVName);
          avgout << "algname,time,mean,median,in_centers" << endl;
          for (auto const& x : sumResults) {
            avgout << x.first                // string (key)
              << ',' << x.second.str()  // string's value
              << std::endl;
          }

          endTime = MPI::Wtime();
          if (experiment.verbose) {
            cout << "MPI finalized!" << endl;
            printf("ElapsedTime=%f\nResults:\n", endTime - startTime);
            //        string command="./pretty_tsv.sh "+CSVName+" >"+TXTName;
            //        cout<<command<<endl;
            //        ierr=system(command.c_str());
            writeFile(TXTName);
          }
        }
        std::cout << experiment.outputFolder<<"XX_"<<CSVName << endl;
      }
      delete[] output;
    }
  }

  //  Terminate MPI.
  ierr = MPI_Finalize();
  UNUSED(ierr);
  //  VARi(ierr);

  return 0;
}

void writeFile(string fileName) {
  std::string getcontent;
  std::ifstream openfile(fileName);
  if (openfile.is_open()) {
    while (!openfile.eof()) {
      std::getline(openfile, getcontent);
      std::cout << getcontent << std::endl;
    }
  }
}

// TODO:
// move this to another file
void parseArgument(Experiment& experiment, int argc, char* argv[]) {
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-d") == 0)  // dataSet name
    {
      experiment.dataSetName = (argv[++i]);

    } else if (strcmp(argv[i], "-a") == 0)  // address
    {
      experiment.address = (argv[++i]);

    } else if (strcmp(argv[i], "-v") == 0)  // verbose
    {
      experiment.verbose = (true);
    } else if (strcmp(argv[i], "-r") == 0)  // verbose
    {
      experiment.n_repeats = (atoi(argv[++i]));
    } else if (strcmp(argv[i], "-f") == 0)  // verbose
    {
      experiment.outputFolder = argv[++i];
    }

    else if (strcmp(argv[i], "-o") == 0)  // number of outliers
    {
      experiment.n_outliers = (atoi(argv[++i]));
    } else if (strcmp(argv[i], "-c") == 0)  // number of centers
    {
      experiment.n_clusters = (atoi(argv[++i]));
    }
  }
}

void initDataSetStatic() {
  //  DataSet::withHeader.push_back("kdd");
  //  DataSet::kmeansCost["16points"]=128;
  //  DataSet::kmediansCost["spambase"]=0;
}
