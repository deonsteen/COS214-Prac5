#include <iostream>
#include "CampusGuardSystem.h"

int main() {
    {
        CampusGuardSystem system;
        system.runScenarioOne();
        system.runScenarioTwo();
    } 
    std::cout << "\n------------CampusGuard shut down cleanly------------\n";
    return 0;
}
