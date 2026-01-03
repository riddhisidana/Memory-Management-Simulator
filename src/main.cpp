#include <iostream>
#include <string>
using namespace std;

// Forward declarations
int run_memory_mode();
int run_buddy_mode();
int run_cache_mode();

void modechanger()
{
    cout << R"(Choose a mode:
1. Memory Allocation Simulator (First/Best/Worst Fit)
2. Buddy System Simulator
3. Cache Simulator
4. Quit
)";
}

int main()
{
    cout << R"(
 __  __                                 __  __                                   
|  \/  | ___ _ __ ___   ___  _ __ _   _|  \/  | __ _ _ __   __ _  __ _  ___ _ __ 
| |\/| |/ _ \ '_ ` _ \ / _ \| '__| | | | |\/| |/ _` | '_ \ / _` |/ _` |/ _ \ '__|
| |  | |  __/ | | | | | (_) | |  | |_| | |  | | (_| | | | | (_| | (_| |  __/ |   
|_|  |_|\___|_| |_| |_|\___/|_|   \__, |_|  |_|\__,_|_| |_|\__,_|\__, |\___|_|   
                                  |___/                          |___/           
  ____  _                 _       _             
 / ___|(_)_ __ ___  _   _| | __ _| |_ ___  _ __ 
 \___ \| | '_ ` _ \| | | | |/ _` | __/ _ \| '__|
  ___) | | | | | | | |_| | | (_| | || (_) | |   
 |____/|_|_| |_| |_|\__,_|_|\__,_|\__\___/|_|   
                                                
        === Memory Management Simulator ===
              OS Project - ACM
Type 'help' for available commands.
)" << endl;

    while (true)
    {
        modechanger();
        cout << "Enter choice (1-4): ";
        int choice;
        cin >> choice;

        if (choice == 1)
        {
            cout << "Entering Memory Allocation Simulator...\n";
            run_memory_mode();
        }
        else if (choice == 2)
        {
            cout << "Entering Buddy System Simulator...\n";
            run_buddy_mode();
        }
        else if (choice == 3)
        {
            cout << "Entering Cache Simulator...\n";
            run_cache_mode();
        }
        else if (choice == 4)
        {
            cout << "Goodbye!\n";
            break;
        }
        else
        {
            cout << "Invalid choice. Please enter 1, 2, 3, or 4.\n";
        }
    }

    return 0;
}