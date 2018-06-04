#include "partitioning.h"
#include <cassert>

void PartitionClustering::perform(const ClusteringInput &input,
    ClusteringOutput &output) {
  ClusteringInput myInputSlice;
  ClusteringOutput myOutput;
  int length = input.data.size() / n_nodes;
  auto first = input.data.begin() + node_id * length;
  auto last = input.data.begin() +
    std::min((int)((node_id + 1) * length), (int)(input.data.size()));
  myInputSlice.data = std::vector<arma::vec>(first, last);
  myInputSlice.n_centers = n_local_centers;
  myInputSlice.n_outliers = input.n_outliers / n_nodes;

  if(onlyCenters)
    myInputSlice.n_outliers=0;
  else
    myInputSlice.n_outliers = input.n_outliers / n_nodes;
  localClusteringAlgo->perform(myInputSlice, myOutput);

  myOutput.make_sure_labels_weights_outliers(myInputSlice);
  myOutput.setNcols(myInputSlice.getNcols());
  int maxMsgSize = localClusteringAlgo->maxMsgSize(myInputSlice);
  int sendSize = -1;
  //=myOutput.centers.size()*(2+myOutput.centers.at(0).n_elem)+2;
  double *sendbuf = (double *)malloc(maxMsgSize * sizeof(double));
  double *recvbuf = NULL;
  UNUSED(recvbuf);  // These extra things is to avoid warnings
  //  std::cout<<"sendSize="<<sendSize<<" , maxMsgSize="
  //        <<localClusteringAlgo->maxMsgSize(myInputSlice)<<" "<<maxMsgSize<<"
  //        "<<myInputSlice.getNrows()
  //        <<" "<<myInputSlice.n_centers<<"
  //        "<<myInputSlice.n_outliers<<std::endl;
  prepareSendArray(sendbuf, sendSize, myOutput, myInputSlice);
  //  std::cout<<"sendSize="<<sendSize<<" , maxMsgSize="
  //          <<localClusteringAlgo->maxMsgSize(myInputSlice)<<"
  //          "<<maxMsgSize<<" "<<myInputSlice.getNrows()
  //          <<" "<<myInputSlice.n_centers<<"
  //          "<<myInputSlice.n_outliers<<std::endl;
  assert(sendSize <= localClusteringAlgo->maxMsgSize(myInputSlice));
  if (node_id == root) {
    recvbuf = (double *)malloc(n_nodes * maxMsgSize * sizeof(double));
  }
  gatherArrsToRoot(recvbuf, maxMsgSize, sendbuf, maxMsgSize, node_id, n_nodes,
      root);  // MPI_Gather
  free(sendbuf);
  // combine them
  if (node_id == root) {  // We are in root and we can combine the points
    ClusteringInput newInput;
    ClusteringOutput newOutput;
    newInput.n_centers = input.n_centers;
    if(onlyCenters)
      newInput.n_outliers=0;
    else
      newInput.n_outliers = input.n_outliers;
    //    arrToStream(std::cout,recvbuf,n_nodes*maxMsgSize);
    unpackRecvArray(recvbuf, maxMsgSize, newInput, n_nodes);
    free(recvbuf);
    output.num_intermediate_centers = newInput.getNrows();
    assert(newInput.weights.size() > 0);
    newInput.n_outliers = input.n_outliers;
    globalClusteringAlgo->perform(newInput, newOutput);
    output.centers = newOutput.centers;
    //    util::find_indices(output.outliers_indices,input.data,newInput.data,newOutput.outliers_indices);
  }
}
