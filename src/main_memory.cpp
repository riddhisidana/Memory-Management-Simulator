#include "../include/contiguous/contiguous.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip>
using namespace std;

void printHelp()
{
    cout << R"(
================ MEMORY SIMULATOR ================
Commands:
  malloc              - Allocate memory (asks for size & PID)
  free                - Free memory by process ID
  dump                - Show memory layout
  stats               - Show fragmentation statistics
  getstrategy         - Show current allocation strategy
  setstrategy         - Change allocation strategy
  processes           - Show all active processes
  compact             - Compact memory (defragment)
  bar                 - Show memory usage bar
  clear               - Reset memory to initial state
  mode                - Return to mode selection menu
  help                - Show this help
  quit                - Exit simulator
=================================================
)";
}

void printBanner()
{
    cout << R"(  
        === Memory Management Simulator ===
               !!! Contiguous Mode !!!
                  OS Project - ACM

Type 'help' for available commands.
)" << endl;
}

int run_memory_mode()
{
    printBanner();

    size_t mem_size;
    cout << "Enter total memory sizes (bytes): ";
    cin >> mem_size;

    int choice;
    cout << "Choose strategy (1=First Fit, 2=Best Fit, 3=Worst Fit): ";
    cin >> choice;

    AllocationStrategy strategy;
    if (choice == 1)
        strategy = AllocationStrategy::first_fit;
    else if (choice == 2)
        strategy = AllocationStrategy::best_fit;
    else
        strategy = AllocationStrategy::worst_fit;

    init_memory(mem_size, strategy);

    string command;
    while (true)
    {
        cout << "\nmem> ";
        cin >> command;

        if (command == "malloc")
        {
            int size;
            int process_id;
            cout << "Enter size and PID (separated by space): ";
            cin >> size >> process_id;
            Block *result = allocate(size, process_id);
            if (result != nullptr)
            {
                cout << "Allocated " << size << " bytes for process " << process_id << ". Address: 0x" << hex << result->start << dec << endl;
            }
            else
            {
                cout << "Allocation failed! Not enough memory." << endl;
            }
        }
        else if (command == "free")
        {
            int process_id;
            cout << "Enter PID : ";
            cin >> process_id;
            deallocate(process_id);
        }
        else if (command == "dump")
        {
            display_memory();
        }
        else if (command == "help")
        {
            printHelp();
        }
        else if (command == "mode")
        {
            cout << "Returning to mode selection menu...\n";
            shutdown_memory();
            return 0; // Return to main menu
        }
        else if (command == "getstrategy")
        {
            AllocationStrategy curr = get_strategy();
            cout << "Current strategy: ";
            if (curr == AllocationStrategy::first_fit)
                cout << "First Fit\n";
            else if (curr == AllocationStrategy::best_fit)
                cout << "Best Fit\n";
            else
                cout << "Worst Fit\n";
        }
        else if (command == "setstrategy")
        {
            int strat_choice;
            cout << "Choose strategy (1=First Fit, 2=Best Fit, 3=Worst Fit): ";
            cin >> strat_choice;

            if (strat_choice == 1)
            {
                set_strategy(AllocationStrategy::first_fit);
                cout << "Strategy changed to First Fit\n";
            }
            else if (strat_choice == 2)
            {
                set_strategy(AllocationStrategy::best_fit);
                cout << "Strategy changed to Best Fit\n";
            }
            else if (strat_choice == 3)
            {
                set_strategy(AllocationStrategy::worst_fit);
                cout << "Strategy changed to Worst Fit\n";
            }
        }
        else if (command == "clear")
        {
            shutdown_memory();
            init_memory(mem_size, get_strategy());
            reset_stats();
            cout << "Memory cleared!\n";
        }
        else if (command == "processes")
        {
            display_processes();
        }
        else if (command == "compact")
        {
            compact_memory();
            cout << "Memory compacted!\n";
        }
        else if (command == "bar")
        {
            display_memory_bar();
        }
        else if (command == "stats")
        {
            display_stats();
        }
        else if (command == "quit")
        {
            break;
        }
        else
        {
            cout << "Unknown command. Type 'help' for available commands.\n";
        }
    }

    shutdown_memory();
    cout << "Goodbye!\n";
    return 0;
}