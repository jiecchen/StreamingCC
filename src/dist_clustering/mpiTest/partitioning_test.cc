#include "../algorithms/algo_util.h"
#include "../algorithms/simple_algo.h"
#include "../experiments/dataManager.h"

#include "../algorithms/kmean_minus_minus.h"
#include "../distAlgo/partitioning.h"

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

#include "../lest.hpp"

#include <mpi.h>

const lest::test specification[] = {
    /*
    CASE("PartitionClustering") {
      ClusteringInput input;
      ClusteringOutput output;
      input.n_centers = 10;
      for (int i = 0; i < 100; ++i) {
        arma::vec v(5);
        v.fill(i);
        input.data.push_back(v);
      }
      input.n_outliers = 0;

      // ************ MPI **************
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

      syncPrint("Hello world",id, np);
      // ************ MPI **************


      PartitionClustering algo;
      RandomClustering randomClustering1,randomClustering2;
      ArmaKmeansClustering kmeans1,kmeans2;
      kmeans1.n_iter=10;
      kmeans1.verbose=false;
      kmeans2.n_iter=10;
      kmeans2.verbose=false;
      algo.localClusteringAlgo=
          &kmeans1;
//          &randomClustering1;
      algo.globalClusteringAlgo=
          &kmeans2;
//          &randomClustering2;
      algo.n_nodes=np;
      algo.node_id=id;
      algo.root=root;
      algo.n_local_centers=input.n_centers;

      algo.perform(input, output);
      //  Terminate MPI.
      ierr = MPI_Finalize ( );

      if(id==root){
        EXPECT( output.centers.size() == input.n_centers );
        EXPECT( output.outliers_indices.size() == input.n_outliers );
      }
    },
     */

    CASE("PartitionClustering 16points"){

        int ierr;
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

syncPrint("Hello world", id, np);

ClusteringInput input;
ClusteringOutput output;
DataSet dataSet;
dataSet.name = "16points";
dataSet.folderName = "./data/";
dataSet.load();
input.data = dataSet.data;
input.n_centers = 3;
input.n_outliers = 4;

PartitionClustering algo;
RandomClustering randomClustering;
ArmaKmeansClustering kmeans;
KMeanMinusMinus kmmmm;
IdentityClustering ic1, ic2;

kmeans.n_iter = 100;
kmeans.verbose = false;
algo.localClusteringAlgo = &ic1;
algo.n_nodes = np;
algo.node_id = id;
algo.root = root;
algo.n_local_centers = input.n_centers;
algo.globalClusteringAlgo = &kmmmm;
algo.perform(input, output);
if (id == root) {
  EXPECT(output.centers.size() == input.n_centers);
  EXPECT(output.outliers_indices.size() == input.n_outliers);
}

ierr = MPI_Finalize();
}
,

/*
CASE("unpackRecvArray"){
  //unpackRecvArray(double* array,const ClusteringOutput& output,const
ClusteringInput& input);
  int n_nodes=1;
  double array[]={2,3,
      1,2,3,
      4,5,6,
      10,20};
  ClusteringInput newInput;
  unpackRecvArray(array,newInput,n_nodes);
  bool equal=true;
  int index=0;
  EXPECT(array[0]==newInput.data.size());
  for(uint i=0;i<newInput.data.size();i++){
    equal &= (array[1]==newInput.data.at(i).n_elem);
//        std::cout<<equal<<" "<<newInput.data.at(i)<<std::endl;
    for(uint j=0;j<newInput.data.at(i).n_elem;j++){
      index=i*array[1]+j+2;
      equal &= (newInput.data.at(i)(j)==array[index]);
//          std::cout<<equal<<" "<<newInput.data.at(i)(j)<<"
"<<array[index]<<std::endl;
    }
  }
//      std::cout<<std::endl;
  for(uint i=0;i<newInput.data.size();i++){
    equal &= (newInput.weights[i]==array[++index]);
//        std::cout<<equal<<" "<<newInput.weights[i]<<"
"<<array[index]<<std::endl;

  }
  EXPECT(equal);
}
 */
/*
CASE("ArmaKmeansClustering") {
ClusteringInput input;
ClusteringOutput output;
input.n_centers = 10;
for (int i = 0; i < 100; ++i) {
  arma::vec v(5);
  v.fill(i);
  input.data.push_back(v);
}
input.n_outliers = 5;

ArmaKmeansClustering algo;
algo.n_iter=10;
algo.perform(input, output);

EXPECT( output.centers.size() == input.n_centers );
EXPECT( output.outliers_indices.size() == input.n_outliers );
},
CASE("ArmaKmeansClustering") {
ClusteringInput input;
ClusteringOutput output;
DataSet dataSet;
dataSet.name="16points";
dataSet.load();
input.n_centers = 4;
input.n_outliers = 0;
input.data=dataSet.data;
ArmaKmeansClustering algo;
algo.n_iter=10;
algo.perform(input, output);

EXPECT( output.centers.size() == input.n_centers );
EXPECT( output.outliers_indices.size() == input.n_outliers );
}
 */
/*
CASE("void prepareSendArray"){
  //void prepareSendArray(double* array,int&sendSize,
  //      const ClusteringOutput& output,const ClusteringInput& input);

  ClusteringInput input;
  ClusteringOutput output;
  ArmaKmeansClustering algo;
  algo.n_iter=10;


  DataSet dataSet;
  dataSet.name="16points";
  dataSet.folderName="./data/";
  dataSet.load();
  input.data=dataSet.data;
  input.n_centers=3;
  input.n_outliers=4;
  //a perfect solver gives this output
  arma::vec v1={2,2};output.centers.push_back(v1);
  arma::vec v2={102,2};output.centers.push_back(v2);
  arma::vec v3={2,102};output.centers.push_back(v3);


  std::vector<int> ov={12,13,14,15};
  output.outliers_indices=ov;

  output.weights={4,4,4};
  double* sendbuf=(double*)malloc(sizeof(double)*algo.maxMsgSize(input));
  int sendSize;
  //      std::cout<<output.getNcenters

  prepareSendArray(sendbuf,sendSize,output,input);
//      arrToStream(std::cout,sendbuf,algo.maxMsgSize(input));
  double
answer[]={7,2,2,2,102,2,2,102,100,100,100,104,104,100,104,104,4,4,4,1,1,1,1};
  for(int i=0;i<sendSize;i++)
  {
    EXPECT(sendbuf[i]==answer[i]);
  }
}
 */
}
;

int main() { return lest::run(specification); }
