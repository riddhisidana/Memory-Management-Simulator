#ifndef BUDDY_H
#define BUDDY_H

#include <cstddef>
#include <stddef.h>
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <iomanip>
#include <sstream>
#include <limits>
#include <fstream>
#include <cstring>
#include <ctime>
using namespace std;

#define MIN_BLOCK_SIZE 32
#define MIN_BLOCK_POWER 5
#define MAX_LEVELS 6

struct BuddyBlock
{
    size_t size;
    bool is_free;
    int process_id;
    BuddyBlock *next;
    BuddyBlock *prev;
};

// Functions for buddy
void buddy_init(size_t total_size);

void *buddy_allocate(size_t size, int process_id);

bool buddy_deallocate(int process_id);

void buddy_display();

void buddy_stats();

void buddy_dump();

void buddy_reset();

void buddy_shutdown();

int get_level(size_t size);

size_t get_size_for_level(int level);

size_t next_power_of_two(size_t n);

#endif
