#include "../algorithms/algo_util.h"
#include "../algorithms/kmean_minus_minus.h"
#include "../algorithms/simple_algo.h"
#include "../distAlgo/distAlgo_util.h"
#include "../distAlgo/multiRoundSampling.h"
#include "../distAlgo/partitioning.h"
#include "../experiments/dataManager.h"

#include <mpi.h>
#include <map>

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

#include "../lest.hpp"

const lest::test specification[] = {

    CASE("allgather"){int ierr;
UNUSED(ierr);
int np;
int id;
const int root = 0;  // root id aka coordinator aka master process
UNUSED(root);

ierr = MPI_Init(NULL, NULL);

//  The number of processes:
ierr = MPI_Comm_size(MPI_COMM_WORLD, &np);

//  My process ID:
ierr = MPI_Comm_rank(MPI_COMM_WORLD, &id);

int msgSize = 4;

double* sendbuf = (double*)malloc(msgSize * sizeof(double));
for (int i = 0; i < msgSize; i++) sendbuf[i] = (id + 1) * (i + 1);
double* recvbuf = (double*)malloc(np * msgSize * sizeof(double));
allGatherCenters(recvbuf, sendbuf, msgSize, id, np, root);

//  Terminate MPI.
ierr = MPI_Finalize();

//    util::arrToStream(std::cout,recvbuf,np*msgSize);std::cout<<std::endl;
for (int i = 0; i < np; i++)
  for (int j = 0; j < msgSize; j++)
    EXPECT(recvbuf[i * msgSize + j] == (i + 1) * (j + 1));
free(sendbuf);
free(recvbuf);
}
/*
CASE("multiRoundSampling"){

int ierr;
UNUSED(ierr);
int np;
int id;
const int root=0;//root id aka coordinator aka master process
UNUSED(root);

ierr = MPI_Init (NULL, NULL);

//  The number of processes:
ierr = MPI_Comm_size ( MPI_COMM_WORLD, &np );

//  My process ID:
ierr = MPI_Comm_rank ( MPI_COMM_WORLD, &id );

DataSet dataSet;
dataSet.name="16points";
dataSet.folderName="./data/";
dataSet.load();
ClusteringInput input;
ClusteringOutput output;
input.data=dataSet.data;
input.n_centers=4;
input.n_outliers=0;

MultiRoundSamplingClustering mrs;
mrs.node_id=id;
mrs.root=root;
mrs.n_local_centers=input.n_centers;
mrs.n_rounds=log(input.getNrows());
mrs.squareDist=true;

mrs.perform(input,output);
//    for(uint i=0;i< output.centers.size();i++)
//      output.centers[i].print("i: ");
//    util::print_vector(output.centers);
//  Terminate MPI.
ierr = MPI_Finalize ( );
EXPECT(output.centers.size()==mrs.n_local_centers*mrs.n_rounds*np);
}
*/
/*
    CASE("strictMultiRoundSampling"){

      int ierr;
      UNUSED(ierr);
      int np;
      int id;
      const int root=0;//root id aka coordinator aka master process
      UNUSED(root);

      ierr = MPI_Init (NULL, NULL);

      //  The number of processes:
      ierr = MPI_Comm_size ( MPI_COMM_WORLD, &np );

      //  My process ID:
      ierr = MPI_Comm_rank ( MPI_COMM_WORLD, &id );

      DataSet dataSet;
      dataSet.name="16points";
      dataSet.folderName="./data/";
      dataSet.load();
      ClusteringInput input;
      ClusteringOutput output;
      input.data=dataSet.data;
      input.n_centers=4;
      input.n_outliers=0;

      TwoPhaseClustering smrs;
      smrs.node_id=id;
      smrs.root=root;

      MultiRoundSamplingClustering mrs;
      mrs.node_id=id;
      mrs.root=root;
      mrs.n_local_centers=input.n_centers;
      mrs.n_rounds=log(input.getNrows());
      mrs.squareDist=true;

      smrs.firstAlgo=&mrs;
      KMeanMinusMinus kmmm;
      smrs.reclusteringAlgo=&kmmm;
      smrs.perform(input,output);


      //    for(uint i=0;i< output.centers.size();i++)
  //      output.centers[i].print("i: ");
  //    util::print_vector(output.centers);
      //  Terminate MPI.
      ierr = MPI_Finalize ( );
      EXPECT(output.centers.size()==input.n_centers);
    }
*/
}
;

int main() { return lest::run(specification); }
