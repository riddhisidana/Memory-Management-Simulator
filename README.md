# Memory Management Simulator

A comprehensive C++ simulation tool designed to demonstrate and visualize various memory management techniques used in modern operating systems.

## Features

The simulator is divided into three primary modules, each focusing on a different aspect of memory management:

### 1. Contiguous Allocation Simulator
Simulates memory allocation in a continuous block of memory with advanced management features:

- `Allocation Strategies`: Supports **First Fit**, **Best Fit**, and **Worst Fit** algorithms.
- `Dynamic Strategy Switching`: Change the allocation algorithm on-the-fly to compare performance.
- `Compaction`: Move all allocated blocks to one end to eliminate external fragmentation.
- `Visualization`: 
    - **Memory Layout Map**: A block-by-block view of memory occupancy.
    - **Visual Map**: A horizontal bar representing process placement and holes.
    - **Usage Bar**: A high-level utilization percentage graph.
- `Statistics`: Comprehensive metrics including:
    - Memory utilization percentage.
    - External fragmentation analysis.
    - Allocation success/failure rates and attempts.
    - Process-specific memory footprint tracking.
- `Coalescing`: Automatic merging of adjacent free blocks upon deallocation.

### 2. Buddy System Simulator
Implements the binary buddy algorithm for efficient, power-of-two based memory management:

- `Power of Two Management`: Automatically rounds all memory requests up to the nearest $2^n$ block size.
- `Dynamic Splitting & Coalescing`: 
    - Recursively splits large blocks into "buddies" to satisfy small requests.
    - Automatically merges adjacent buddies back into larger blocks when freed.
- `Free List Tracking`: Displays the state of free blocks at every level (power of two).
- `Memory Dump`: A detailed view of every block in the system, its size, and its current owner (PID).
- `Efficiency Metrics`: Tracking of internal and external fragmentation caused by block rounding.

### 3. Cache Simulator
A sophisticated multi-level cache hierarchy simulator to study memory access patterns:
- `Configurable Hierarchy`: Support for **L1, L2, and L3** cache levels.
- `Fully Customizable Parameters`: Configure each level independently:
    - **Cache Size**: Total capacity in KB.
    - **Block Size**: Data block size in bytes.
    - **Associativity**: From Direct-Mapped to $N$-Way Set Associative.
- `Replacement Policy`: Implements the **FIFO (First-In-First-Out)** replacement algorithm.
- `Memory Access Simulation`: Simulate single memory accesses using hexadecimal addresses.
- `Performance Analytics`:
    - Individual hit/miss statistics for each cache level.
    - Hit rate and miss rate percentages.
    - Global hierarchy performance summary.
- `Cache Content Inspection`: View the current configuration and status of all cache levels.

---

## Build


### Build Instructions

PreRequisite : `c++23` or change the compiling code accordingly.

Run the code on the root :
```
g++ -Iinclude -std=c++23 src/*.cpp src/buddy/*.cpp src/cache/*.cpp src/contiguous/*.cpp -o memory_sim
```
**Note:** Makefile isn't working; will correct this at a later stage.

This will generate the executable `memory_sim`.

---

## Usage

Run the simulator using:
```
./memory_sim
```

### Navigating the Simulator
1. Upon start, choose a simulation mode from the main menu.
2. Type `help` within any mode to see a list of available commands.
3. Use the `mode` command to return to the main menu.
4. Use `quit` to exit the application.

### Example Commands
**All the multilines command can work in single line also.**
- **In `Contiguous` Mode**:
    - to `malloc` 100 bytes for PID 1.
      ```
      malloc 
      100 1
      ```
    - `setstrategy`: Change allocation algorithm (e.g., to Best Fit).
      ```
      setstrategy
      2
      ```
    - `compact`: Run the defragmentation routine.
      ```
      compact
      ```
    - `stats`: View detailed fragmentation and success metrics.
      ```
      stats
      ```
    - `bar`: View a visual utilization graph.
      ```
      bar
      ```
- **In `Buddy` Mode**:
    - to `malloc` 250 bytes (auto-rounded to 256).
      ```
      malloc
      250 1
      ```
    - `display`: Show the current free lists for Buddies.
      ```
      display
      ```
    - `dump`: View the exact block-by-block memory layout.
      ```
      dump
      ```
    - `free 1`: Release memory belonging to PID 1.
      ```
      free
      1
      ```
- **In `Cache` Mode**:
    - Initialize a 3-level cache (L1, L2, L3).
      ``` 
      init 3
      ```
    - Set L1 to 64KB, 64B block size, 2-way associative.
      ```
      config 1 64 64 2
      ```
    - Simulate a memory access to address 0x1A2B.
      ```
      access 0x1A2B
      ```
    - Compare hit/miss rates across all cache levels.
      ```
      stats
      ```
## Project Structure

```
├── README.md
├── Makefile
├── memory_sim.exe
├── include/
│   ├── contiguous/
│   │   ├── block.h
│   │   └── contiguous.h
│   ├── buddy/
│   │   └── buddy.h
│   ├── cache/
│   │   ├── cache.h
│   │   └── cache_types.h
├── src/
│   ├── contiguous/
│   │   └── contiguous.cpp
│   ├── buddy/
│   │   └── buddy.cpp
│   ├── cache/
│   │   └── cache.cpp
│   ├── main.cpp
│   ├── main_memory.cpp
│   ├── main_buddy.cpp
│   └── main_cache.cpp
├── test/
│   └── test.cpp
└── docs/
    └── design.md
```
---
