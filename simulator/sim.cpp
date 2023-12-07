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
#define SLTU 011   //   
#define XOR 100     //  
#define SRLSRA 101      
#define OR 110  //      
#define AND 111  //    

// Load/Store
#define LDW 010 //010
#define SW 010

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

    void decode(const string& inst);
    void execute(reg rd_write[]);    //change to pass by reference
    void memory(map<long,long> &data_memory);
    void writeBack(reg rd_write[]);
};

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
    cout << "func3 added: " << func3 << endl;


    temp = inst.substr(20, 5);
    rd = binaryToDecimal(stoi(temp, nullptr, 10));
    cout << "rd added: " << rd << endl;


    temp = inst.substr(25, 7);
    opcode = stoi(temp, nullptr, 10);
    cout << "opcode added: " << opcode << endl;


    if (opcode == rType) {
        temp = inst.substr(0, 7);
        Rimmed = binaryToDecimal(stol(temp, NULL, 10));
        cout << "Rimmed added: " << Rimmed << endl;


        temp = inst.substr(7, 5);
        rs2 = binaryToDecimal(stoi(temp, nullptr, 10));
        cout << "rs2 added: " << rs2 << endl;
    }
}

void imem::execute(reg rd_write[]) 
{  // change to pass by reference
    switch (opcode)
    {
        case iType:
            switch (func3)
            {
                case ADDI:
                {
                    cout << "ADDI ";
                    // check if rs1 reg already has value, if so use it
                    if (!rd_write[rs1].isSet)
                    {
                        rd_write[rd].value = rs1 + immed;
                        rd_write[rd].isSet = true;
                        cout << "result: " << rd_write[rd].value << endl;
                    }
                    else
                    {
                        rd_write[rd].value = rd_write[rs1].value + immed;
                        rd_write[rd].isSet = true;
                        cout << "result: " << rd_write[rd].value << endl;
                    }
                    break;
                }

                case SLTI:
                {
                    cout << "SLTI" << endl;
                    if (rd_write[rs1].value < immed)
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
                case SLTIU:
                {
                    cout << "SLTIU" << endl;
                    if ((unsigned int)rd_write[rs1].value < immed)
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
                case XORI:
                {
                    cout << "XORI" << endl;
                    if (!rd_write[rs1].isSet)
                    {
                        rd_write[rd].value = rd_write[rs1].value ^ immed;
                        rd_write[rd].isSet = true;
                    }
                    else
                    {
                        rd_write[rd].value = rd_write[rs1].value ^ immed;
                        rd_write[rd].isSet = true;
                    }
                    cout << "result: " << rd_write[rd].value << endl;
                    break;
                }
                case ORI:
                {
                    cout << "ORI" << endl;
                    if (!rd_write[rs1].isSet)
                    {
                        rd_write[rd].value = rd_write[rs1].value | immed;
                        rd_write[rd].isSet = true;
                    }
                    else
                    {
                        rd_write[rd].value = rd_write[rs1].value | immed;
                        rd_write[rd].isSet = true;
                    }
                    cout << "result: " << rd_write[rd].value << endl;
                    break;
                }
                case ANDI:
                {
                    cout << "ANDI" << endl;
                    if (!rd_write[rs1].isSet)
                    {
                        rd_write[rd].value = rd_write[rs1].value & immed;
                        rd_write[rd].isSet = true;
                    }
                    else
                    {
                        rd_write[rd].value = rd_write[rs1].value & immed;
                        rd_write[rd].isSet = true;
                    }
                    cout << "result: " << rd_write[rd].value << endl;
                    break;
                }

                case SLL:
                {
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
                case SRLISRAI:
                {
                    int temp_rs1 = rd_write[rs1].isSet ? rd_write[rs1].value : rs1;

                    // Immediate value contains the shift amount (lower 5 bits)
                    int shift_amount = immed & 0b00011111;

                    if (Rimmed = 0b0100000)
                    {
                        cout << "SRAI ";
                        rd_write[rd].value = temp_rs1 >> shift_amount;
                    }
                    else
                    {
                        cout << "SRLI ";
                        rd_write[rd].value = (unsigned int)temp_rs1 >> shift_amount;
                    }

                    rd_write[rd].isSet = true;

                    cout << "result: " << rd_write[rd].value << endl;
                    break;
                }
                default:
                {
                    cout << "not valid I-TYPE instruction" << endl;
                    break;
                }
            }
                break;
    
            case rType:
            {
                switch (func3)
                {
                case SLL:
                {
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
                }
                break;
            }
            default:
            {
                cout << "not valid instruction" << endl;
                break;
            }
    }
}
      

void imem::memory(map<long,long> &data_memory)
{
    long address,store_address;
    switch (opcode) 
    {
        case 000011:
            cout << "LW ";
            address = rd_write[rs1].value + immed;
            if (data_memory.find(address) != data_memory.end()) 
            {
                rd_write[rd].value = data_memory[address];
                rd_write[rd].isSet = true;
                cout << "result: " << rd_write[rd].value << endl;
            } else 
            {
                cout << "Error: Memory address not found." << endl;
            }
            break;
        case 0100011:
            cout << "SW ";
            store_address = rd_write[rs1].value + immed;
            data_memory[store_address] = rd_write[rs2].value;
            cout << "Data stored at address " << store_address << ": " << rd_write[rs2].value << endl;
            break;



    }    
}

/*
void imem::writeBack(reg rd_write[], reg output[])
{
    
}
*/

int main(){

    map<long, long> data_memory; // Added for memory storage


    imem ob[100]; //consider 100 instructions
    for (int i=0;i<100;i++){
        Imem_Init(ob[i]);
    }

    
    for (int i=0;i<33;i++){
        Reg_Init(rd_write[i]);
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

    while (continueLoop) {
        cout << "===========================================" << endl;
        cout << "Select an action:" << endl;
        cout << "r. RUN" << endl;
        cout << "s. STEP" << endl;
        cout << "x(reg). RETURN REG" << endl;
        cout << "0x(addr). RETURN ADR" << endl;
        cout << "pc. RETURN PC" << endl;
        cout << "q. QUIT" << endl;
        cout << "Current Instruction: " << line << endl;
        cout << "===========================================" << endl;
        cout << "Enter your choice: ";

        cin >> choice;
        char prefix;
        unsigned int location;
        cout << "______________________________________" << endl << "OUTPUT:" << endl << "______________________________________" << endl;

        if (choice == "r") {
            while (!inputFile.eof()) {
                if (getline(inputFile, line)) {
                    ob[total].decode(line);
                    cout << "Current Instruction: " << line << endl;
                    if (ob[total].opcode == 0) {
                        break;
                    }
                    ob[total].execute(rd_write);
                    total++;
                }
            }
        } else if (choice == "s") {
            if (getline(inputFile, line)) {
                ob[total].decode(line);
                ob[total].execute(rd_write);
                total++;
            } else {
                cout << "Maximum instruction limit reached." << endl;
            }
        } else if (choice[0] == 'x') {
            location = stoi(choice.substr(1), nullptr, 16);
            cout << "Register :" << choice << ": " << rd_write[location].value << endl;
        } else if (choice[0] == '0') {
            /*
            location = stoi(choice.substr(2), nullptr, 16);
            cout << "Memory Address " << choice << ": " << data_memory[location] << endl;
            */
        } else if (choice == "pc") {
            cout << choice << ": (value goes here)" << endl;
        } else if (choice == "q") {
            cout << "Thank you! Run again!" << endl;
            continueLoop = false;
        }
    }

    //printing reg number and value --like reg.txt
    for(int i = 0; i < 32; i++){
        cout << "reg number: " << i << ", reg value: " << rd_write[i].value << endl;
    }
    
    return 0;
    }