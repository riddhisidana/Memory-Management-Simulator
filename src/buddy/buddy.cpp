#include "../include/buddy/buddy.h"

BuddyBlock *free_lists[MAX_LEVELS];
size_t total_memory = 0;
char *memory_pool;

void buddy_init(size_t total_size)
{
    if (total_size < MIN_BLOCK_SIZE)
    {
        total_size = MIN_BLOCK_SIZE;
    }
    size_t actual_total = next_power_of_two(total_size);
    size_t max_size = get_size_for_level(MAX_LEVELS - 1);
    if (actual_total > max_size)
    {
        actual_total = max_size;
    }
    total_memory = actual_total;
    memory_pool = new char[total_memory];
    memset(memory_pool, 0, total_memory);
    for (int i = 0; i < MAX_LEVELS; i++)
    {
        free_lists[i] = nullptr;
    }

    BuddyBlock *initial_block = (BuddyBlock *)memory_pool;
    initial_block->size = total_memory;
    initial_block->is_free = true;
    initial_block->process_id = -1;
    initial_block->prev = nullptr;
    initial_block->next = nullptr;

    int level = get_level(total_memory);
    if (level < 0)
        level = 0;
    if (level >= MAX_LEVELS)
        level = MAX_LEVELS - 1;
    free_lists[level] = initial_block;
}
void *buddy_allocate(size_t size, int process_id)
{
    char *check_ptr = memory_pool;
    while (check_ptr < memory_pool + total_memory)
    {
        BuddyBlock *check = (BuddyBlock *)check_ptr;
        if (!check->is_free && check->process_id == process_id)
        {
            cout << "Error: Process with PID " << process_id << " already exists!" << endl;
            return nullptr;
        }
        check_ptr += check->size;
    }

    // FIXED: Account for header size
    size_t actual_size = next_power_of_two(size);
    int target_level = get_level(actual_size);
    int current_level = target_level;

    while (current_level < MAX_LEVELS && free_lists[current_level] == nullptr)
    {
        current_level++;
    }
    if (current_level == MAX_LEVELS)
    {
        return nullptr;
    }

    while (current_level > target_level)
    {
        BuddyBlock *block = free_lists[current_level];
        // take head
        free_lists[current_level] = (block) ? block->next : nullptr;
        if (block && block->next)
            block->next->prev = nullptr;
        if (!block)
            break; // safety

        size_t half_size = block->size / 2;
        BuddyBlock *buddy1 = block;
        BuddyBlock *buddy2 = (BuddyBlock *)((char *)block + half_size);

        // initialize buddies
        buddy1->size = half_size;
        buddy1->is_free = true;
        buddy1->process_id = -1;
        buddy1->prev = nullptr;

        buddy2->size = half_size;
        buddy2->is_free = true;
        buddy2->process_id = -1;
        buddy2->prev = buddy1;

        // Insert buddy1->buddy2 at head of lower-level free list
        BuddyBlock *old_head = free_lists[current_level - 1];
        buddy1->next = buddy2;
        buddy2->next = old_head;
        if (old_head)
            old_head->prev = buddy2;
        free_lists[current_level - 1] = buddy1;

        current_level--;
    }

    BuddyBlock *block = free_lists[target_level];
    if (!block)
        return nullptr;

    free_lists[target_level] = block->next;
    if (block->next)
        block->next->prev = nullptr;

    block->is_free = false;
    block->process_id = process_id;
    block->next = nullptr;
    block->prev = nullptr;

    // Return pointer after header (user doesn't see header)
    return (void *)((char *)block + sizeof(BuddyBlock));
}

bool buddy_deallocate(int process_id)
{
    BuddyBlock *block = nullptr;
    char *ptr = memory_pool;

    // Find the block allocated to this process
    while (ptr < memory_pool + total_memory)
    {
        BuddyBlock *b = (BuddyBlock *)ptr;
        if (!b->is_free && b->process_id == process_id)
        {
            block = b;
            break;
        }
        ptr += b->size;
    }

    if (!block)
    {
        return false; // Block not found
    }

    // Mark block as free
    block->is_free = true;
    block->process_id = -1;

    size_t block_size = block->size;
    int level = get_level(block_size);

    // Merge with buddy blocks
    while (level < MAX_LEVELS - 1)
    {
        size_t offset = (char *)block - memory_pool;
        size_t buddy_offset = offset ^ block_size; // XOR to find buddy

        if (buddy_offset >= total_memory)
            break;

        BuddyBlock *buddy = (BuddyBlock *)(memory_pool + buddy_offset);

        // Check if buddy is free and same size
        if (!(buddy->is_free && buddy->size == block_size))
        {
            break;
        }

        // Remove buddy from free list
        if (buddy->prev)
        {
            buddy->prev->next = buddy->next;
        }
        if (buddy->next)
        {
            buddy->next->prev = buddy->prev;
        }
        if (free_lists[level] == buddy)
        {
            free_lists[level] = buddy->next;
        }

        // Merge: use the lower address block
        if (buddy < block)
        {
            block = buddy;
        }

        // Double the size and move up a level
        block->size *= 2;
        block_size = block->size; // CRITICAL: Update block_size for next iteration
        block->is_free = true;
        block->process_id = -1;
        level++;
    }

    // Insert merged block into appropriate free list
    block->prev = nullptr;
    block->next = free_lists[level];
    if (free_lists[level])
        free_lists[level]->prev = block;
    free_lists[level] = block;

    return true; // Successfully deallocated
}

void buddy_display()
{
    cout << "\n===== Buddy System Free Lists =====" << endl;
    for (int level = 0; level < MAX_LEVELS; ++level)
    {
        size_t block_size = get_size_for_level(level);
        cout << "Level " << level << " (Block size " << block_size << "): ";
        BuddyBlock *curr = free_lists[level];
        if (!curr)
        {
            cout << "[empty]" << endl;
            continue;
        }
        while (curr)
        {
            size_t offset = (char *)curr - memory_pool;
            cout << "[offset " << offset << ", size " << curr->size << "] ";
            curr = curr->next;
        }
        cout << endl;
    }
    cout << "===================================" << endl;
}
void buddy_stats()
{
    size_t total_free = 0, largest_free = 0, used_memory = 0;
    int free_blocks = 0, used_blocks = 0;
    char *ptr = memory_pool;
    while (ptr < memory_pool + total_memory)
    {
        BuddyBlock *block = (BuddyBlock *)ptr;
        if (block->is_free)
        {
            total_free += block->size;
            free_blocks++;
            if (block->size > largest_free)
                largest_free = block->size;
        }
        else
        {
            used_memory += block->size;
            used_blocks++;
        }
        ptr += block->size;
    }
    double utilization = (double)used_memory / total_memory * 100;
    double external_frag = (total_free > 0) ? 1.0 - ((double)largest_free / total_free) : 0.0;

    cout << "\n========== BUDDY SYSTEM STATISTICS ==========\n";
    cout << "Total Memory:        " << total_memory << " bytes\n";
    cout << "Used Memory:         " << used_memory << " bytes\n";
    cout << "Free Memory:         " << total_free << " bytes\n";
    cout << "Memory Utilization:  " << fixed << setprecision(1) << utilization << "%\n";
    cout << "---------------------------------------------\n";
    cout << "Allocated Blocks:    " << used_blocks << "\n";
    cout << "Free Blocks:         " << free_blocks << "\n";
    cout << "Largest Free Block:  " << largest_free << " bytes\n";
    cout << "---------------------------------------------\n";
    cout << "External Fragmentation: " << fixed << setprecision(1) << (external_frag * 100) << "%\n";
    cout << "=============================================\n";
}
void buddy_shutdown()
{

    if (memory_pool)
    {
        delete[] memory_pool;
        memory_pool = nullptr;
    }
    for (int i = 0; i < MAX_LEVELS; ++i)
    {
        free_lists[i] = nullptr;
    }
    total_memory = 0;
}

int get_level(size_t size)
{
    return (int)log2(size) - MIN_BLOCK_POWER;
}

size_t get_size_for_level(int level)
{
    return MIN_BLOCK_SIZE << level;
}

size_t next_power_of_two(size_t size)
{
    if (size <= MIN_BLOCK_SIZE)
    {
        return MIN_BLOCK_SIZE;
    }
    int power = (int)ceil(log2(size));
    return (size_t)1 << power;
}

void buddy_dump()
{
    cout << "\n===== Buddy System Memory Dump =====" << endl;
    char *ptr = memory_pool;
    int idx = 0;
    while (ptr < memory_pool + total_memory)
    {
        BuddyBlock *b = (BuddyBlock *)ptr;
        cout << "[Block " << idx << "] Offset: " << (ptr - memory_pool)
             << ", Size: " << b->size << ", ";
        if (b->is_free)
            cout << "Status: Free";
        else
            cout << "Status: Allocated, PID: " << b->process_id;
        cout << endl;
        ptr += b->size;
        idx++;
    }
    cout << "====================================" << endl;
}

void buddy_reset()
{
    if (!memory_pool)
        return;

    // clear pool to avoid stale headers
    memset(memory_pool, 0, total_memory);

    // reset free lists
    for (int i = 0; i < MAX_LEVELS; ++i)
        free_lists[i] = nullptr;

    // create single initial free block covering entire pool
    BuddyBlock *initial_block = (BuddyBlock *)memory_pool;
    initial_block->size = total_memory;
    initial_block->is_free = true;
    initial_block->process_id = -1;
    initial_block->prev = nullptr;
    initial_block->next = nullptr;

    int level = get_level(total_memory);
    if (level < 0)
        level = 0;
    if (level >= MAX_LEVELS)
        level = MAX_LEVELS - 1;
    free_lists[level] = initial_block;
}