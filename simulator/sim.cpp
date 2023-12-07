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
#define STLI 10 //010   
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

void imem::execute(reg rd_write[]) {  // change to pass by reference
    switch (opcode) {
        case iType:
            switch (func3) {
                case ADDI:
                    cout << "ADDI ";
                    rd_write[rd].value = rd_write[rs1].value + immed;
                    rd_write[rd].isSet = true;
                    cout << "result: " << rd_write[rd].value << endl;
                    break;
                case SLLI:
                    cout << "SLLI ";
                    rd_write[rd].value = rd_write[rs1].value << (immed & 0b00011111);
                    rd_write[rd].isSet = true;
                    cout << "result: " << rd_write[rd].value << endl;
                    break;
                case SRLISRAI:
                    cout << ((Rimmed == 0b0100000) ? "SRAI " : "SRLI ");
                    rd_write[rd].value = ((Rimmed == 0b0100000) ? (rd_write[rs1].value >> (immed & 0b00011111))
                                                                   : ((unsigned int)rd_write[rs1].value >> (immed & 0b00011111)));
                    rd_write[rd].isSet = true;
                    cout << "result: " << rd_write[rd].value << endl;
                    break;
                default:
                    cout << "Invalid I-Type instruction" << endl;
                    break;
            }
            break;

        case rType:
            switch (func3) {
                case SLL:
                    cout << "SLL ";
                    rd_write[rd].value = rd_write[rs1].value << (rd_write[rs2].value & 0b00011111);
                    rd_write[rd].isSet = true;
                    cout << "result: " << rd_write[rd].value << endl;
                    break;
                default:
                    cout << "Invalid R-Type instruction" << endl;
                    break;
            }
            break;

        default:
            cout << "Invalid instruction" << endl;
            break;
    }
}

int main(){

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
            cout << "Memory Address " << choice << ": (value goes here)" << endl;
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