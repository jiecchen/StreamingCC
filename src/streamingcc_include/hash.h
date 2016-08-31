// Copyright 2016 Jiecao Chen

#ifndef SRC_STREAMINGCC_INCLUDE_HASH_H_
#define SRC_STREAMINGCC_INCLUDE_HASH_H_

#ifdef __clang__
  #include <tr1/cstdint>
#else
  #include <cstdint>
#endif

namespace streamingcc {

uint32_t murmurhash(uint32_t key, uint32_t seed);


}  // namespace streamingcc


#endif  // SRC_STREAMINGCC_INCLUDE_HASH_H_














