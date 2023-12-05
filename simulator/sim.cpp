#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <iomanip>

using namespace std;

#define iType 10011
#define rType 110011 

//FUNC3 Bits
//I-type
#define ADDI 0                   
#define STLI 10                    
#define SLTIU 11                   
#define XORI 100                      
#define ORI 110                         
#define ANDI 111                       
#define SLLI 1              
#define SRLISRAI 101              
//R-type
#define ADDSUB 0              
#define SLL 1                
#define SLT 10               
#define SLTU 011                    
#define XOR 100             
#define SRLSRA 101          
#define OR 110                     
#define AND 111                   

class imem{
public:
    string instruction;
    string opcode;
    string rd;
    string func3;
    string rs1;
    string immed;
    string Rimmed;
    string rs2;
};


class reg{
public:
    int value;
    bool isSet;
};

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


void action1() {
    cout << "Performing action 1" << endl;
}

void action2() {
    cout << "Performing action 2" << endl;
}

void action3() {
    cout << "Performing action 3" << endl;
}

void action4() {
    cout << "Performing action 4" << endl;
}

int main() {
    char choice;
    bool continueLoop = true;

    while (continueLoop) {
        cout << "Select an action:" << endl;
        cout << "r. RUN" << endl;
        cout << "s. STEP" << endl;
        cout << "x(reg). RETURN REG" << endl;
        
        cout << "x(addr). RETURN ADR" << endl;
        cout << "pc. RETURN PC" << endl;
        cout << "q. QUIT" << endl;
        cout << "Enter your choice: ";

        cin >> choice;

        //fetch
        //decode
        //execute

        switch(choice) {
            case 'r':
                action1();
                break;
            case 's':
                action2();//do all 3 stages  but stop after dat reader reads first 32 bits (1 instruction)
                break;
            case 'x0':
                action3();//read user input and use value x0-x31 to determine the location from rd_write array
                break;//ex if 0x31 then rd_write[31]
                case 'x0':
                action3();//read user input and use value x0-x31 to determine the location from rd_write array
                break;//ex if 0x31 then rd_write[31]
            case 'pc':
                action4();
                break;
            case '1':
                continueLoop = false; // Set flag to exit loop
                break;
            default:
                cout << "Invalid choice!" << endl;
                break;
        }
    }

    return 0;
}

