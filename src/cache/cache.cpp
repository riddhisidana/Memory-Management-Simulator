#include "../../include/cache/cache.h"
#include <climits>
#include <iomanip>

// Multilevel cache globals
vector<CacheLevel> cache_levels;
int num_levels = 0;

// ============================================================================
// MULTILEVEL CACHE FUNCTIONS
// ============================================================================

void init_multilevel_cache(int num_levels_to_create)
{
    num_levels = num_levels_to_create;
    cache_levels.resize(num_levels);

    // Initialize each level with default values
    for (int i = 0; i < num_levels; i++)
    {
        cache_levels[i].level_number = i + 1;
        // Default configurations (can be changed via configure_level)
        if (i == 0) // L1
        {
            configure_level(i + 1, 32, 64, 1); // 32KB, 64B, direct-mapped
        }
        else if (i == 1) // L2
        {
            configure_level(i + 1, 256, 64, 4); // 256KB, 64B, 4-way
        }
        else // L3
        {
            configure_level(i + 1, 2048, 64, 8); // 2MB, 64B, 8-way
        }
    }
}

void configure_level(int level, int size_kb, int blk_size, int ways)
{
    if (level < 1 || level > num_levels)
        return;

    int idx = level - 1;
    CacheLevel &cache_level = cache_levels[idx];

    cache_level.cache_size_kb = size_kb;
    cache_level.block_size = blk_size;
    cache_level.associativity = ways;

    int total_blocks = (size_kb * 1024) / blk_size;
    cache_level.num_sets = total_blocks / ways;

    // Initialize cache sets
    cache_level.sets.resize(cache_level.num_sets);
    for (int i = 0; i < cache_level.num_sets; i++)
    {
        cache_level.sets[i].resize(ways);
        for (int j = 0; j < ways; j++)
        {
            cache_level.sets[i][j].valid = false;
            cache_level.sets[i][j].tag = 0;
            cache_level.sets[i][j].fifo_counter = 0;
        }
    }

    // Reset statistics
    cache_level.accesses = 0;
    cache_level.hits = 0;
    cache_level.misses = 0;
    cache_level.global_fifo_counter = 0;
}

bool multilevel_access(unsigned long address)
{
    bool overall_hit = false;

    // Try each level in order
    for (int level = 0; level < num_levels; level++)
    {
        CacheLevel &cache_level = cache_levels[level];
        cache_level.accesses++;

        // Decode address for this level
        int set_idx = address % cache_level.num_sets;
        unsigned long tag = address / cache_level.num_sets;

        // Check for hit in this level
        bool level_hit = false;
        for (int way = 0; way < cache_level.associativity; way++)
        {
            if (cache_level.sets[set_idx][way].valid &&
                cache_level.sets[set_idx][way].tag == tag)
            {
                // Hit!
                cache_level.hits++;
                level_hit = true;
                overall_hit = true;

                // Update FIFO
                cache_level.global_fifo_counter++;
                cache_level.sets[set_idx][way].fifo_counter = cache_level.global_fifo_counter;
                break;
            }
        }

        if (!level_hit)
        {
            // Miss - install in this level
            cache_level.misses++;

            // Find victim
            int oldest_counter = INT_MAX;
            int victim_way = 0;
            for (int way = 0; way < cache_level.associativity; way++)
            {
                if (cache_level.sets[set_idx][way].fifo_counter < oldest_counter)
                {
                    oldest_counter = cache_level.sets[set_idx][way].fifo_counter;
                    victim_way = way;
                }
            }

            // Replace
            cache_level.sets[set_idx][victim_way].valid = true;
            cache_level.sets[set_idx][victim_way].tag = tag;
            cache_level.global_fifo_counter++;
            cache_level.sets[set_idx][victim_way].fifo_counter = cache_level.global_fifo_counter;

            // If this is not the last level, continue to next level
            // If this is the last level, it's a main memory access (always "miss" from cache perspective)
            if (level == num_levels - 1)
            {
                overall_hit = false; // Main memory access
            }
        }
        else
        {
            // Hit in this level - no need to go further
            break;
        }
    }

    return overall_hit;
}

void display_multilevel_stats()
{
    cout << "\n========== MULTILEVEL CACHE STATISTICS ==========\n";

    for (int i = 0; i < num_levels; i++)
    {
        CacheLevel &level = cache_levels[i];
        cout << "\n--- Level " << level.level_number << " (L" << level.level_number << ") ---\n";
        cout << "Size: " << level.cache_size_kb << "KB, Block: " << level.block_size
             << "B, Ways: " << level.associativity << "\n";
        cout << "Accesses: " << level.accesses << "\n";
        cout << "Hits: " << level.hits << "\n";
        cout << "Misses: " << level.misses << "\n";
        cout << "Hit Rate: " << fixed << setprecision(2)
             << (level.accesses > 0 ? (double)level.hits / level.accesses * 100 : 0) << "%\n";
    }

    // Overall statistics
    long long total_accesses = 0;
    long long total_hits = 0;
    for (auto &level : cache_levels)
    {
        total_accesses += level.accesses;
        total_hits += level.hits;
    }

    cout << "\n--- Overall ---\n";
    cout << "Total Accesses: " << total_accesses << "\n";
    cout << "Total Hits: " << total_hits << "\n";
    cout << "Overall Hit Rate: " << fixed << setprecision(2)
         << (total_accesses > 0 ? (double)total_hits / total_accesses * 100 : 0) << "%\n";
    cout << "====================================================\n";
}

void clear_multilevel_cache()
{
    for (auto &level : cache_levels)
    {
        for (int i = 0; i < level.num_sets; i++)
        {
            for (int j = 0; j < level.associativity; j++)
            {
                level.sets[i][j].valid = false;
                level.sets[i][j].tag = 0;
                level.sets[i][j].fifo_counter = 0;
            }
        }
        level.accesses = 0;
        level.hits = 0;
        level.misses = 0;
        level.global_fifo_counter = 0;
    }
}
