#ifndef _UTILS_H
#define _UTILS_H

#include "../algorithms/algo_util.h"
#include "../algorithms/util.h"

#include <math.h>
#include <string.h>
#include <cstdlib>
#include <iostream>

// Macros for the whole project:

#include "../experiments/erfanUtil.h"

// MPI:

void syncPrint(const std::string s, int id,
               int np);  // Outputs with the order of processor ids.

void printDoubleArray(double* arr, int length);
void gatherArrsToRoot(double* recvbuf, int maxMsgSize, double* sendbuf,
                      int sendSize, int id, int n_nodes, int root);

void prepareSendArray(double* array, int& sendSize,
                      const ClusteringOutput& output,
                      const ClusteringInput& input);
void unpackRecvArray(double* array, int blockSize, ClusteringInput& newInput,
                     const int n_nodes);

void allGatherCenters(double* recvbuf, double* sendbuf, int msgSize, int id,
                      int n_nodes, int root);

#endif
