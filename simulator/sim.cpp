#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
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

//keep track of reg values
class reg{
public:
    int value;
    bool isSet;
};

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
    void memory(map<long,long> %data_memory);
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
    cout << "... converting " << temp << " ..." << endl;
    immed = binaryToDecimal(stol(temp, NULL, 2));
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
    switch (opcode) {
        case iType:
            switch (func3) {
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
            }
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
    
}

void imem::writeBack(reg rd_write[])
{

}

int main(){

    map<long, long> data_memory; // Added for memory storage


    imem ob[100]; //consider 100 instructions
    for (int i=0;i<100;i++){
        Imem_Init(ob[i]);
    }

    reg rd_write[32]; //32 registers
    for (int i=0;i<33;i++){
        Reg_Init(rd_write[i]);
    }
    
    string filename = "tests/r_type.dat"; //change file for testing

    //opening file
    ifstream inputFile;
    inputFile.open(filename);
    if(!inputFile.is_open()){
        cout<<"unable to open file"<<endl;
        exit(1);
    }

    string choice;
    string line;
    bool continueLoop = true;   //UI Variables
    int total = 0;

     while(getline(inputFile, line) && choice != "q"){
        cout << "===========================================" << endl;
        cout << "Select an action:" << endl;    //UI Menu (User Options)
        cout << "r. RUN" << endl;
        cout << "s. STEP" << endl;
        cout << "x(reg). RETURN REG" << endl;
        cout << "0x(addr). RETURN ADR" << endl;
        cout << "pc. RETURN PC" << endl;
        cout << "q. QUIT" << endl;
        cout << "Current Instruction: " << line << endl;
        cout << "===========================================" << endl;
        cout << "Enter your choice: ";

        cin >> choice;  //read choice from keyboard
        char prefix;    //extracting register or memory address
        unsigned int location;
        cout << "______________________________________" << endl << "OUTPUT:" << endl << "______________________________________" << endl;
        if(choice == "r")
        {
            while (total < 100) {
                ob[total].decode(line);
                if (ob[total].opcode == 0) {
                    break;
                }
                ob[total].execute(rd_write);
                total++;
            }
        } else if(choice == "s")
        {
                if (total < 100) {
                    ob[total].decode(line);
                    ob[total].execute(rd_write);
                    total++;
                } else {
                    cout << "Maximum instruction limit reached." << endl;
                }
        } else if(choice[0] == 'x')
        {
            cout << "Register " << choice << ": (value goes here)" << endl;
        }else if(choice[0] == '0'){
            location = stoi(choice.subtr(2),nullptr,16); //Extracts memory address(ignore 0x read addy)
            cout << "Memory Address " << choice << ": " << data_memory[location] << endl;
        } else if(choice == "pc")
        {
            cout << choice << ": (value goes here)" << endl;
        } else if(choice == "q")
        {
            cout << "Thank you! Run again!" << endl;
        }

    }

    // printing instructions --like pc.txt 
    for(int i=0;i<total;i++){
        cout <<"Instructions in binary" << endl;
        cout << ob[i].instruction << endl << endl; //expected 32 bits
    }

    //printing reg number and value --like reg.txt
    for(int i = 0; i < 32; i++){
        cout << "reg number: " << i << ", reg value: " << rd_write[i].value << endl;
    }
    
    return 0;
    }