#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <iostream>
#include <map>
#include <string>
#include <iomanip>
#include <sstream>
#include <limits>
#include <fstream>
#include <ctime>
#include "block.h"

using namespace std;

enum class AllocationStrategy
{
    first_fit,
    best_fit,
    worst_fit
};

void init_memory(size_t total_size, AllocationStrategy strategy);

Block *allocate(size_t size, int process_id);

void deallocate(int process_id);

void display_memory();

double calculate_external_fragmentation();

void shutdown_memory();

void set_strategy(AllocationStrategy strategy);

AllocationStrategy get_strategy();

void display_processes();

void compact_memory();

void display_memory_bar();

void display_stats();

void reset_stats();

#endif