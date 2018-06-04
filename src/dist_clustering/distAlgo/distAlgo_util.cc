#include "distAlgo_util.h"
#include <mpi.h>

//**************** MPI:

void syncPrint(const std::string s, int id,
               int np) {  // Outputs with the order of processor ids.
  MPI_Barrier(MPI_COMM_WORLD);
  for (int i = 0; i < np; i++) {
    MPI_Barrier(MPI_COMM_WORLD);
    if (i == id) {
      printf("%d/%d: ", id, np);
      std::cout << s << std::endl;
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

void gatherArrsToRoot(double* recvbuf, int maxMsgSize, double* sendbuf,
                      int sendSize, int id, int n_nodes,
                      int root) {  // TODO:get rid of sendSize
  int ierr;
  UNUSED(ierr);
  ierr = MPI_Gather(sendbuf, maxMsgSize, MPI_DOUBLE, recvbuf, maxMsgSize,
                    MPI_DOUBLE, root, MPI_COMM_WORLD);
  if (id == root) {
  }
}

void printDoubleArray(double* arr, int length) {
  for (int i = 0; i < length; i++) printf("%f ", arr[i]);
  printf("\n");
}

void unpackRecvArray(double* array, int blockSize, ClusteringInput& newInput,
                     const int n_nodes) {
  //  std::ostringstream stream;
  //  arrToStream(stream,array,n_nodes*array[0]*array[1]);
  //  std::cout<<"unpack stream: "<<stream.str()<<std::endl;

  int index = 0;
  newInput.weights.resize(0);
  for (int node = 0; node < n_nodes; node++) {
    index = blockSize * node;
    int n_rows = array[index++];
    int n_cols = array[index++];
    for (int i = 0; i < n_rows; i++) {
      arma::vec row = arma::vec(&array[index], n_cols);
      index += n_cols;
      newInput.data.push_back(row);
    }
    newInput.weights.insert_rows(newInput.weights.n_elem,
                                 arma::vec(&array[index], n_rows));
  }
}

void prepareSendArray(double* array, int& sendSize,
                      const ClusteringOutput& output,
                      const ClusteringInput& input) {
  int index = 0;
  array[index++] = output.getNcenters() + output.getNoutliers();
  array[index++] = output.getNcols();
  // copying centers:
  util::copy_to_array(array + index, output.centers);
  index += output.getNcenters() * output.getNcols();
  // copying outliers:
  for (auto& ind : output.outliers_indices) {
    util::copy_vec_to_array(array + index, input.data.at(ind));
    index += output.getNcols();
  }
  //  index+=output.getNoutliers()*output.getNcols();
  // weights of centers:
  util::copy_vec_to_array(array + index, output.weights);
  assert((uint)output.getNcenters() == output.weights.n_elem);
  index += output.getNcenters();  //==output.weight.n_elem;
  // weights of outliers:
  std::fill(array + index, array + index + output.getNoutliers(), 1);
  index += output.getNoutliers();
  sendSize = index;
}

void allGatherCenters(double* recvbuf, double* sendbuf, int msgSize, int id,
                      int n_nodes, int root) {
  /*
   * int MPI_Allgather(const void *sendbuf, int sendcount, MPI_Datatype
   * sendtype,
   *                   void *recvbuf, int recvcount, MPI_Datatype recvtype,
   *                                     MPI_Comm comm)
   */
  MPI_Allgather(sendbuf, msgSize, MPI_DOUBLE, recvbuf, msgSize, MPI_DOUBLE,
                MPI_COMM_WORLD);
}
/******************** Basic general-purpose functions ************/
