#ifndef _ORCRUNNER_
#define _ORCRUNNER_

#include "orcloader.h"

// OrcRunner (current file)
#include <fstream>
#include <unordered_map>
#include <bitset>
#include <vector>

class OrcIsObject{};

#define Opcode unsigned char
enum type1_opcode : unsigned char { ADD, SUB, MUL, DIV, CMP, TST, AND, ORR, XOR, STR, STB, LOD };
enum type2_opcode : unsigned char { JMP, JLT, JEQ, CAL, PSH, POP, NOT, OUT, INP, AMP, ALT, AEQ, AAL };
enum type3_opcode : unsigned char { RET, NOP };

struct Register {
    unsigned char type;
    Opcode opcode; // can be initialized to "type1_opcode," "type2_opcode," "type3_opcode"
            // type 1
    unsigned char dst;  // destination
    unsigned char src;  // source
            // type 2
    unsigned short val; // value
};
struct Constant {
    unsigned char type;
    Opcode opcode;
            // type 1
    unsigned char dst;  // destination
    unsigned short src; // source
            // type 2
    unsigned short val;  // value
};

struct permissions {
    Byte7 byte7;
    bool readable()   const { return (bool)((byte7>>6)&1); }
    bool writeable()  const { return (bool)((byte7>>5)&1); }
    bool executable() const { return (bool)((byte7>>4)&1); }
};
std::ostream & operator << (std::ostream &out, const permissions & p);
struct memory_value {
        Byte7 value;
        permissions p;
        // Byte7 a = memory_value();
        operator Byte7() const {return value;}
};
std::ostream & operator << (std::ostream &out, const memory_value & m);

class OrcRunner {

    public:
        OrcRunner();
        OrcRunner(const Orc & orc);

    private:
        friend class OrcLoader;
        friend class Orc;

        void printListOfStrings() const;

        void execute() ;
        
        size_t entryPoint;
        std::unordered_map<size_t,memory_value> MEMORY;
        std::string filename;

        /*
        this is a vector containing a list of instrution strings in format:

        0101000
        0000000
        0000000
        0110111
        0000000
        ...
        */
        std::vector<std::string> listOfInstructionStrings;


};


#endif
