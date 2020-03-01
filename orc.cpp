#include "orc.h"
#include "orcinput.h"

using namespace std;

// #include <iostream>

Orc::Orc() {
    // cout << "Entered default constructor" << endl;
    filename = "EMPTY";
    header = "EMPTY";
    type = OBJ;
    hasEntryPoint = FALSE;
    entryPoint = 0;
    symbolTable.numEntries = 0;
    symbolTable.symbols = NULL;
    relocationTable.numEntries = 0;
    relocationTable.relocations = NULL;
    sectionTable.numEntries = 0;
    sectionTable.section = NULL;
    segmentTable.numEntries = 0;
    segmentTable.segment = NULL;
    contents_size = 0;
    contents = NULL;
    // cout << "Completed default constructor" << endl;
}
Orc::Orc(const string &filename) {
    // cout << "Entered Orc(filename) constructor" << endl;
    Orc in = Orc( OrcInput().getOrcFromFilename(filename) );
    this->filename = filename;
    header = in.header;
    type = in.type;
    hasEntryPoint = in.hasEntryPoint;
    entryPoint = in.entryPoint;

    symbolTable.numEntries = in.symbolTable.numEntries;
    if (symbolTable.numEntries == 0) symbolTable.symbols = NULL;
    else symbolTable.symbols = new Symbol[symbolTable.numEntries];
    for (int i = 0; i < symbolTable.numEntries; i++) {
        symbolTable.symbols[i].name = in.symbolTable.symbols[i].name;
        symbolTable.symbols[i].type = in.symbolTable.symbols[i].type;
        symbolTable.symbols[i].isDefined = in.symbolTable.symbols[i].isDefined;
        symbolTable.symbols[i].section = in.symbolTable.symbols[i].section;
        symbolTable.symbols[i].offset = in.symbolTable.symbols[i].offset;
    }
    
    relocationTable.numEntries = in.relocationTable.numEntries;
    if (relocationTable.numEntries == 0) relocationTable.relocations = NULL;
    else relocationTable.relocations = new Relocation[relocationTable.numEntries];
    for (int i = 0; i < relocationTable.numEntries; i++) {
        relocationTable.relocations[i].offset = in.relocationTable.relocations[i].offset;
        relocationTable.relocations[i].section = in.relocationTable.relocations[i].section;
        relocationTable.relocations[i].symbol = in.relocationTable.relocations[i].symbol;
        relocationTable.relocations[i].plus = in.relocationTable.relocations[i].plus;
    }

    sectionTable.numEntries = in.sectionTable.numEntries;
    if (sectionTable.numEntries == 0) relocationTable.relocations = NULL;
    else sectionTable.section = new Section[sectionTable.numEntries];
    for (int i = 0; i < sectionTable.numEntries; i++) {
        sectionTable.section[i].permissions = in.sectionTable.section[i].permissions;
        sectionTable.section[i].offset = in.sectionTable.section[i].offset;
        sectionTable.section[i].name = in.sectionTable.section[i].name;
        sectionTable.section[i].size = in.sectionTable.section[i].size;
    }

    segmentTable.numEntries = in.segmentTable.numEntries;
    if (segmentTable.numEntries == 0) segmentTable.segment = NULL;
    else segmentTable.segment = new Segment[segmentTable.numEntries];
    for (int i = 0; i < segmentTable.numEntries; i++) {
        segmentTable.segment[i].name = in.segmentTable.segment[i].name;
        segmentTable.segment[i].offset = in.segmentTable.segment[i].offset;
        segmentTable.segment[i].base = in.segmentTable.segment[i].base;
        segmentTable.segment[i].size = in.segmentTable.segment[i].size;
        segmentTable.segment[i].permissions = in.segmentTable.segment[i].permissions;
        segmentTable.segment[i].type = in.segmentTable.segment[i].type;
    }

    contents_size = in.contents_size;
    if (contents_size == 0) contents = NULL;
    else contents = new Byte7[contents_size];
    for (size_t i = 0; i < contents_size; i++) {
        contents[i] = in.contents[i];
    }
    // cout << "Completed Orc(filename) constructor" << endl;
}
Orc::Orc(const Orc &in) {
    // cout << "Entered copy constructor" << endl;
    filename = in.filename;
    header = in.header;
    type = in.type;
    hasEntryPoint = in.hasEntryPoint;
    entryPoint = in.entryPoint;

    symbolTable.numEntries = in.symbolTable.numEntries;
    if (symbolTable.numEntries == 0) symbolTable.symbols = NULL;
    else symbolTable.symbols = new Symbol[symbolTable.numEntries];
    for (int i = 0; i < symbolTable.numEntries; i++) {
        symbolTable.symbols[i].name = in.symbolTable.symbols[i].name;
        symbolTable.symbols[i].type = in.symbolTable.symbols[i].type;
        symbolTable.symbols[i].isDefined = in.symbolTable.symbols[i].isDefined;
        symbolTable.symbols[i].section = in.symbolTable.symbols[i].section;
        symbolTable.symbols[i].offset = in.symbolTable.symbols[i].offset;
    }

    relocationTable.numEntries = in.relocationTable.numEntries;
    if (relocationTable.numEntries == 0) relocationTable.relocations = NULL;
    else relocationTable.relocations = new Relocation[relocationTable.numEntries];
    for (int i = 0; i < relocationTable.numEntries; i++) {
        relocationTable.relocations[i].offset = in.relocationTable.relocations[i].offset;
        relocationTable.relocations[i].section = in.relocationTable.relocations[i].section;
        relocationTable.relocations[i].symbol = in.relocationTable.relocations[i].symbol;
        relocationTable.relocations[i].plus = in.relocationTable.relocations[i].plus;
    }

    sectionTable.numEntries = in.sectionTable.numEntries;
    if (sectionTable.numEntries == 0) relocationTable.relocations = NULL;
    else sectionTable.section = new Section[sectionTable.numEntries];
    for (int i = 0; i < sectionTable.numEntries; i++)
    {
        sectionTable.section[i].permissions = in.sectionTable.section[i].permissions;
        sectionTable.section[i].offset = in.sectionTable.section[i].offset;
        sectionTable.section[i].name = in.sectionTable.section[i].name;
        sectionTable.section[i].size = in.sectionTable.section[i].size;
    }

    segmentTable.numEntries = in.segmentTable.numEntries;
    if (segmentTable.numEntries == 0) segmentTable.segment = NULL;
    else segmentTable.segment = new Segment[segmentTable.numEntries];
    for (int i = 0; i < segmentTable.numEntries; i++)
    {
        segmentTable.segment[i].name = in.segmentTable.segment[i].name;
        segmentTable.segment[i].offset = in.segmentTable.segment[i].offset;
        segmentTable.segment[i].base = in.segmentTable.segment[i].base;
        segmentTable.segment[i].size = in.segmentTable.segment[i].size;
        segmentTable.segment[i].permissions = in.segmentTable.segment[i].permissions;
        segmentTable.segment[i].type = in.segmentTable.segment[i].type;
    }

    contents_size = in.contents_size;
    if (contents_size == 0) contents = NULL;
    else contents = new Byte7[contents_size];
    for (size_t i = 0; i < contents_size; i++) {
        contents[i] = in.contents[i];
    }
    // cout << "Completed copy constructor" << endl;
}
Orc::~Orc() {
    // cout << "Entered destructor" << endl;
    if (symbolTable.numEntries > 0)         delete [] symbolTable.symbols;
    if (relocationTable.numEntries > 0)     delete [] relocationTable.relocations;
    if (sectionTable.numEntries > 0)        delete [] sectionTable.section;
    if (segmentTable.numEntries > 0)        delete [] segmentTable.segment;
    if (contents_size > 0)                  delete [] contents;
    // cout << "Completed destructor" << endl;
}