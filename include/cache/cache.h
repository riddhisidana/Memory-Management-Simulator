#ifndef CACHE_H
#define CACHE_H

#include "cache_types.h"
#include <iostream>
#include <string>
using namespace std;

// Multilevel cache system
extern vector<CacheLevel> cache_levels;
extern int num_levels;

// Functions for multilevel cache
void init_multilevel_cache(int num_levels_to_create);
void configure_level(int level, int size_kb, int blk_size, int ways);
bool multilevel_access(unsigned long address);
void display_multilevel_stats();
void clear_multilevel_cache();

#endif