#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <iomanip>
using namespace std;

//opcode
#define iType 10011
#define rType 110011
#define LD 11 //0000011
#define SW 100011 //0100011
#define LUI 110111 // 0110111

//func3
//I-type
#define ADDI 0 //000    
#define SLTI 10 //010   
#define SLTIU 11 //011  
#define XORI 100 //     
#define ORI 110 //      
#define ANDI 111 //     
#define SLLI 1 //001    
#define SRLISRAI 101 // 
//R-type
#define ADDSUB 0 //000  
#define SLL 1 //001     
#define SLT 10 //010    
#define SLTU 11   //   
#define XOR 100     //  
#define SRLSRA 101      
#define OR 110  //      
#define AND 111

#define BEQ 0 //000
#define BNE 1 // 001
#define BLT 100
#define BGE 101
#define BLTU 110
#define BGEU 111

//keep track of reg values
class reg{
public:
    int value;
    bool isSet;
};

//global var
reg rd_write[32]; //32 registers


class imem{
public:
    string instruction;
    int opcode;
    int rd;
    int func3;
    int rs1;
    long immed;
    long Rimmed;
    int rs2;

    long Simmed;
    long Uimmed;

    long Bimmed;
    long UJimmed;

    void decode(const string& inst);
    void execute(reg rd_write[]);    //change to pass by reference
    void memory(map<long,long> &data_memory);
    void writeBack(reg rd_write[]);
};

class dmem{
    public:
    long address;
    long data;

    void Dmem_Init()
    {
        address = 0;
        data = 0;
    }
};

dmem data_memory[250];

void Imem_Init(imem& ob){
    ob.instruction = "";
    ob.opcode = 0;
    ob.rd = 0;
    ob.func3 = 0;
    ob.rs1 = 0;
    ob.immed = 0;
}

void Reg_Init(reg& rd_write){
    rd_write.value = 0;
    rd_write.isSet = false;
}

//converting signed binary to decimal
int binaryToDecimal(long n){
    long temp=n;
    long dec=0;
    long base=1;
    while(temp){
        long last=temp%10;
        temp=temp/10;
        dec+=last*base;
        base*=2;
    }
    dec=(dec+128)%256 -128;
    return dec;
}

long twoComp(string str){
    for(int i=0;i<str.length();i++){
        if(str[i]=='1'){
            str[i]='0';
        }
        else if(str[i]=='0'){
            str[i]='1';
        }
    }
    long temp=0;
    temp = stol(str,nullptr,2);
    temp*=-1;
    temp-=1;
    return temp;
}
    
void imem::decode(const string& inst) {
    string temp = inst.substr(0, 12);
    instruction = inst;

    /*
    cout << "... converting " << temp << " ..." << endl;
    immed = binaryToDecimal(stol(temp, NULL, 2));
    cout << "immed added: " << immed << endl;
    */
    int immediateValue = stoi(inst.substr(0, 12), nullptr, 2);
    immed = (immediateValue & 0x800) ? (immediateValue | 0xFFFFF000) : immediateValue;
    cout << "immed added: " << immed << endl;

    temp = inst.substr(12, 5);
    rs1 = binaryToDecimal(stoi(temp, nullptr, 10));
    cout << "rs1 added: " << rs1 << endl;  


    temp = inst.substr(17, 3);
    func3 = stoi(temp, nullptr, 10);
    cout << "func3 added: " << temp << endl;


    temp = inst.substr(20, 5);
    rd = binaryToDecimal(stoi(temp, nullptr, 10));
    cout << "rd added: " << rd << endl;


    temp = inst.substr(25, 7);
    opcode = stoi(temp, nullptr, 10);
    cout << "opcode added: " << temp << endl;


    if (opcode == rType) {
        temp = inst.substr(0, 7);
        Rimmed = binaryToDecimal(stol(temp, NULL, 10));
        cout << "Rimmed added: " << temp << endl;


        temp = inst.substr(7, 5);
        rs2 = binaryToDecimal(stoi(temp, nullptr, 10));
        cout << "rs2 added: " << rs2 << endl;
    }
    if (opcode == 0110111)  //LUI
    {
        temp = instruction.substr(0,20);
        if (temp[0] == '1'){
            Uimmed = twoComp(temp);
        }
        else {
            Uimmed = stol(temp,nullptr,2);
        }
    }
    if (opcode == 100011) //SW
    {
        temp = inst.substr(0, 7);
        immed = stoi(temp, nullptr, 2);
        immed = immed << 5;

        temp = inst.substr(7, 5);
        rs2 = stoi(temp, nullptr, 2);

        temp = inst.substr(12, 5);
        rs1 = stoi(temp, nullptr, 2);

        temp = inst.substr(20, 5);
        rd = stoi(temp, nullptr, 2);
        immed += rd;
        

        // Display rs1 and rs2
        cout << "rs1 added (SW instruction): " << rs1 << endl;
        cout << "rs2 added (SW instruction): " << rs2 << endl; 
    }

}

void imem::execute(reg rd_write[]) 
{  // change to pass by reference
    switch(opcode){
        case iType:{
            cout << "itype func3: " << func3 << endl;
            switch(func3){
                case ADDI:{
                    cout << "ADDI ";
                    //check if rs1 reg already has value, if so use it
                    if(!rd_write[rs1].isSet){ 
                        rd_write[rd].value = rs1 + immed;
                        rd_write[rd].isSet=true;
                        cout << "result: " << rd_write[rd].value <<endl;
                    }
                    else {
                        rd_write[rd].value = rd_write[rs1].value + immed;
                        rd_write[rd].isSet=true;
                        cout << "result: " << rd_write[rd].value <<endl;
                    }
                    break;
                }
                case SLLI: {
                    int temp_rs1 = rd_write[rs1].isSet ? rd_write[rs1].value : rs1;

                    // Immediate value contains the shift amount (lower 5 bits)
                    int shift_amount = immed & 0b00011111;

                    cout << "SLLI ";
                    rd_write[rd].value = temp_rs1 << shift_amount;
                    rd_write[rd].isSet = true;

                    cout << "result: " << rd_write[rd].value << endl;
                    break;
                }
                case SRLISRAI: {
                    int temp_rs1 = rd_write[rs1].isSet ? rd_write[rs1].value : rs1;

                    immed = binaryToDecimal(stol(instruction.substr(0, 7)));

                    rs2 = binaryToDecimal(stol(instruction.substr(7, 5)));
                    cout << "immed added: " << immed << endl;
                    cout << "shamt added: " << rs2 << endl;
                    
                    int shift_amount = rs2;

                    if ((immed) == 32) {
                        cout << "SRAI ";
                        rd_write[rd].value = temp_rs1 >> shift_amount;
                    } else {
                        cout << "SRLI ";
                        rd_write[rd].value = (unsigned int)temp_rs1 >> shift_amount;
                    }

                    rd_write[rd].isSet = true;
                    
                    cout << "result: " << rd_write[rd].value << endl;
                    break;
                }
                case SLTI: {
                    cout << "SLTI" << endl;
                    int temp_rs1 = rd_write[rs1].isSet ? rd_write[rs1].value : rs1;

                    if (temp_rs1 < immed) {
                        rd_write[rd].value = 1;
                    } else {
                        rd_write[rd].value = 0;
                    }

                    rd_write[rd].isSet = true;
                    cout << "result: " << rd_write[rd].value << endl;
                    break;
                }
                case SLTIU: {
                    cout << "SLTIU" << endl;
                    int temp_rs1 = rd_write[rs1].isSet ? rd_write[rs1].value : rs1;
                    int temp_immed = immed;

                    // Comparing rs1 with immed as an unsigned integer
                    if ((unsigned int)temp_rs1 < (unsigned int)temp_immed) {
                        rd_write[rd].value = 1;
                    } else {
                        rd_write[rd].value = 0;
                    }
                    rd_write[rd].isSet = true;
                    cout << "result: " << rd_write[rd].value << endl;
                    break;
                }
                case XORI: {
                    cout << "XORI ";
                    if (!rd_write[rs1].isSet) { 
                        rd_write[rd].value = rs1 ^ immed;
                        rd_write[rd].isSet = true;
                        cout << "result: " << rd_write[rd].value << endl;
                    } else {
                        rd_write[rd].value = rd_write[rs1].value ^ immed;
                        rd_write[rd].isSet = true;
                        cout << "result: " << rd_write[rd].value << endl;
                    }
                    break;
                }
                case ORI: {
                    cout << "ORI ";
                    // check if rs1 reg already has a value, if so, use it
                    if (!rd_write[rs1].isSet) {
                        rd_write[rd].value = rs1 | immed; // perform OR operation with immediate value
                        rd_write[rd].isSet = true;
                        cout << "result: " << rd_write[rd].value << endl;
                    } else {
                        rd_write[rd].value = rd_write[rs1].value | immed; // perform OR operation
                        rd_write[rd].isSet = true;
                        cout << "result: " << rd_write[rd].value << endl;
                    }
                    break;
                }
                case ANDI: {
                    cout << "ANDI ";
                    int temp_rs1 = rd_write[rs1].isSet ? rd_write[rs1].value : rs1;
                    int result = temp_rs1 & immed; // Perform bitwise AND operation
                    rd_write[rd].value = result; // Store the result in the destination register
                    rd_write[rd].isSet = true;
                    cout << "result: " << rd_write[rd].value << endl;
                    break;
                }
                default:{
                    cout<<"not valid I-TYPE instruction"<<endl;
                    break;
                }
            }
            break;
        } 
        case rType:{
            cout << "rtype func3: "<< func3 << endl;
            switch(func3) {
                case ADDSUB:{ 
                    int temp_rs1 = rd_write[rs1].isSet ? rd_write[rs1].value : rs1;
                    int temp_rs2 = rd_write[rs2].isSet ? rd_write[rs2].value : rs2;
                    if (Rimmed == 0b0100000)
                    {
                        cout << "SUB" << endl;
                        rd_write[rd].value = temp_rs1 - temp_rs2;
                        rd_write[rd].isSet = true;
                    }
                    else
                    {
                        cout << "ADD" << endl;
                        rd_write[rd].value = temp_rs1 + temp_rs2;
                        rd_write[rd].isSet = true;
                    }
                    cout << "result: " << rd_write[rd].value << endl;
                    break;
                }

                case SLL: {
                    int temp_rs1 = rd_write[rs1].isSet ? rd_write[rs1].value : rs1;
                    int temp_rs2 = rd_write[rs2].isSet ? rd_write[rs2].value : rs2;
                    
                    // Bitmask to extract last 5 bits of the shift amount
                    int bitmask = 0b00011111;
                    temp_rs2 = temp_rs2 & bitmask;

                    cout << "SLL ";
                    rd_write[rd].value = temp_rs1 << temp_rs2;
                    rd_write[rd].isSet = true;
                    cout << "result: " << rd_write[rd].value << endl;
                    break;
                }
                case SLT: {
                    cout << "SLT" << endl;
                    int temp_rs1 = rd_write[rs1].isSet ? rd_write[rs1].value : rs1;
                    int temp_rs2 = rd_write[rs2].isSet ? rd_write[rs2].value : rs2;

                    if (temp_rs1 < temp_rs2)
                    {
                        rd_write[rd].value = 1;
                        rd_write[rd].isSet = true;
                    }
                    else
                    {
                        rd_write[rd].value = 0;
                        rd_write[rd].isSet = true;
                    }
                    cout << "result: " << rd_write[rd].value << endl;
                    break;
                }
                case SLTU: {
                    cout << "SLTU" << endl;
                    int temp_rs1 = rd_write[rs1].isSet ? rd_write[rs1].value : rs1;
                    int temp_rs2 = rd_write[rs2].isSet ? rd_write[rs2].value : rs2;
                    if (temp_rs1 < temp_rs2)
                    {
                        rd_write[rd].value = 1;
                        rd_write[rd].isSet = true;
                    }
                    else
                    {
                        rd_write[rd].value = 0;
                        rd_write[rd].isSet = true;
                    }
                    cout << "result: " << rd_write[rd].value << endl;
                    break;
                }
                case XOR: {
                    cout << "XOR" << endl;
                    int temp_rs1 = rd_write[rs1].isSet ? rd_write[rs1].value : rs1;
                    int temp_rs2 = rd_write[rs2].isSet ? rd_write[rs2].value : rs2;
                    rd_write[rd].value = temp_rs1 ^ temp_rs2;
                    rd_write[rd].isSet = true;
                    cout << "result: " << rd_write[rd].value << endl;
                    break;
                }
                case SRLSRA: {
                    int temp_rs1 = rd_write[rs1].isSet ? rd_write[rs1].value : rs1;
                    int temp_rs2 = rd_write[rs2].isSet ? rd_write[rs2].value : rs2;
                    int bitmask = 0b00011111;
                    temp_rs2 = temp_rs2 & bitmask;
                    if (Rimmed == 0b0100000)
                    { 
                        cout << "SRA" << endl;
                        rd_write[rd].value = (unsigned int)temp_rs1 >> temp_rs2;
                        rd_write[rd].isSet = true;
                    }
                    else
                    {
                        cout << "SRL" << endl;
                        rd_write[rd].value = (unsigned int)temp_rs1 >> temp_rs2;
                        rd_write[rd].isSet = true;
                    }
                    cout << "result: " << rd_write[rd].value << endl;
                    break;
                }
                case OR: {
                    cout << "OR" << endl;
                    int temp_rs1 = rd_write[rs1].isSet ? rd_write[rs1].value : rs1;
                    int temp_rs2 = rd_write[rs2].isSet ? rd_write[rs2].value : rs2;
                    rd_write[rd].value = temp_rs1 | temp_rs2;
                    rd_write[rd].isSet = true;
                    cout << "result: " << rd_write[rd].value << endl;
                    break;
                }
                case AND: {
                    cout << "AND" << endl;
                    int temp_rs1 = rd_write[rs1].isSet ? rd_write[rs1].value : rs1;
                    int temp_rs2 = rd_write[rs2].isSet ? rd_write[rs2].value : rs2;
                    rd_write[rd].value = temp_rs1 & temp_rs2;
                    rd_write[rd].isSet = true;
                    cout << "result: " << rd_write[rd].value << endl;
                    break;
                }
                }
                break;
            }
            case 110111: { //lui
                //Take immediate, find the address of reg
                //Then store into register
                rd_write[rd].value=Uimmed;
                cout<<"LUI Result: "<<rd_write[rd].value<<endl;


                break;
            
            }
            case 11://LW
            {
                cout<<"Immed: "<<immed<<" Rs1:"<<rs1<<" RD:"<<rd<<endl; 
                //rs1=0x80000000
                rd=immed+rs1;
                rd_write[rd].value=data_memory[rd/4].data;
                cout<<"LW Result: "<<rd_write[rd].value<<endl;
                break;                                
            }
            case 100011://SW
            {
                cout << "SW REACHED" << endl;
                cout << "immed: " << immed << endl;
                cout << "rs2: " << rs2 << endl;
                cout << "rs1: " << rs1 << endl;
                cout << "imm5: " << rd << endl;

                break;
            }

            default:{
            cout<<"not valid instruction"<<endl;
            break;
        }
    }
}
      



int main(){

    int pc = 0;


    imem ob[100]; //consider 100 instructions
    for (int i=0;i<100;i++){
        Imem_Init(ob[i]);
    }

    
    for (int i=0;i<33;i++){
        Reg_Init(rd_write[i]);
    }

    for (long i=0;i<250;i++){
        data_memory[i].Dmem_Init();
    }

    long startingAddr = 0x80000000;
    for(int i=0;i<128;i++){
        data_memory[i].address = startingAddr;
        data_memory[i].data = 0;
        startingAddr+=4;
    }
    
    string filename = "tests/r_type.dat"; // Change file for testing
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cout << "Unable to open file" << endl;
        return 1;
    }

    string choice;
    string line;
    bool continueLoop = true;
    int total = 0;
    int memory_index = 0;

    while (continueLoop) {
        cout << "===========================================" << endl;
        cout << "Select an action:" << endl;
        cout << "r. RUN" << endl;
        cout << "s. STEP" << endl;
        cout << "x(reg). RETURN REG" << endl;
        cout << "0x(addr). RETURN ADR" << endl;
        cout << "pc. RETURN PC" << endl;
        cout << "q. QUIT" << endl;
        cout << "===========================================" << endl;
        cout << "Enter your choice: ";

        cin >> choice;
        char prefix;
        unsigned int location;
        cout << "______________________________________" << endl << "OUTPUT:" << endl << "______________________________________" << endl;

        if (choice == "r") {
            while (!inputFile.eof()) {
                if (getline(inputFile, line)) {
                    cout << "Current Instruction: " << line << endl;
                    ob[total].decode(line);
                    if (ob[total].opcode == 0) {
                        break;
                    }
                    ob[total].execute(rd_write);
                    total++;
                    pc += 4; // Increment PC after instruction execution
                    cout << "Program Counter (PC): " << pc << endl; // Display PC value
                    cout << endl;
                }
            }
        } else if (choice == "s") {
            if (getline(inputFile, line)) {
                cout << "Current Instruction: " << line << endl;   
                ob[total].decode(line);
                ob[total].execute(rd_write);
                total++;
                pc=pc+4;
            } else {
                cout << "Maximum instruction limit reached." << endl;
            }
        } else if (choice[0] == 'x') {
            location = stoi(choice.substr(1), nullptr, 10);
            cout<<"Location: "<< dec << location<<endl;
            cout << "Register :" << choice << ": " << hex << rd_write[location].value << endl;
        } else if (choice[0] == '0') {
            /*
            location = stoi(choice.substr(2), nullptr, 16);
            cout << "Memory Address " << choice << ": " << data_memory[location] << endl;
            */
        } else if (choice == "pc") {
            cout << "Program Counter (PC): " << hex << pc << endl;
        } else if (choice == "q") {
            cout << "Thank you! Run again!" << endl;
            continueLoop = false;
        }
    }

    //printing reg number and value --like reg.txt
    for(int i = 0; i < 32; i++){
        cout << "reg number: " << dec << i << ", reg value: " << hex << rd_write[i].value << endl;
    }
    
    return 0;
    }