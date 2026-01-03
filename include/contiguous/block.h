#ifndef BLOCK_H
#define BLOCK_H

#include <cstddef>
#include <stddef.h>
using namespace std;

typedef struct Block
{
    size_t start;
    size_t size;
    bool is_free;
    int process_id;

    Block *prev;
    Block *next;

} Block;

#endif