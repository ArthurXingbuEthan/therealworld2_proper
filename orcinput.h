#ifndef _ORCINPUT_
#define _ORCINPUT_

// Orc Forward Declarations
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

// OrcInput (current file)
class NoMoreByte7s{};
class BadFileName{};

#include <fstream>
#include <string>

class OrcInput {
    public:
        OrcInput() { MI = 0; }
    private:
        friend class Orc;

        Orc getOrcFromFilename(const std::string & filename);
        unsigned int MI;
        std::string * F;
        size_t F_size;

        std::string filename;
        std::ifstream ifs;
        std::ofstream ofs;

        std::string reverseString(std::string s);
        unsigned char getCharacterFrom7Bits(std::string s);
        unsigned short getWord28();
        std::string getText7();
        unsigned char getByte7();
        Permissions getPermissions();
        Symbol getSymbol();
        SymbolTable getSymbolTable();
        Relocation getRelocation();
        RelocationTable getRelocationTable();
        Section getSection();
        SectionTable getSectionTable();
        Segment getSegment();
        SegmentTable getSegmentTable();

};


#endif