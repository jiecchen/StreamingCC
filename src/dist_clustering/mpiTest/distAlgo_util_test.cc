#include "../distAlgo/distAlgo_util.h"
#include "../algorithms/algo_util.h"
#include "../algorithms/simple_algo.h"
#include "../algorithms/util.h"
#include "../distAlgo/partitioning.h"

#include <mpi.h>

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

#include "../lest.hpp"

const lest::test specification[] = {

    CASE("void gatherArrsToRoot"){// void gatherArrsToRoot(double*
                                  // sendbuf,double* recvbuf, int sendSize,int
                                  // id, int n_nodes,int root)
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

ClusteringOutput co;
arma::mat Abase;
// endr indicates "end of row"
Abase << 0.555950 << 0.274690 << 0.540605 << 0.798938 << arma::endr << 0.108929
      << 0.830123 << 0.891726 << 0.895283 << arma::endr << 0.948014 << 0.973234
      << 0.216504 << 0.883152 << arma::endr << 0.023787 << 0.675382 << 0.231751
      << 0.450332 << arma::endr;
auto A = Abase * (id + 1);
std::vector<arma::vec> B;
util::convert_from_mat(B, A);
co.centers = B;

int sendSize = 0;
double *sendbuf = (double *)malloc(sendSize * sizeof(double));
double *recvbuf = NULL;
UNUSED(recvbuf);
ClusteringInput tempInput;
prepareSendArray(sendbuf, sendSize, co, tempInput);
if (id == root) {
  recvbuf = (double *)malloc(np * sendSize * sizeof(double));
}
//      std::ostringstream sendstream;
//      arrToStream(sendstream,sendbuf,sendSize);
//      syncPrint(sendstream.str(),id,np);

gatherArrsToRoot(recvbuf, sendSize, sendbuf, sendSize, id, np,
                 root);  // MPI_Gather
free(sendbuf);
bool equal = true;
if (id == root) {
  //        std::ostringstream recvstream;
  //        arrToStream(recvstream,recvbuf,np*sendSize);
  //        std::cout<<"recv: "<<recvstream.str()<<std::endl;

  for (int node = 0; node < np; node++)
    for (uint i = 0; i < Abase.n_rows; i++)
      for (uint j = 0; j < Abase.n_cols; j++) {
        int totalIndex =
            node * Abase.n_cols * Abase.n_rows + i * Abase.n_cols + j;
        equal &= (recvbuf[totalIndex] == Abase(i, j) * (node + 1));
      }
  free(recvbuf);
}
EXPECT(equal);

//  Terminate MPI.
ierr = MPI_Finalize();
}
,

    CASE("arrToStream") {
  double arr[] = {0.1, 02};
  std::ostringstream stream;
  util::arrToStream(stream, arr, 2);
  std::string st = "0.1,2";
  EXPECT(st == stream.str());
}
}
;

int main() { return lest::run(specification); }
