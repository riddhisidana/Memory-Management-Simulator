# Memory Management Simulator - Test Scripts

This file contains test scripts for each module. Copy-paste the commands into the simulator console and compare with expected output.

---

## 1. Contiguous Memory Allocator Test

**Objective:** Test First Fit allocation, deallocation, fragmentation, and compaction.

### How to Run
1. Run `./memory_sim.exe`
2. Select mode `1` (Contiguous Allocator)
3. Enter the setup inputs, then run commands one by one

### Setup Input
```
1024
1
```
*(1024 bytes total memory, First Fit strategy)*

### Test Commands
```
malloc
200 1
malloc
300 2
malloc
150 3
dump
free
2
dump
malloc
100 4
dump
compact
dump
stats
bar
quit
```

### Expected Output

**After allocating P1, P2, P3 and running `dump`:**
```
===== Memory Layout =====
[Block 0] Start: 0, Size: 200, Status: Allocated, Process: 1
[Block 1] Start: 200, Size: 300, Status: Allocated, Process: 2
[Block 2] Start: 500, Size: 150, Status: Allocated, Process: 3
[Block 3] Start: 650, Size: 374, Status: Free
=========================

Visual Map:
|---P1(200)---|---P2(300)---|---P3(150)---|---FREE(374)---|
```

**After `free 2` and `dump`:**
```
===== Memory Layout =====
[Block 0] Start: 0, Size: 200, Status: Allocated, Process: 1
[Block 1] Start: 200, Size: 300, Status: Free
[Block 2] Start: 500, Size: 150, Status: Allocated, Process: 3
[Block 3] Start: 650, Size: 374, Status: Free
=========================

Visual Map:
|---P1(200)---|---FREE(300)---|---P3(150)---|---FREE(374)---|
```

**After `malloc 100 4` and `dump`:**
```
===== Memory Layout =====
[Block 0] Start: 0, Size: 200, Status: Allocated, Process: 1
[Block 1] Start: 200, Size: 100, Status: Allocated, Process: 4
[Block 2] Start: 300, Size: 200, Status: Free
[Block 3] Start: 500, Size: 150, Status: Allocated, Process: 3
[Block 4] Start: 650, Size: 374, Status: Free
=========================

Visual Map:
|---P1(200)---|---P4(100)---|---FREE(200)---|---P3(150)---|---FREE(374)---|
```

**After `compact` and `dump`:**
```
===== Memory Layout =====
[Block 0] Start: 0, Size: 200, Status: Allocated, Process: 1
[Block 1] Start: 200, Size: 100, Status: Allocated, Process: 4
[Block 2] Start: 300, Size: 150, Status: Allocated, Process: 3
[Block 3] Start: 450, Size: 574, Status: Free
=========================

Visual Map:
|---P1(200)---|---P4(100)---|---P3(150)---|---FREE(574)---|
```

**After `stats`:**
```
========== MEMORY STATISTICS ==========
Total Memory:        1024 bytes
Used Memory:         450 bytes
Free Memory:         574 bytes
Memory Utilization:  43.9%
----------------------------------------
Allocated Blocks:    3
Free Blocks:         1
Largest Free Block:  574 bytes
----------------------------------------
External Fragmentation: 0.0%
----------------------------------------
Allocation Attempts: 4
Successful:          4
Failed:              0
Success Rate:        100.0%
========================================
```

**After `bar`:**
```
[#############-----------------] 43.9% used
Used: 450 / 1024 bytes
```

---

## 2. Buddy System Allocator Test

**Objective:** Test buddy allocation, splitting, deallocation, and coalescing.

### Setup Input
From main menu, select mode `2` (Buddy System), then enter:
```
512
```
*(512 bytes total memory - will be treated as power of 2)*

### Test Commands
```
malloc
64 1
malloc
64 2
malloc
128 3
display
dump
stats
free
2
display
dump
free
1
display
dump
free
3
display
stats
quit
```

### Expected Output

**After allocating P1 (64), P2 (64), P3 (128) and `display`:**
```
===== Buddy System Free Lists =====
Level 0 (Block size 32): [empty]
Level 1 (Block size 64): [empty]
Level 2 (Block size 128): [empty]
Level 3 (Block size 256): [offset 256, size 256] 
Level 4 (Block size 512): [empty]
Level 5 (Block size 1024): [empty]
===================================
```
*(One 128-byte block remains free after splitting)*

**After `dump`:**
```
===== Buddy System Memory Dump =====
[Block 0] Offset: 0, Size: 64, Status: Allocated, PID: 1
[Block 1] Offset: 64, Size: 64, Status: Allocated, PID: 2
[Block 2] Offset: 128, Size: 128, Status: Allocated, PID: 3
[Block 3] Offset: 256, Size: 256, Status: Free
====================================
```

**After `stats`:**
```
========== BUDDY SYSTEM STATISTICS ==========
Total Memory:        512 bytes
Used Memory:         256 bytes
Free Memory:         256 bytes
Memory Utilization:  50.0%
---------------------------------------------
Allocated Blocks:    3
Free Blocks:         1
Largest Free Block:  256 bytes
---------------------------------------------
External Fragmentation: 0.0%
=============================================
```

**After `free 2` and `display`:**
```
===== Buddy System Free Lists =====
Level 0 (Block size 32): [empty]
Level 1 (Block size 64): [offset 64, size 64]
Level 2 (Block size 128): [empty]
Level 3 (Block size 256): [offset 256, size 256] 
Level 4 (Block size 512): [empty]
Level 5 (Block size 1024): [empty]
===================================d
```

**After `free 1` and `display`:**
```
===== Buddy System Free Lists =====
Level 0 (Block size 32): [empty]
Level 1 (Block size 64): [empty]
Level 2 (Block size 128): [offset 0, size 128]
Level 3 (Block size 256): [offset 256, size 256]
Level 4 (Block size 512): [empty]
Level 5 (Block size 1024): [empty]
===================================
```

**After `free 3` and `display`:**
```
===== Buddy System Free Lists =====
Level 0 (Block size 32): [empty]
Level 1 (Block size 64): [empty]
Level 2 (Block size 128): [empty]
Level 3 (Block size 256): [empty]
Level 4 (Block size 512): [offset 0, size 512] 
Level 5 (Block size 1024): [empty]
===================================
```

**Final `stats`:**
```
========== BUDDY SYSTEM STATISTICS ==========
Total Memory:        512 bytes
Used Memory:         0 bytes
Free Memory:         512 bytes
Memory Utilization:  0.0%
---------------------------------------------
Allocated Blocks:    0
Free Blocks:         1
Largest Free Block:  512 bytes
---------------------------------------------
External Fragmentation: 0.0%
=============================================
```

---

## 3. Cache Simulator Test

**Objective:** Test multilevel cache access, hits, misses, and FIFO replacement.

### Setup
From main menu, select mode `3` (Cache Simulator). Default 2-level cache is initialized automatically.

### Test Commands
```
display
config 1 1 64 2
config 2 4 64 4
display
access 0
access 100
access 200
access 0
access 100
access 300
access 400
access 0
stats
clear
stats
quit
```

### Expected Output

**After `display` (default config):**
```
Multilevel cache with 2 levels:
L1: 32KB, 64B, 1-way
L2: 256KB, 64B, 4-way
```

**After configuring and `display`:**
```
Multilevel cache with 2 levels:
L1: 1KB, 64B, 2-way
L2: 4KB, 64B, 4-way
```

**Access sequence outputs:**
```
cache> access 0
Access to 0x0: MISS

cache> access 100
Access to 0x64: MISS

cache> access 200
Access to 0xc8: MISS

cache> access 0
Access to 0x0: HIT

cache> access 100
Access to 0x64: HIT

cache> access 300
Access to 0x12c: MISS

cache> access 400
Access to 0x190: MISS

cache> access 0
Access to 0x0: HIT
```

**After `stats`:**
```
========== MULTILEVEL CACHE STATISTICS ==========

--- Level 1 (L1) ---
Size: 1KB, Block: 64B, Ways: 2
Accesses: 8
Hits: 3
Misses: 5
Hit Rate: 37.50%

--- Level 2 (L2) ---
Size: 4KB, Block: 64B, Ways: 4
Accesses: 5
Hits: 0
Misses: 5
Hit Rate: 0.00%

--- Overall ---
Total Accesses: 13
Total Hits: 3
Overall Hit Rate: 23.08%
====================================================
```

**After `clear` and `stats`:**
```
========== MULTILEVEL CACHE STATISTICS ==========

--- Level 1 (L1) ---
Size: 1KB, Block: 64B, Ways: 2
Accesses: 0
Hits: 0
Misses: 0
Hit Rate: 0.00%

--- Level 2 (L2) ---
Size: 4KB, Block: 64B, Ways: 4
Accesses: 0
Hits: 0
Misses: 0
Hit Rate: 0.00%

--- Overall ---
Total Accesses: 0
Total Hits: 0
Overall Hit Rate: 0.00%
====================================================
```

---

## Quick Reference

| Module | Menu Choice | Key Commands |
|--------|-------------|--------------|
| Contiguous | 1 | `malloc`, `free`, `dump`, `compact`, `stats`, `bar` |
| Buddy | 2 | `malloc`, `free`, `display`, `dump`, `stats` |
| Cache | 3 | `init`, `config`, `access`, `display`, `stats`, `clear` |
