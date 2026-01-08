#include "../include/buddy/buddy.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

void printBuddyHelp()
{
    cout << R"(
================ BUDDY SYSTEM SIMULATOR ================
Commands:
    malloc     - Allocate memory (asks for size & PID)
    free       - Free memory by process ID
    display    - Show buddy system free lists
    dump       - Show all memory blocks (allocated and free)
    stats      - Show buddy system statistics
    clear      - Reset buddy allocator (keep same total memory)
    mode       - Return to mode selection menu
    help       - Show this help
    quit       - Exit program completely
========================================================
)";
}

void printBuddyBanner()
{
    cout << R"(
        === Memory Management Simulator ===
                !!! Buddy Mode !!!
              OS Project - ACM

Type 'help' for available commands.
)" << endl;
}
int run_buddy_mode()
{
    printBuddyBanner();

    size_t mem_size;
    cout << R"(
Made for exact power of 2.
# Enter 1000 it gets rounded to 1024
# Enter 150 it gets rounded to 256
Enter total memory sizes (bytes): )";
    cin >> mem_size;
    buddy_init(mem_size);
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
            void *result = buddy_allocate(size, process_id);
            if (result != nullptr)
            {
                cout << "Allocated " << size << " bytes for process " << process_id << ". Address: " << result << endl;
            }
            else
            {
                cout << "Allocation failed! Not enough memory or duplicate PID." << endl;
            }
        }
        else if (command == "display")
        {
            buddy_display();
        }
        else if (command == "stats")
        {
            buddy_stats();
        }
        else if (command == "free")
        {
            int process_id;
            cout << "Enter PID: ";
            cin >> process_id;
            bool success = buddy_deallocate(process_id);
            if (success)
                cout << "Deallocated memory for process " << process_id << endl;
            else
                cout << "Error: No such process ID found." << endl;
        }
        else if (command == "dump")
        {
            buddy_dump();
        }
        else if (command == "clear")
        {
            buddy_reset();
            cout << "Buddy allocator reset.\n";
        }
        else if (command == "quit")
        {
            buddy_shutdown();
            cout << "Goodbye!\n";
            exit(2); // Exit entire program
        }
        else if (command == "mode")
        {
            cout << "Returning to mode selection menu...\n";
            buddy_shutdown();
            return 0; // Return to main menu
        }
        else if (command == "help")
        {
            printBuddyHelp();
        }
        else
        {
            cout << "Unknown command. Type 'help' for available commands.\n";
        }
    }

    buddy_shutdown();
    cout << "Goodbye!\n";
    return 0;
}