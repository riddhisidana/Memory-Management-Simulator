# Design and Implementation: Memory Management Simulator

This document outlines the architectural design, data structures, and algorithms implemented in the Memory Management Simulator.

## Architectural Overview

The project follows a modular architecture, where each memory management technique is isolated into its own component. This allows for easy extension and testing of individual modules.

- **Main Router** \
(`src/main.cpp`): Acts as the central hub, providing the top-level menu and orchestrating the transition between simulation modes.

- **Contiguous Allocator** \
(`src/contiguous/`): Implements the core logic for linear memory management, while (`include/contiguous/`) defines the block structures and allocation strategy Enums.

- **Buddy Allocator** \
(`src/buddy/`): Handles the binary buddy splitting and coalescing algorithms, while (`include/buddy/`) contains the definitions for buddy blocks and level-based free lists.

- **Cache Hierarchy** \
(`src/cache/`): Simulates the data flow through a multi-level cache system, while (`include/cache/`) defines the set-associative metadata and replacement policies.

---

## Module Details

### Contiguous Memory Management
The contiguous allocator manages memory as a doubly linked list of blocks.

- **Data Structure**: 
  ```cpp
  struct Block {
      size_t start;
      size_t size;
      bool is_free;
      int process_id;
      Block *prev, *next;
  };
  ```
- **Algorithms**:
  - **First Fit**: Searches from the head and picks the first free block that is large enough.
  - **Best Fit**: Scans the entire list to find the smallest free block that fits, minimizing wasted space (internal fragmentation).
  - **Worst Fit**: Scans the entire list to find the largest free block, attempting to leave large enough leftovers for other processes.
  - **Compaction**: Iterates through the list, shifts all allocated blocks to the start of the memory pool, and merges all free blocks into a single large block at the end.

### Buddy System
The Buddy System uses a fixed-size memory pool (rounded to a power of two) and manages free blocks using a series of free lists.

- **Data Structure**:
  - `BuddyBlock`: A header placed at the start of each block containing metadata.
  - `free_lists[MAX_LEVELS]`: An array of linked lists, where `free_lists[i]` stores free blocks of size `2^(MIN_POWER + i)`.
- **Key Concepts**:
  - **Splitting**: If no block of the requested size is available, a larger block is recursively split into two "buddies" until the target size is reached.
  - **Coalescing (Merging)**: When a block is freed, the allocator checks if its "buddy" is also free. If so, they are merged back into a single larger block. This process continues recursively.
  - **Buddy Calculation**: The address of a buddy is calculated using XOR: **`buddy_offset = current_offset ^ block_size`**.

### Cache Simulator
A functional simulation of a multilevel set-associative cache.

- **Design**:
  - **Set-Associative**: Each level is divided into $S$ sets, each containing $W$ ways (lines).
  - **Mapping**: Addresses are mapped using the formula: **`Index = (Address / BlockSize) % NumSets`**.
  - **Replacement Policy**: **FIFO (First-In-First-Out)**. Every set maintains a local or global counter to track the arrival order of blocks.
- **Multilevel Hierarchy**:
  - Access starts at L1.
  - On an L1 miss, the request is passed to L2, and so on.
  - A hit at any level stops the search.
  - A miss at the final level is treated as a Main Memory access.

---


## Performance Metrics
The simulator calculates several key performance indicators:
- **Utilization**: (Allocated Memory / Total Memory) * 100.
- **External Fragmentation**: Calculated as `1.0 - (LargestFreeBlock / TotalFreeMemory)`.
- **Cache Hit Rate**: (Hits / Total Accesses) * 100.
- **Allocation Success Rate**: (Successful Mallocs / Total Malloc Attempts) * 100.

---

## Limitations and Simplifications

### Contiguous Allocator
- **Single Memory Pool**
- **No Real Memory**
- **Integer Process IDs**
- **Synchronous Operations**

### Buddy System
- **Power-of-Two Sizes**
- **Fixed Maximum Levels** : `MAX_LEVELS = 6` (block sizes from 32B to 1024B by default).
- **Minimum Block Size**: Fixed at 32 bytes (`MIN_BLOCK_SIZE`).
- **No Partial Coalescing**

### Cache Simulator
- **FIFO Only**
- **No Cache Coherency**
- **Simplified Address Model**

### General
- **No Persistence**
- **No Virtual Memory**

---

## Future Improvements
- **LRU/LFU Replacement Policies**: Add more advanced replacement algorithms for the Cache Simulator.
- **Virtual Memory**: Implement virtual memory and paging


---