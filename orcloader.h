#ifndef _ORCLOADER_
#define _ORCLOADER_

class OrcRunner;

// Orc Forward Declarations
#define Byte7 unsigned char
struct Permissions;
struct Symbol;
struct SymbolTable;
struct Section;
struct SectionTable;
struct Segment;
struct SegmentTable;
struct Relocation;
struct RelocationTable;
class Orc;


// OrcLoader (current file)
#include <fstream>
#include <string>
#include <vector>

class OrcLoader {
    public:
        OrcRunner getOrcRunner(const Orc & orc);
    private:
        std::string filename;
};

#endif