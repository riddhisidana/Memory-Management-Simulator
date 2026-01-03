#ifndef CACHE_TYPES_H
#define CACHE_TYPES_H

#include <vector>
using namespace std;

enum ReplacementPolicy
{
    FIFO
};

struct CacheLine
{
    bool valid;
    unsigned long tag;
    int fifo_counter;
};

// Structure for a single cache level
struct CacheLevel
{
    vector<vector<CacheLine>> sets;
    int level_number; // 1=L1, 2=L2, 3=L3
    int cache_size_kb;
    int block_size;
    int associativity;
    int num_sets;

    // Statistics for this level
    long long accesses;
    long long hits;
    long long misses;

    // FIFO counter
    int global_fifo_counter;
};

#endif