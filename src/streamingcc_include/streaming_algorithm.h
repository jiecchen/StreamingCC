// Copyright 2016 Jiecao Chen
#ifndef SRC_STREAMINGCC_INCLUDE_STREAMING_ALGORITHM_H_
#define SRC_STREAMINGCC_INCLUDE_STREAMING_ALGORITHM_H_

#include <cstdint>
#include <vector>
#include <utility>

namespace streamingcc {


// All streaming algorithms for integer stream should be placed
// under this namespace. All classes defined in this namespace, should
// be named with "Int" suffix.
namespace integer {

// Interface for streaming algorithms for unweighted integer data stream.
class StreamingAlgorithmInt {
 public:
  // Processes an item
  virtual void ProcessItem(const uint32_t item) = 0;
  // Processes a sequence of items
  virtual void ProcessItems(const std::vector<uint32_t>& items) {
    // TODO(jiecchen): run it in parallel
    for (const uint32_t item : items) {
      ProcessItem(item);
    }
  }
  virtual ~StreamingAlgorithmInt() {}
};

// Interface for streaming algorithms for weighted integer data stream.
class StreamingAlgorithmWeightedInt {
 public:
  // Processes an item
  virtual void ProcessItem(const uint32_t item, const double weight) = 0;
  // Processes a sequence of items
  virtual void ProcessItems(const std::vector<std::pair<uint32_t, double>>&
                            items_weights) {
    // TODO(jiecchen): run it in parallel
    for (const auto& itm_wt : items_weights) {
      ProcessItem(itm_wt.first, itm_wt.second);
    }
  }
  virtual ~StreamingAlgorithmWeightedInt() {}
};

}  // namespace integer
}  // namespace streamingcc

#endif  // SRC_STREAMINGCC_INCLUDE_STREAMING_ALGORITHM_H_














