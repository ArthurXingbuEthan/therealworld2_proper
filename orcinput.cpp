#include "orc.h"

#include <bitset>
#include <vector>
#include <iostream>

using namespace std;

string OrcInput::reverseString(string s) {
  string out = "";
  for (int i = s.size() - 1; i >= 0; i--)
  {
    out += s[i];
  }
  return out;
}
unsigned char OrcInput::getCharacterFrom7Bits(string s) {
  int backtodec = 0;
  if(s[6] == '1') backtodec += 1;
  if(s[5] == '1') backtodec += 2;
  if(s[4] == '1') backtodec += 4;
  if(s[3] == '1') backtodec += 8;
  if(s[2] == '1') backtodec += 16;
  if(s[1] == '1') backtodec += 32;
  if(s[0] == '1') backtodec += 64;
  int ascii = stoi(s);
  return (char)backtodec;
}

unsigned short OrcInput::getWord28() {
    int out = 0;
    for (unsigned char i = 0; i < 4; i++)
        out = (out >> 7) | getByte7()<<21;
    return out;
}

string OrcInput::getText7() {
    string out = "";
    while (F[MI]!="0000000") {
       out += getByte7();
    }
    MI++;
    return out;
}
unsigned char OrcInput::getByte7() {
    if ( MI >= F_size ) throw NoMoreByte7s();
    char c =  getCharacterFrom7Bits(reverseString( F[MI++] ));
    return c;
}

Permissions OrcInput::getPermissions() {
    Permissions p;
    p.byte7 = getByte7();
    ofs << "Permissions: ";
    if (p.readable()) ofs << "READ ";
    if (p.writeable()) ofs << "WRIT ";
    if (p.executable()) ofs << "EXEC ";
    ofs << endl;
    return p;
}

Symbol OrcInput::getSymbol() {
    Symbol s;

    s.name = getText7();
    ofs << "   Symbol Name: " << s.name << endl;
    s.type = (SymbolType)getByte7();
    ofs << "   Symbol Type: " << s.type << endl;
    s.isDefined = (Bool)getByte7();
    ofs << "   Symbol Defined: " << s.isDefined << endl;
    if (s.isDefined) {
        s.section = getWord28();
        ofs << "       Symbol Section: " << s.section << endl;
        s.offset = getWord28();
        ofs << "       Symbol Offset: " << s.offset << endl;
    }

    return s;

}

SymbolTable OrcInput::getSymbolTable() {
    SymbolTable st;
    st.numEntries = getWord28();
    ofs << "SymbolTable numEntries: " <<  st.numEntries << endl;

    st.symbols = new Symbol[st.numEntries];
    for (Word28 i = 0; i < st.numEntries; i++)
        st.symbols[i] = Symbol(getSymbol());

    return st;
}

Relocation OrcInput::getRelocation() {
    Relocation r;

    r.offset = getWord28();
    ofs << "   Relocation Offset: " << r.offset << endl;
    r.section = getWord28();
    ofs << "   Relocation selection: " << r.section << endl;
    r.symbol = getText7();
    ofs << "   Relocation symbol: " << r.symbol << endl;
    r.plus = getWord28();
    ofs << "   Relocation plus: " << r.plus << endl;
    return r;

}

RelocationTable OrcInput::getRelocationTable() {
    RelocationTable rt;

    rt.numEntries = getWord28();
    ofs << "Relocation Table Entries: " << rt.numEntries << endl;

    for (Word28 i = 0; i < rt.numEntries; i++)
        rt.relocations[i] = Relocation( getRelocation() );

    return rt;
}
Section OrcInput::getSection() {
    Section s;
    ofs << "   Section getPermissions:    ";
    s.permissions =  getPermissions();
    s.offset = getWord28();
    ofs << "   Section offset: " << s.offset << endl;
    s.name = getText7();
    ofs << "   Section name: " << s.name << endl;
    s.size = getWord28();
    ofs << "   Section size: " << s.size << endl;
    return s;
}
SectionTable OrcInput::getSectionTable() {
    SectionTable st;

    st.numEntries = getWord28();
    ofs << "SectionTable numEntries: " << st.numEntries << endl;
    st.section = new Section[ st.numEntries ];
    for (Word28 i = 0; i < st.numEntries; i++)
        st.section[i] = Section(  getSection() );
    ofs << "SectionTable numEntries: " << st.numEntries <<"    MI: " << MI<< endl;
    return st;
}
Segment OrcInput::getSegment() {
    Segment s;

    s.name = getText7();
    ofs << "   Segment Name: " << s.name << endl;
    s.offset  = getWord28();
    ofs << "   Segment offset: " << s.offset << endl;
    s.base = getWord28();
    ofs << "   Segment base: " << s.base << endl;
    s.size = getWord28();
    ofs << "   Segment size: " << s.size << endl;

    ofs << "   Segment getPermissions:    ";
    s.permissions = getPermissions();
    s.type = (SegmentType)getByte7();
    
    if(s.type == 1)
        ofs << "   Segment type: NOTE" << endl;
    else
        ofs << "   Segment type: PROGBITS" << endl;
    return s;
}
SegmentTable OrcInput::getSegmentTable() {
    SegmentTable st;

    st.numEntries = getWord28();
    ofs << "SegmentTable numEntries: " << st.numEntries << endl;
    st.segment = new Segment[ st.numEntries ];
    for (Word28 i = 0; i < st.numEntries; i++)
        st.segment[i] = Segment(  getSegment() );

    return st;
}


Orc OrcInput::getOrcFromFilename(const string & filename) {
    Orc orc;
    this->filename = filename;
    ifs.open(filename);
    if (!ifs) {cerr << "file \"" << filename << "\" does not exist\n"; throw BadFileName();}
    string line;
    vector<string> F_temp;
    while (getline(ifs,line)) {
        if(line == "---") break;
        F_temp.push_back(line);
    }
    ifs.close();

    F_size = F_temp.size();
    F = new string[F_size];
    for (size_t i = 0; i < F_size; i++) F[i] = F_temp[i];

    ofs.open(filename+".orc.txt");

    orc.header = getText7();
    ofs << "Header: " << orc.header << endl;
    orc.type = (FileType)getByte7();
    ofs << "FileType: "<< (FileType)orc.type << endl;
    orc.hasEntryPoint = (Bool)getByte7();
    ofs << "hasEntryPoint: " << (int)orc.hasEntryPoint << endl;
    if (orc.hasEntryPoint) {
        orc.entryPoint = 0;
        orc.entryPoint = getWord28();
        ofs << "EntryPoint: " << orc.entryPoint << endl;
    }
        

    orc.symbolTable = SymbolTable( getSymbolTable() );

    orc.relocationTable = RelocationTable( getRelocationTable() );

    orc.sectionTable = SectionTable( getSectionTable() );

    orc.segmentTable = SegmentTable( getSegmentTable() );

    ofs << "\nCONTENTS:\n\n";

    orc.contents = new Byte7[ F_size - MI ];
    try { for (orc.contents_size = 0; true; orc.contents_size++) {
            orc.contents[orc.contents_size] = getByte7();
            ofs << ((orc.contents[orc.contents_size]!=0)?(char)orc.contents[orc.contents_size]:' ') << " : " << bitset<7>(orc.contents[orc.contents_size]).to_string() << endl;
    }}
    catch ( NoMoreByte7s e ) {}
    orc.contents_size--;

    ofs << "\nsize: " << orc.contents_size;
    
    ofs.close();

    return orc;

}