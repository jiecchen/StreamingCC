// Copyright 2016 Jiecao Chen
#ifndef SRC_STREAMINGCC_INCLUDE_STREAMING_ALGORITHM_H_
#define SRC_STREAMINGCC_INCLUDE_STREAMING_ALGORITHM_H_

#include <cstdint>

namespace streamingcc {

// Alias of the unsigned int type, as the type of the items.
using ItemType = uint32_t;

// All streaming algorithms for integer stream should be placed
// under this namespace. All classes defined in this namespace, should
// be named with "Int" suffix.
namespace integer {

// Interface for streaming algorithms for unweighted integer data stream.
class StreamingAlgorithmInt {
 public:
  // Processes an item
  virtual void ProcessItem(const ItemType item) = 0;
  virtual ~StreamingAlgorithmInt() {}
};

// Interface for streaming algorithms for weighted integer data stream.
class StreamingAlgorithmWeightedInt {
 public:
  // Processes an item
  virtual void ProcessItem(const ItemType item, const double weight) = 0;
  virtual ~StreamingAlgorithmWeightedInt() {}
};

}  // namespace integer
}  // namespace streamingcc

#endif  // SRC_STREAMINGCC_INCLUDE_STREAMING_ALGORITHM_H_














