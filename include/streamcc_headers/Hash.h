/**
 * @file Hash.h
 * @author Joseph Werle <joseph.werle@gmail.com>
 * @modified by Jiecao Chen <chenjiecao@gmail.com>
 * 
 * Header file for `murmurhash.h' - murmurhash
 */

#ifndef __HASH_HPP__
#define __HASH_HPP__ 

#include <stdint.h>
#include "config.h"

#define MURMURHASH_VERSION "0.0.3"



/*!
 * @return a murmur hash of `key' based on `seed'
 * using the MurmurHash3 algorithm
 */
uint32_t murmurhash(const int *key, uint32_t seed);

#endif










