#include "orc.h"
// JANK FIX: Please find better way unless this is best
#include "orcloader.cpp"

#include <iostream>
#include <vector>

typedef unordered_map<size_t,memory_value>::const_iterator map_iter;

using namespace std;
using std::cout;
using std::endl;

struct instruction{
    public:
    std::string value;
    int type;
    std::string firstcode;
    std::string opcode;
    std::string name;
    std::string src;
    std::string dest;
    std::string val;
    std::string toString(){
        //return (this->name + "+" + this->value);
        std::string out;
        out += this->name + "+" + this->value;
        out += "\nTYPE: " + to_string(this->type) + ", opcode:" + this->opcode;
        out += this->type == 1 ? ("\nsrc: " + this->src + ", dest:" + this->dest + "\n") : "";
        out += this->type == 2 ? ("\nval: " + this->val + "\n") : "";
        if(this->type == 1 && this->name == "ORR"){
            out+="char: ";
            out+=(char)(stoi(this->src.substr(0,7), NULL, 2));
            out+= "\n";
        }
        return out;

    }
};

int* registers;
vector<int> stack;
size_t currentIndex;
std::string our_stdout;

void printRegisters(){
    cout << "registers:\n";
    for(int i = 0; i < 11; i++){
        cout << i << ":[" << registers[i] << "]\n";
    }
    cout << endl;
}

void executeAdd(instruction i){

}
void executeSub(instruction i){

}
void executeMul(instruction i){

}
void executeDiv(instruction i){

}
void executeCmp(instruction i){

}
void executeTst(instruction i){

}
void executeAnd(instruction i){

}
void executeOrr(instruction i){
    //cout << "executing orr" << endl;
    if(i.firstcode == "010"){
        registers[stoi(i.dest,NULL,2)] = registers[stoi(i.dest, NULL,2)] | registers[stoi(i.src, NULL, 2)];
    }
    else if(i.firstcode == "011"){
        //cout << i.src << endl;

        //Arthur - 
        //why is it the substring of 0 to 7? IDK, but the instruction is 
        //ORR+011011100000001101100000000000000000000000
        //src is 1101100000000000000000000000, which we need to do a bitwise OR on. I think we are supposed to look at the first byte?
        //it works if you only look at the first 7 characters.
        registers[stoi(i.dest,NULL,2)] = registers[stoi(i.dest, NULL,2)] | stoi(i.src.substr(0,7), NULL, 2);
    }
    else{
        cout << "problem with executeOrr" << endl;
    }

}
void executeXor(instruction i){
    //cout << "executing xor" << endl;
    if(i.firstcode == "010"){
        cout << "in dest: " << registers[stoi(i.dest, NULL,2)];
        cout << "in src: " << registers[stoi(i.src, NULL, 2)];
        cout << "0 and 0: " << to_string(0 ^ 0) ;
        cout << "xor value: " << to_string(registers[stoi(i.dest, NULL,2)] ^ registers[stoi(i.src, NULL, 2)]);
        registers[stoi(i.dest,NULL,2)] = registers[stoi(i.dest, NULL,2)] ^ registers[stoi(i.src, NULL, 2)];
    }
    else if(i.firstcode == "011"){
        registers[stoi(i.dest,NULL,2)] = registers[stoi(i.dest, NULL,2)] ^ stoi(i.src, NULL, 2);
    }
    else{
        cout << "problem with executeXor" << endl;
    }

}
void executeStr(instruction i){

}
void executeStb(instruction i){

}
void executeLod(instruction i){

}
void executeJmp(instruction i){

}
void executeJlt(instruction i){

}
void executeJeq(instruction i){

}
//returns the number of lines we are jumping, this value gets added to returnValue in interpretAndWrite()'
int executeCal(instruction i){
    //registers[9] is value of pc
    stack.push_back(registers[9] + i.value.length());
    //decrements pc by 4;
    registers[10] -= 4;
    int jumpBy = stoi(i.val.substr(0,7), NULL, 2);

    //this might solve out problem is Colin made a mistake and 0101000 should be 0010100 instead
    
    //jumpBy = jumpBy >> 1;

    //decrement the size of instruction itself
    
    //if(i.firstcode == "101") jumpBy -= 5; // -5 because if firstCode is 101, then instruction is a 35-bit long string, therefore subtract 5

    jumpBy = 12;

    cout << "in executCal, jumping " << jumpBy<< " lines\n";
    return jumpBy;
}
void executePsh(instruction i){


}
void executePop(instruction i){

}
void executeNot(instruction i){

}
void executeOut(instruction i){
    //cout << "executing out" << endl;
    if(i.firstcode == "100"){
        //cout << stoi(i.val, NULL, 2);
        //cout << registers[0];
        //cout << (char)registers[stoi(i.val, NULL, 2)] << endl;
        cout << "adding: " << (char)registers[stoi(i.val, NULL, 2)] << " to std_out";
        our_stdout += (char)registers[stoi(i.val, NULL, 2)];
    }
    else if(i.firstcode == "101"){
        //might be wrong, i dont really care
        cout << "adding: " << (char)stoi(i.val, NULL, 2) << " to std_out";
        our_stdout += (char)stoi(i.val, NULL, 2);
    }
    else{
        cout << "problem with executeOut" << endl;
    }

}
void executeInp(instruction i){

}
void executeAmp(instruction i){

}
void executeAlt(instruction i){

}
void executeAeq(instruction i){

}
int executeAal(instruction i){
    return executeCal(i);
}
void executeRet(instruction i){

}
void executeNop(instruction i){

}



void OrcRunner::printListOfStrings() const{
    ofstream printListOfInstrBinary;
    printListOfInstrBinary.open("instrBinary.txt");
    for(size_t i = 0; i <this->listOfInstructionStrings.size(); i++){
        //cout << this->listOfInstructionStrings[i] << endl;
        printListOfInstrBinary << this->listOfInstructionStrings[i] << endl;
    }
    printListOfInstrBinary.close();
}

ostream & operator << (ostream &out, const permissions & p) {
        out << ( (p.readable())?"Re":"  " ) << ( (p.writeable())?"Wr":"  " ) << ( (p.executable())?"Ex":"  " );
        return out;
}
ostream & operator << (ostream &out, const memory_value & m) {
        out << ((m.value!=0)?(char)m.value:' ') << " : " << bitset<7>(m.value).to_string() << " : " << m.p;
        return out;
}

OrcRunner::OrcRunner() { }

OrcRunner::OrcRunner(const Orc & orc) {
    OrcRunner in = OrcLoader().getOrcRunner(orc);
    MEMORY = unordered_map<size_t,memory_value>(in.MEMORY);
    entryPoint = in.entryPoint;

    filename = orc.getFilename();
}

instruction interpretInstructionType1(std::string fullinstruction){
    instruction i;
    i.value = fullinstruction;
    i.type = 1;
    i.firstcode = fullinstruction.substr(0,3);
    i.opcode = fullinstruction.substr(3,4);
    switch(stoi(i.opcode, NULL,2)){
        case 0:
            i.name = "ADD";
            break;
        case 1:
            i.name = "SUB";
            break;
        case 2:
            i.name = "MUL";
            break;
        case 3:
            i.name = "DIV";
            break;
        case 4:
            i.name = "CMP";
            break;
        case 5:
            i.name = "TST";
            break;
        case 6:
            i.name = "AND";
            break;
        case 7:
            i.name = "ORR";
            break;
        case 8:
            i.name = "XOR";
            break;
        case 9:
            i.name = "STR";
            break;
        case 10:
            i.name = "STB";
            break;
        case 11:
            i.name = "LOD";
            break;
        default:
            i.name = "NAN";
            break;
        }
    if(i.firstcode == "010"){  
        i.src = fullinstruction.substr(7,4);
        i.dest = fullinstruction.substr(11,4);
    }
    else if(i.firstcode == "011"){
        i.src = fullinstruction.substr(14);
        i.dest = fullinstruction.substr(7,4);
    }
    return i;
    
}

instruction interpretInstructionType2(std::string fullinstruction){
    instruction i;
    i.value = fullinstruction;
    i.type = 2;
    i.firstcode = fullinstruction.substr(0,3);
    i.opcode = fullinstruction.substr(3,4);
    switch(stoi(i.opcode, NULL,2)){
        case 0:
            i.name = "JMP";
            break;
        case 1:
            i.name = "JLT";
            break;
        case 2:
            i.name = "JEQ";
            break;
        case 3:
            i.name = "CAL";
            break;
        case 4:
            i.name = "PSH";
            break;
        case 5:
            i.name = "POP";
            break;
        case 6:
            i.name = "NOT";
            break;
        case 7:
            i.name = "OUT";
            break;
        case 8:
            i.name = "INP";
            break;
        case 9:
            i.name = "AMP";
            break;
        case 10:
            i.name = "ALT";
            break;
        case 11:
            i.name = "AEQ";
            break;
        case 12:
            i.name = "AAL";
            break;
        default:
            i.name = "NAN";
            break;
        }
    if(i.firstcode == "100"){  
        i.val = fullinstruction.substr(7,4);
    }
    else if(i.firstcode == "101"){
        i.val = fullinstruction.substr(7);
    }
    return i;
    
}

//function returns where the next starting location should be
size_t interpretAndWrite(std::string firstLineOfOper, size_t indexInMem, std::unordered_map<size_t,memory_value>& mem, ofstream& printer){
    cout << "in interpret and write, indexInMem: " << indexInMem << endl;



    
    instruction instr;
    bool instructionParsed = 0;
    size_t returnValue = indexInMem;


    if(firstLineOfOper == ""){
        cout << "in interpretAndWrite, instruction string is NULL";
        return -1;
    }
    std::string firstCode = firstLineOfOper.substr(0,3);
    if(firstCode == "010"){
        std::string fullInstr = bitset<7>(mem.at(indexInMem).value).to_string() + bitset<7>(mem.at(indexInMem+1).value).to_string() + bitset<7>(mem.at(indexInMem+2).value).to_string();
        returnValue += 3;
        instr = interpretInstructionType1(fullInstr);
        instructionParsed = 1;
    }
    else if(firstCode == "011"){
        std::string fullInstr = bitset<7>(mem.at(indexInMem).value).to_string() + bitset<7>(mem.at(indexInMem+1).value).to_string() + bitset<7>(mem.at(indexInMem+2).value).to_string() + bitset<7>(mem.at(indexInMem+3).value).to_string() + bitset<7>(mem.at(indexInMem+4).value).to_string() + bitset<7>(mem.at(indexInMem+5).value).to_string();
        returnValue += 6;
        instr = interpretInstructionType1(fullInstr);
        instructionParsed = 1;
    }
    else if(firstCode == "100"){
        std::string fullInstr = bitset<7>(mem.at(indexInMem).value).to_string() + bitset<7>(mem.at(indexInMem+1).value).to_string();
        returnValue += 2;
        instr = interpretInstructionType2(fullInstr);
        instructionParsed = 1;
    }
    else if(firstCode == "101"){
        std::string fullInstr = bitset<7>(mem.at(indexInMem).value).to_string() + bitset<7>(mem.at(indexInMem+1).value).to_string() + bitset<7>(mem.at(indexInMem+2).value).to_string() + bitset<7>(mem.at(indexInMem+3).value).to_string() + bitset<7>(mem.at(indexInMem+4).value).to_string();
        returnValue += 5;
        instr = interpretInstructionType2(fullInstr);
        instructionParsed = 1;
    }
    else{
        std::string errorMsg = "reached an instruction that cannot be parsed, firstCode: " + (firstCode);
        cout << errorMsg << endl;
        printer << errorMsg << endl;
    }

    if(instructionParsed) printer << instr.toString() << endl;
    if(!instructionParsed) return returnValue;
    if(instr.name == "ADD") executeAdd(instr);
    else if(instr.name == "SUB") executeSub(instr);
    else if(instr.name == "MUL") executeMul(instr);
    else if(instr.name == "DIV") executeDiv(instr);
    else if(instr.name == "CMP") executeCmp(instr);
    else if(instr.name == "TST") executeTst(instr);
    else if(instr.name == "AND") executeAnd(instr);
    else if(instr.name == "ORR") executeOrr(instr);
    else if(instr.name == "XOR") executeXor(instr);
    else if(instr.name == "STR") executeStr(instr);
    else if(instr.name == "STB") executeStb(instr);
    else if(instr.name == "LOD") executeLod(instr);
    else if(instr.name == "JMP") executeJmp(instr);
    else if(instr.name == "JLT") executeJlt(instr);
    else if(instr.name == "JEQ") executeJeq(instr);
    else if(instr.name == "CAL") returnValue += executeCal(instr);
    else if(instr.name == "PSH") executePsh(instr);
    else if(instr.name == "POP") executePop(instr);
    else if(instr.name == "NOT") executeNot(instr);
    else if(instr.name == "OUT") executeOut(instr);
    else if(instr.name == "INP") executeInp(instr);
    else if(instr.name == "AMP") executeAmp(instr);
    else if(instr.name == "ALT") executeAlt(instr);
    else if(instr.name == "AEQ") executeAeq(instr);
    else if(instr.name == "AAL") returnValue += executeAal(instr);
    else if(instr.name == "RET") executeRet(instr);
    else if(instr.name == "NOP") executeNop(instr);
    else{

    }

    return returnValue;
}

// this will be the meaty function
// run the operation that you find, change MI to next operation
void OrcRunner::execute()  {
    ofstream ofs_full;
    ofs_full.open("out_full.exe.txt");
    ofs_full << "ererer" << endl;
    ofstream ofs_limited;
    ofstream printer;
    std::string desired_filename_full = "out_full.exe.txt";
    std::string desired_filename_lim = "out_limited_1.txt";
    desired_filename_full = "output1.txt";
    ofs_limited.open(desired_filename_lim);
    printer.open("instructionLog.txt");

    //set registers to an array full of zeros
    registers = new int[11];
    for(int i = 0; i < 11; i++) registers[i] = 0;


    // YO! You are allowed to do this! That way you don't have to do "MEMORY.at(40).value"
    // int a = MEMORY.at(40);

    cout << "about to read memory" << endl;
    ofs_full << "h" << endl;

    // currently just prints all memory addresses
    for (map_iter it = MEMORY.begin(); it != MEMORY.end(); it++ ){
        //cout << ((it->first > 99)?"":" ") << it->first << ": " << it->second << endl;
        ofs_full << ((it->first > 99)?"":" ") << it->first << ": " << it->second << endl;
        ofs_limited << bitset<7>(it->second.value).to_string() << endl;
        std::string res = bitset<7>(it->second.value).to_string();
        if(it->second.p.executable()) this->listOfInstructionStrings.push_back(res); 
    }
    currentIndex = entryPoint;
    cout << "currentIndex in the beginning: " << currentIndex << endl;
    cout << "first instruction: " << bitset<7>(MEMORY.at(currentIndex).value).to_string() << endl;
    //cout << "starting element: " << bitset<7>(MEMORY.at(currentIndex).value).to_string() << endl;

    //printListOfStrings();

    while(MEMORY.find(currentIndex) != MEMORY.end()){

        size_t nextLoc = interpretAndWrite(bitset<7>(MEMORY.at(currentIndex).value).to_string(), currentIndex, MEMORY, printer);
        cout << "just executed: " << currentIndex << endl;
        cout << "next instruction is at: " << nextLoc << endl;
        printRegisters();
        if(nextLoc == currentIndex){
            cout << "interpreter returned the same location, check log" << endl;
            break;
        }
        else currentIndex = nextLoc;
    }

    cout << "\nThis is our_stdout: " << our_stdout << endl << endl;
    

    ofs_full.close();
    ofs_limited.close();
    printer.close();

    delete [] registers;
}

