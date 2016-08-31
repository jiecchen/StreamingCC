// Copyright 2016 Jiecao Chen
#ifndef SRC_STREAMINGCC_INCLUDE_STREAMING_ALGORITHM_H_
#define SRC_STREAMINGCC_INCLUDE_STREAMING_ALGORITHM_H_

#include <vector>
#include <utility>

#if (_LIBCPP_VERSION)
  #include <cstdint>
#else
  #include <tr1/cstdint>
#endif

namespace streamingcc {


// Template of streaming algorithms for unweighted data stream.
// Note that T should be support by std::hash<T>, otherwise, consider
// defining your own hash function to map each item to uint32_t and use
// StreamingAlgorithmInt.
// TODO(jiecchen): add type checking.
template <typename T>
class StreamingAlgorithm {
  // Processes an item
  virtual void ProcessItem(const T& item) = 0;
  // Processes a sequence of items
  virtual void ProcessItems(const std::vector<T>& items) {
    // TODO(jiecchen): run it in parallel
    for (const auto& item : items) {
      ProcessItem(item);
    }
  }
  virtual ~StreamingAlgorithm() {}
};


// Template of streaming algorithms for weighted data stream.
// Same as in StreamingAlgorithm<T>, T should be supported by std::hash<T>.
template <typename T>
class StreamingAlgorithmWeighted {
 public:
  // Processes an item
  virtual void ProcessItem(const T& item, const double weight) = 0;
  // Processes a sequence of items
  virtual void ProcessItems(const std::vector<std::pair<T, double>>&
                            items_weights) {
    // TODO(jiecchen): run it in parallel
    for (const auto& itm_wt : items_weights) {
      ProcessItem(itm_wt.first, itm_wt.second);
    }
  }
  virtual ~StreamingAlgorithmWeighted() {}
};


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














