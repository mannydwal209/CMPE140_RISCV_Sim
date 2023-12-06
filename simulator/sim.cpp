#include <iostream>
#include <fstream>
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

class imem
{
public:
    string instruction;
    string opcode;
    string rd;
    string func3;
    string rs1;
    string immed;
    string Rimmed;
    string rs2;

    // member functions
    //left Imem_Init() outside of class
    /*
    void Imem_Init()
    {
        instruction = "";
        opcode = "";
        rd = "";
        func3 = "";
        rs1 = "";
        immed = "";
    }
    */

    void fetch(ifstream &inputFile, int &total)
    {
        string line;
        string subInstruction = "";
        int lineCount = 0;

        while (getline(inputFile, line))
        { // read line by line of file until EOF

            // FETCH STAGE

            // instructions read in as little endian
            subInstruction = line + subInstruction;
            lineCount++;

            if (lineCount == 4)
            { // go through five stages every 4 lines
                instruction = subInstruction;
                string temp = subInstruction;
                lineCount = 0;
                subInstruction = "";

                total++;
            }
        }
    }

    void decode(int &immed, int &rs1, int &func3, int &rd, int &opcode, int &Rimmed, int &rs2)
    {
        /*
        // sort into I-type format
        immed = binaryToDecimal(stol(instruction.substr(0, 12), nullptr, 10));
        rs1 = binaryToDecimal(stoi(instruction.substr(12, 5), nullptr, 10));
        func3 = stoi(instruction.substr(17, 3), nullptr, 10);
        rd = binaryToDecimal(stoi(instruction.substr(20, 5), nullptr, 10));
        opcode = stoi(instruction.substr(25, 7), nullptr, 10);

        // R-type format
        Rimmed = binaryToDecimal(stol(instruction.substr(0, 7), nullptr, 10));
        rs2 = binaryToDecimal(stoi(instruction.substr(7, 5), nullptr, 10));
        */
        
        string temp;

        temp=instruction.subtr(0,12);
        immed=binaryToDecimal(stol(temp,nullptr,10));

        temp = instruction.substr(12,5);
        rs1=binaryToDecimal(stoi(temp,nullptr,10));

        temp = instruction.substr(17,3);
        func3 = stoi(temp,nullptr,10);

        temp = instruction.substr(20,5);
        rd=binaryToDecimal(stoi(temp,nullptr,10));

        temp = instruction.substr(25,7);
        opcode = stoi(temp,nullptr,10);

        if(opcode == rType)
        {
            temp = instruction.substr(0,7);
            Rimmed=binaryToDecimal(stol(temp,nullptr,10));
        
            temp = instruction.substr(7,5);
            rs2=binaryToDecimal(stoi(temp,nullptr,10));
        }
    }

    void execute(int opcode, int func3, int rs1, int rs2, int rd, int immed, int Rimmed, vector<reg> &rd_write)
    {
        switch (opcode)
        {
        case iType:
        {
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
};

//keep track of reg values
class reg{
public:
    int value;
    bool isSet;
};

//function declarations
void Imem_Init(imem& ob);
void Reg_Init(reg& rd_write);
int binaryToDecimal(long n);

int main(){

    imem ob[100]; //consider 100 instructions
    for (int i=0;i<100;i++){
        Imem_Init(ob[i]);
    }

    reg rd_write[32]; //32 registers
    for (int i=0;i<33;i++){
        Reg_Init(rd_write[i]);
    }
    
    string filename = "r_type_jayson.dat"; //change file for testing

    //opening file
    ifstream inputFile;
    inputFile.open(filename);
    if(!inputFile.is_open()){
        cout<<"unable to open file"<<endl;
        exit(1);
    }

    char choice;
    string line;
    bool continueLoop = true;   //UI Variables
    int total = 0;

     while(getline(inputFile,line))
     {
        cout << "Select an action:" << endl;    //UI Menu (User Options)
        cout << "r. RUN" << endl;
        cout << "s. STEP" << endl;
        cout << "x(reg). RETURN REG" << endl;
        cout << "x(addr). RETURN ADR" << endl;
        cout << "pc. RETURN PC" << endl;
        cout << "q. QUIT" << endl;
        cout << "Enter your choice: ";

        cin >> choice;  //read choice from keyboard
        

        switch(choice) {
            case 'r':
                break;
            case 's':
                 //instruction number
                    ob[total].instruction = line; //assuming the data file has 32-bit instructions on each line
                    ob[total].decode();
                    
                    //====================================================//
                    //EXECUTE
                    
                    switch(ob[total].opcode){
                        case iType:{
                            switch(ob[total].func3){
                                case ADDI:{
                                    cout << "ADDI ";
                                    //check if rs1 reg already has value, if so use it
                                    if(!rd_write[ob[total].rs1].isSet){ 
                                        rd_write[ob[total].rd].value = ob[total].rs1 + ob[total].immed;
                                        rd_write[ob[total].rd].isSet=true;
                                        cout << "result: " << rd_write[ob[total].rd].value <<endl;
                                    }
                                    else {
                                        rd_write[ob[total].rd].value = rd_write[ob[total].rs1].value + ob[total].immed;
                                        rd_write[ob[total].rd].isSet=true;
                                        cout << "result: " << rd_write[ob[total].rd].value <<endl;
                                    }
                                    break;
                                }
                                case SLL: {
                                    int temp_rs1 = rd_write[ob[total].rs1].isSet ? rd_write[ob[total].rs1].value : ob[total].rs1;
                                    int temp_rs2 = rd_write[ob[total].rs2].isSet ? rd_write[ob[total].rs2].value : ob[total].rs2;
                                    
                                    // Bitmask to extract last 5 bits of the shift amount
                                    int bitmask = 0b00011111;
                                    temp_rs2 = temp_rs2 & bitmask;

                                    cout << "SLL ";
                                    rd_write[ob[total].rd].value = temp_rs1 << temp_rs2;
                                    rd_write[ob[total].rd].isSet = true;
                                    cout << "result: " << rd_write[ob[total].rd].value << endl;
                                    break;
                                }
                                case SRLISRAI: {
                                    int temp_rs1 = rd_write[ob[total].rs1].isSet ? rd_write[ob[total].rs1].value : ob[total].rs1;
                                    
                                    // Immediate value contains the shift amount (lower 5 bits)
                                    int shift_amount = ob[total].immed & 0b00011111;

                                    if (ob[total].Rimmed = 0b0100000) {
                                        cout << "SRAI ";
                                        rd_write[ob[total].rd].value = temp_rs1 >> shift_amount;
                                    } else {
                                        cout << "SRLI ";
                                        rd_write[ob[total].rd].value = (unsigned int)temp_rs1 >> shift_amount;
                                    }

                                    rd_write[ob[total].rd].isSet = true;
                                    
                                    cout << "result: " << rd_write[ob[total].rd].value << endl;
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
                            switch(ob[total].func3) {
                                case SLL: {
                                    int temp_rs1 = rd_write[ob[total].rs1].isSet ? rd_write[ob[total].rs1].value : ob[total].rs1;
                                    int temp_rs2 = rd_write[ob[total].rs2].isSet ? rd_write[ob[total].rs2].value : ob[total].rs2;
                                    
                                    // Bitmask to extract last 5 bits of the shift amount
                                    int bitmask = 0b00011111;
                                    temp_rs2 = temp_rs2 & bitmask;

                                    cout << "SLL ";
                                    rd_write[ob[total].rd].value = temp_rs1 << temp_rs2;
                                    rd_write[ob[total].rd].isSet = true;
                                    cout << "result: " << rd_write[ob[total].rd].value << endl;
                                    break;
                                }
                            }
                            break;
                        }
                        default:{
                            cout<<"not valid instruction"<<endl;
                            break;
                        }
                    }

                    total++;
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