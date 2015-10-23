/**
 * `murmurhash.h' - murmurhash
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 *
 * modified by Jiecao Chen <chenjiecao@gmail.com>
 */

#ifndef MURMURHASH_H
#define MURMURHASH_H 1

#include <stdint.h>
#include "config.h"

#define MURMURHASH_VERSION "0.0.3"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns a murmur hash of `key' based on `seed'
 * using the MurmurHash3 algorithm
 */

uint32_t murmurhash (const ItemType *key, uint32_t seed, uint32_t len=sizeof(ItemType));

#ifdef __cplusplus
}
#endif

#endif










