#include "../include/contiguous/contiguous.h"
using namespace std;

Block *head = nullptr;
size_t total_memory_size = 0;
AllocationStrategy current_strategy;
int allocation_attempts = 0;
int allocation_successes = 0;

void init_memory(size_t total_size, AllocationStrategy strategy)
{
    total_memory_size = total_size;
    current_strategy = strategy;

    head = new Block;
    head->start = 0;
    head->size = total_size;
    head->is_free = true;
    head->process_id = -1;
    head->prev = nullptr;
    head->next = nullptr;
}
Block *find_first_fit(size_t size)
{
    Block *current = head;
    while (current != nullptr)
    {
        if (current->is_free && current->size >= size)
        {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

Block *find_best_fit(size_t size)
{
    Block *current = head;
    Block *best = nullptr;

    while (current != nullptr)
    {
        if (current->is_free && current->size >= size)
        {
            if (best == nullptr || current->size < best->size)
            {
                best = current;
            }
        }
        current = current->next;
    }
    return best;
}

Block *find_worst_fit(size_t size)
{
    Block *current = head;
    Block *worst = nullptr;

    while (current != nullptr)
    {
        if (current->is_free && current->size >= size)
        {
            if (worst == nullptr || current->size > worst->size)
            {
                worst = current;
            }
        }
        current = current->next;
    }
    return worst;
}

Block *allocate(size_t size, int process_id)
{
    allocation_attempts++;

    Block *check = head;
    while (check != nullptr)
    {
        if (!check->is_free && check->process_id == process_id)
        {
            cout << "Error: Process with PID " << process_id << " already exists!" << endl;
            return nullptr;
        }
        check = check->next;
    }

    Block *selected = nullptr;

    if (current_strategy == AllocationStrategy::first_fit)
    {
        selected = find_first_fit(size);
    }
    else if (current_strategy == AllocationStrategy::best_fit)
    {
        selected = find_best_fit(size);
    }
    else if (current_strategy == AllocationStrategy::worst_fit)
    {
        selected = find_worst_fit(size);
    }

    if (selected == nullptr)
    {
        return nullptr;
    }

    if (selected->size > size)
    {
        Block *new_block = new Block;
        new_block->start = selected->start + size;
        new_block->size = selected->size - size;
        new_block->is_free = true;
        new_block->process_id = -1;

        new_block->next = selected->next;
        new_block->prev = selected;
        if (selected->next != nullptr)
        {
            selected->next->prev = new_block;
        }
        selected->next = new_block;
        selected->size = size;
    }
    selected->is_free = false;
    selected->process_id = process_id;
    allocation_successes++;
    return selected;
}

void deallocate(int process_id)
{
    Block *current = head;
    while (current != nullptr)
    {
        if (current->process_id == process_id)
        {
            current->is_free = true;
            current->process_id = -1;

            if (current->next != nullptr && current->next->is_free)
            {
                Block *next_block = current->next;
                current->size += next_block->size;
                current->next = next_block->next;
                if (next_block->next != nullptr)
                {
                    next_block->next->prev = current;
                }
                delete next_block;
            }
            if (current->prev != nullptr && current->prev->is_free)
            {
                Block *prev_block = current->prev;
                prev_block->size += current->size;
                prev_block->next = current->next;
                if (current->next != nullptr)
                {
                    current->next->prev = prev_block;
                }
                delete current;
            }
            return;
        }
        current = current->next;
    }
}

void display_memory()
{
    Block *current = head;
    int block_num = 0;
    cout << "\n===== Memory Layout =====\n";

    while (current != nullptr)
    {
        cout << "[Block " << block_num << "] ";
        cout << "Start: " << current->start;
        cout << ", Size: " << current->size;

        if (current->is_free)
        {
            cout << ", Status: Free";
        }
        else
        {
            cout << ", Status: Allocated, Process: " << current->process_id;
        }
        cout << "\n";
        current = current->next;
        block_num++;
    }
    cout << "=========================\n";

    cout << "\nVisual Map:\n|";
    current = head;
    while (current != nullptr)
    {
        if (current->is_free)
        {
            cout << "---FREE(" << current->size << ")---|";
        }
        else
        {
            cout << "---P" << current->process_id << "(" << current->size << ")---|";
        }
        current = current->next;
    }
    cout << "\n";
}

double calculate_external_fragmentation()
{
    Block *current = head;
    size_t total_free = 0;
    size_t largest_free = 0;

    while (current != nullptr)
    {
        if (current->is_free)
        {
            total_free += current->size;
            if (current->size > largest_free)
            {
                largest_free = current->size;
            }
        }
        current = current->next;
    }

    if (total_free == 0)
    {
        return 0.0;
    }

    return 1.0 - ((double)largest_free / (double)total_free);
}

void shutdown_memory()
{
    Block *current = head;

    while (current != nullptr)
    {
        Block *next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
    total_memory_size = 0;
}

void set_strategy(AllocationStrategy strategy)
{
    current_strategy = strategy;
}

AllocationStrategy get_strategy()
{
    return current_strategy;
}

void display_processes()
{
    Block *current = head;
    cout << "\n===== Active Processes =====\n";
    bool found = false;

    while (current != nullptr)
    {
        if (!current->is_free)
        {
            cout << "PID " << current->process_id
                 << ": " << current->size
                 << " bytes at address " << current->start << "\n";
            found = true;
        }
        current = current->next;
    }

    if (!found)
    {
        cout << "No active processes.\n";
    }
    cout << "============================\n";
}
void compact_memory()
{
    Block *current = head;
    size_t next_start = 0;
    while (current != nullptr)
    {
        if (!current->is_free)
        {
            current->start = next_start;
            next_start += current->size;
        }
        current = current->next;
    }
    Block *new_head = nullptr;
    Block *new_tail = nullptr;
    current = head;

    while (current != nullptr)
    {
        Block *next = current->next;
        if (!current->is_free)
        {
            current->prev = new_tail;
            current->next = nullptr;

            if (new_head == nullptr)
            {
                new_head = current;
            }
            else
            {
                new_tail->next = current;
            }
            new_tail = current;
        }
        else
        {
            delete current;
        }
        current = next;
    }

    if (next_start < total_memory_size)
    {
        Block *free_block = new Block;
        free_block->start = next_start;
        free_block->size = total_memory_size - next_start;
        free_block->is_free = true;
        free_block->process_id = -1;
        free_block->prev = new_tail;
        free_block->next = nullptr;

        if (new_tail != nullptr)
        {
            new_tail->next = free_block;
        }
        else
        {
            new_head = free_block;
        }
    }
    head = new_head;
}

void display_memory_bar()
{
    Block *current = head;
    size_t used = 0;

    while (current != nullptr)
    {
        if (!current->is_free)
        {
            used += current->size;
        }
        current = current->next;
    }

    double percent = (double)used / total_memory_size * 100;
    int bar_width = 30;
    int filled = (int)(percent / 100 * bar_width);

    cout << "\n[";
    for (int i = 0; i < bar_width; i++)
    {
        if (i < filled)
            cout << "#";
        else
            cout << "-";
    }
    cout << "] " << fixed << setprecision(1) << percent << "% used\n";
    cout << "Used: " << used << " / " << total_memory_size << " bytes\n";
}

void display_stats()
{
    Block *current = head;
    size_t total_free = 0;
    size_t largest_free = 0;
    size_t used_memory = 0;
    int free_blocks = 0;
    int used_blocks = 0;

    while (current != nullptr)
    {
        if (current->is_free)
        {
            total_free += current->size;
            free_blocks++;
            if (current->size > largest_free)
            {
                largest_free = current->size;
            }
        }
        else
        {
            used_memory += current->size;
            used_blocks++;
        }
        current = current->next;
    }

    double external_frag = 0.0;
    if (total_free > 0)
    {
        external_frag = 1.0 - ((double)largest_free / (double)total_free);
    }

    double utilization = (double)used_memory / total_memory_size * 100;

    double success_rate = 0.0;
    if (allocation_attempts > 0)
    {
        success_rate = (double)allocation_successes / allocation_attempts * 100;
    }

    cout << "\n========== MEMORY STATISTICS ==========\n";
    cout << "Total Memory:        " << total_memory_size << " bytes\n";
    cout << "Used Memory:         " << used_memory << " bytes\n";
    cout << "Free Memory:         " << total_free << " bytes\n";
    cout << "Memory Utilization:  " << fixed << setprecision(1) << utilization << "%\n";
    cout << "----------------------------------------\n";
    cout << "Allocated Blocks:    " << used_blocks << "\n";
    cout << "Free Blocks:         " << free_blocks << "\n";
    cout << "Largest Free Block:  " << largest_free << " bytes\n";
    cout << "----------------------------------------\n";
    cout << "External Fragmentation: " << fixed << setprecision(1) << (external_frag * 100) << "%\n";
    cout << "----------------------------------------\n";
    cout << "Allocation Attempts: " << allocation_attempts << "\n";
    cout << "Successful:          " << allocation_successes << "\n";
    cout << "Failed:              " << (allocation_attempts - allocation_successes) << "\n";
    cout << "Success Rate:        " << fixed << setprecision(1) << success_rate << "%\n";
    cout << "========================================\n";
}

void reset_stats()
{
    allocation_attempts = 0;
    allocation_successes = 0;
}