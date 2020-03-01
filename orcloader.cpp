#include "orc.h"

#include <iostream>

using namespace std;

// DOES NOT INCLUDE SYMBOL, SECTION, 
// copy entryPoint and copy all Orc into memory
OrcRunner OrcLoader::getOrcRunner(const Orc & orc) {
    // cout << "Entering OrcLoader::getOrcRunner(Orc)\n";
    OrcRunner R;

    R.entryPoint = orc.entryPoint;
    
    for (size_t E = 0; E < orc.segmentTable.numEntries; E++) 
        for (size_t O = 0; O < orc.segmentTable.segment[E].size; O++) {
            R.MEMORY[orc.segmentTable.segment[E].base + O].value    = orc.contents[orc.segmentTable.segment[E].offset + O];
            R.MEMORY[orc.segmentTable.segment[E].base + O].p.byte7  = orc.segmentTable.segment[E].permissions.byte7;
        }

    filename = orc.getFilename();
    // cout << "Exiting OrcLoader::getOrcRunner(Orc)\n";
    return R;
}
