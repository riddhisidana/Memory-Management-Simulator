#include "../include/cache/cache.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

void printCacheHelp()
{
    cout << R"(
================ CACHE SIMULATOR ================
Commands:
    init <levels>      - Initialize multilevel cache (2 or 3 levels)
    config <level> <size_kb> <block_size> <ways> - Configure cache level
    access <address>   - Access memory address
    display            - Show cache contents
    stats              - Show multilevel statistics
    clear              - Clear all cache levels
    mode               - Return to mode selection menu
    help               - Show help
    quit               - Exit program completely
=================================================
)";
}

void printcachebanner()
{
    cout << R"(
        === Memory Management Simulator ===
                !!! Cache Mode !!!
              OS Project - ACM

Type 'help' for available commands.
)" << endl;
}

int run_cache_mode()
{
    printcachebanner();
    // Initialize with default 2-level cache
    init_multilevel_cache(2);

    string command;
    while (true)
    {
        cout << "\ncache> ";
        getline(cin, command);

        if (command.empty())
            continue;

        stringstream ss(command);
        string cmd;
        ss >> cmd;

        if (cmd == "init")
        {
            int levels;
            if (ss >> levels && levels >= 1 && levels <= 3)
            {
                init_multilevel_cache(levels);
                cout << "Initialized " << levels << "-level cache hierarchy!" << endl;
            }
            else
            {
                cout << "Usage: init <levels> (1-3)" << endl;
            }
        }
        else if (cmd == "config")
        {
            int level, size, block, ways;
            if (ss >> level >> size >> block >> ways && level >= 1 && level <= num_levels)
            {
                if (size > 0 && block > 0 && ways > 0)
                {
                    cout << "L" << level << " config: "
                         << cache_levels[level - 1].cache_size_kb << "KB, "
                         << cache_levels[level - 1].block_size << "B, "
                         << cache_levels[level - 1].associativity << "-way -> ";
                    configure_level(level, size, block, ways);
                    cout << size << "KB, " << block << "B, " << ways << "-way" << endl;
                }
                else
                {
                    cout << "Invalid values! All parameters must be > 0" << endl;
                }
            }
            else
            {
                cout << "Usage: config <level> <size_kb> <block_size> <associativity>" << endl;
                cout << "Example: config 1 64 64 2" << endl;
            }
        }
        else if (cmd == "access")
        {
            unsigned long address;
            if (ss >> address)
            {
                bool hit = multilevel_access(address);
                cout << "Access to 0x" << hex << address << dec << ": " << (hit ? "HIT" : "MISS") << endl;
            }
            else
            {
                cout << "Usage: access <address>" << endl;
            }
        }
        else if (cmd == "display")
        {
            cout << "Multilevel cache with " << num_levels << " levels:" << endl;
            for (int i = 0; i < num_levels; i++)
            {
                cout << "L" << (i + 1) << ": " << cache_levels[i].cache_size_kb << "KB, "
                     << cache_levels[i].block_size << "B, " << cache_levels[i].associativity << "-way" << endl;
            }
        }
        else if (cmd == "stats")
        {
            display_multilevel_stats();
        }
        else if (cmd == "clear")
        {
            clear_multilevel_cache();
            cout << "All cache levels cleared!" << endl;
        }
        else if (cmd == "mode")
        {
            cout << "Returning to main menu..." << endl;
            return 0;
        }
        else if (cmd == "help")
        {
            printCacheHelp();
        }
        else if (cmd == "quit")
        {
            cout << "Goodbye!\n";
            exit(2); // Exit entire program
        }
        else
        {
            cout << "Unknown command. Type 'help' for available commands." << endl;
        }
    }

    return 0;
}