#include <fstream>
#include <iostream>
#include <string>
#define Memsize 1<<14
#define FF -(~(1<<31))
using namespace std;

typedef union{

    struct{
        unsigned int opcode : 7;
        unsigned int rest : 25;
    };

    struct{
        unsigned int opcode : 7; //0x33
        unsigned int rd : 5;
        unsigned int funct3 : 3;
        unsigned int rs1 : 5 ;
        unsigned int rs2 : 5;
        unsigned int funct7 : 7;
    }Rformat;
    
    struct{
        unsigned int opcode : 7; //0x13
        unsigned int rd : 5;
        unsigned int funct3 : 3;
        unsigned int rs1 : 5;
        unsigned int imm : 12;
    }Iformat; //I
    
    struct{
        unsigned int opcode : 7; //0x23
        unsigned int imm1 : 5;
        unsigned int funct3 : 3;
        unsigned int rs1 : 5;
        unsigned int rs2 : 5;
        unsigned int imm2 : 7;
        
    }Sformat; //S
    
    struct{
        unsigned int opcode : 7; //0x63
        unsigned int imm1 : 1;//13bit signed, [0] assumed to be zero.
        unsigned int imm2 : 4;
        unsigned int funct3 : 3;
        unsigned int rs1 : 5;
        unsigned int rs2 : 5;
        unsigned int imm3 : 6;
        unsigned int imm4 : 1;
    }SBformat; //SB
    
    struct{
        unsigned int opcode : 7; //0x67
        unsigned int rd : 5;
        unsigned int imm1 : 8; //21bit signed, [0] assumed to be zero.
        unsigned int imm2 : 1;
        unsigned int imm3 : 10;
        unsigned int imm4 : 1;
    }UJformat; //UJ
    
    struct{
        unsigned int opcode : 7; //0x37, 0x17
        unsigned int rd : 5;
        unsigned int imm : 20;
        
    }Uformat; //U
    
}FORMAT;

class RiscvSim{
    private:

    FORMAT inst;
    int reg[32];
    unsigned int Mem[Memsize];
    int PC;
    
    public: 
    
    RiscvSim(){
        initialize_Mem();
        PC = 0;
    }

    void read_one_inst(ifstream& fp){
        unsigned int buffer;
        fp.seekg(PC,ios::beg); //FORMAT inst updatedg); //set PC
        fp.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));
        parse(buffer); //FORMAT inst updated
        return;
    }

    void initialize_Mem(){
        for(int i=0;i<Memsize;i++){Mem[i] = FF;}
        return;
    }

    void write_Mem(int address,int val){ // get address 0x10000000~0x1000FFFF
        int index = (address - (1<<28)) / sizeof(int);
        Mem[index] = val;
        return;
    }

    int read_Mem(int address){ // get address 0x10000000~0x1000FFFF
        int index = (address - (1<<28)) / sizeof(int);
        return Mem[index];
    }

    void printRegs(){
        for(int i=0;i<32;i++){
            printf("x%d: 0x%08x\n",i,reg[i]);
        }
        return;
    }

    int sign_extension(int prev, int size){

    if((prev&(1<<(size-1)))!=0){
        return (prev | (-1 << size));
    }
    else return prev;
    }

    void executeDisassembledInst(){
        switch (inst.opcode){
            int imm;
        case 0x33: //R Format
            if(inst.Rformat.funct3==0x00 && inst.Rformat.funct7==0x00){ //add
                reg[inst.Rformat.rd] = reg[inst.Rformat.rs1]+reg[inst.Rformat.rs2];
            }
            else if(inst.Rformat.funct3==0x00 && inst.Rformat.funct7==0x20){//sub
                reg[inst.Rformat.rd] = reg[inst.Rformat.rs1]-reg[inst.Rformat.rs2];
            }
            else if(inst.Rformat.funct3==0x4 && inst.Rformat.funct7==0x00){//xor
                reg[inst.Rformat.rd] = (reg[inst.Rformat.rs1]&~reg[inst.Rformat.rs2])|(~reg[inst.Rformat.rs1]&reg[inst.Rformat.rs2]);
            }
            else if(inst.Rformat.funct3==0x6 && inst.Rformat.funct7==0x00){//or
                reg[inst.Rformat.rd] = reg[inst.Rformat.rs1]|reg[inst.Rformat.rs2];
            }
            else if(inst.Rformat.funct3==0x7 && inst.Rformat.funct7==0x00){//and
                reg[inst.Rformat.rd] = reg[inst.Rformat.rs1]&reg[inst.Rformat.rs2];
            }
            else if(inst.Rformat.funct3==0x1 && inst.Rformat.funct7==0x00){//sll
                reg[inst.Rformat.rd] = reg[inst.Rformat.rs1]<<reg[inst.Rformat.rs2];
            }
            else if(inst.Rformat.funct3==0x5 && inst.Rformat.funct7==0x00){//srl
                reg[inst.Rformat.rd] = (int)(((unsigned int)reg[inst.Rformat.rs1])>>reg[inst.Rformat.rs2]);
            }
            else if(inst.Rformat.funct3==0x5 && inst.Rformat.funct7==0x20){//sra
                reg[inst.Rformat.rd] = reg[inst.Rformat.rs1]>>reg[inst.Rformat.rs2];
            }
            else if(inst.Rformat.funct3==0x2 && inst.Rformat.funct7==0x00){//slt
                if(reg[inst.Rformat.rs1]<reg[inst.Rformat.rs2]) reg[inst.Rformat.rd] = 1;
                else reg[inst.Rformat.rd] = 0;
            }

            PC += 4;
            break;
        
        case 0x13: //I Format
            imm = sign_extension(inst.Iformat.imm,12);
            if(inst.Iformat.funct3==0x0){ //addi
                reg[inst.Iformat.rd] = reg[inst.Iformat.rs1] + imm;
            }
            else if(inst.Iformat.funct3==0x2){//slti
                if(reg[inst.Iformat.rs1] < imm) reg[inst.Iformat.rd] = 1;
                else reg[inst.Iformat.rd] = 0;
            }
            else if(inst.Iformat.funct3==0x4){//xori
                reg[inst.Rformat.rd] = (reg[inst.Iformat.rs1]&~imm)|(~reg[inst.Iformat.rs1]&imm);
            }
            else if(inst.Iformat.funct3==0x6){//ori
                reg[inst.Iformat.rd] = reg[inst.Iformat.rs1] | imm;
            }
            else if(inst.Iformat.funct3==0x7){//andi
                reg[inst.Iformat.rd] = reg[inst.Iformat.rs1] & imm;
            }
            else if(inst.Iformat.funct3==0x1){//slli
                reg[inst.Iformat.rd] = reg[inst.Iformat.rs1] << imm;
            }
            else if((inst.Iformat.funct3==0x5)&&(imm & (1<<10))){//srai
                reg[inst.Iformat.rd] = reg[inst.Iformat.rs1] >> imm;
            }
            else if(inst.Iformat.funct3==0x5){//srli
                reg[inst.Iformat.rd] = (int)((unsigned int)reg[inst.Iformat.rs1] >> imm);
            }
            PC += 4;
            break;

        case 0x03: //I Format
            imm = sign_extension(inst.Iformat.imm,12);
            if(inst.Iformat.funct3==0x2){//lw
            int address = reg[inst.Iformat.rs1]+imm;
                if((1<<29) == address ){ //0x20000000
                    cin >> reg[inst.Iformat.rd];
                }
                else{ // 0x10000000 ~ 0x1000FFFF
                    reg[inst.Iformat.rd] = read_Mem(address);
                }
            }
            PC += 4;
            break;

        case 0x67: //I Format jalr
            imm = sign_extension(inst.Iformat.imm,12);
            reg[inst.UJformat.rd] = PC + 4; //link
            PC = reg[inst.Iformat.rs1]+imm;
            break;
            

        case 0x23:
            //S Format
            imm = sign_extension(inst.Sformat.imm1|(inst.Sformat.imm2<<5),12);
            if(inst.Sformat.funct3==0x2){//sw
                int address = reg[inst.Sformat.rs1]+imm;
                if(address == (1<<29)){ //0x20000000
                    cout << (char)reg[inst.Sformat.rs2];
                }
                else{ //0x10000000 ~ 0x1000FFFF
                    write_Mem(address,reg[inst.Sformat.rs2]);
                }
            }
            PC += 4;
            break;

        case 0x63: //SB Format
            imm = sign_extension((inst.SBformat.imm1<<11)|(inst.SBformat.imm2<<1)|(inst.SBformat.imm3<<5)|(inst.SBformat.imm4<<12),13);
            if(inst.SBformat.funct3==0x0){ //beq
                if(reg[inst.SBformat.rs1] == reg[inst.SBformat.rs2]) PC = imm;
                else PC += 4; 
            }
            else if(inst.SBformat.funct3==0x1){//bne
                if(reg[inst.SBformat.rs1] != reg[inst.SBformat.rs2]) PC = imm;
                else PC += 4; 
            }

            else if(inst.SBformat.funct3==0x4){//blt
                if(reg[inst.SBformat.rs1] < reg[inst.SBformat.rs2]) PC = imm;
                else PC += 4; 
            }
            else if(inst.SBformat.funct3==0x5){//bge
                if(reg[inst.SBformat.rs1] >= reg[inst.SBformat.rs2]) PC = imm;
                else PC += 4; 
            }

            else
                printf("unknown instruction");
            
            break;
        case 0x17: //U Format auipc
            imm = inst.Uformat.imm<<12;
            reg[inst.Uformat.rd] = imm + PC;
            PC += 4;
            break;

        case 0x37: //U Format lui
            imm = inst.Uformat.imm<<12;
            reg[inst.Uformat.rd] = imm;
            PC += 4;
            break;

        case 0x6F: //UJ Format jal
            imm = sign_extension((inst.UJformat.imm1<<12)|(inst.UJformat.imm2<<11)|(inst.UJformat.imm3<<1)|(inst.UJformat.imm4<<20),21);
            reg[inst.UJformat.rd] = PC + 4; //link
            PC = imm;
            
            break;

        default:
            printf("unknown instruction");
            break;
        
        }
        reg[0] = 0; // reset x0 in case of change
        return;
    }

    void parse(unsigned int binary){
    inst.opcode = binary&((1U<<7)-1); //get last 7 bits
    binary >>= 7;

    switch (inst.opcode){

    case 0x33: // R Format
        
        inst.Rformat.rd = binary & ((1U << 5) - 1); //5 bits
        binary >>= 5;

        inst.Rformat.funct3 = binary & ((1U << 3) - 1); // 3 bits
        binary >>= 3;

        inst.Rformat.rs1 = binary & ((1U << 5) - 1); // 5 bits
        binary >>= 5;

        inst.Rformat.rs2 = binary & ((1U << 5) - 1);// 5 bits
        binary >>= 5;

        inst.Rformat.funct7 = binary & ((1U << 7) - 1);// 7 bits
    
        break;

    case 0x13: //I Format
    case 0x03:
    case 0x67:
        inst.Iformat.rd = binary & ((1U << 5) - 1); //5 bits
        binary >>= 5;

        inst.Iformat.funct3 = binary & ((1U << 3) - 1); // 3 bits
        binary >>= 3;

        inst.Iformat.rs1 = binary & ((1U << 5) - 1); // 5 bits
        binary >>= 5;

        inst.Iformat.imm = binary & ((1U << 12) - 1);// 12 bits
        break;

    case 0x23: //S Format
        inst.Sformat.imm1 = binary & ((1U << 5) - 1); // 5 bits
        binary >>= 5;

        inst.Sformat.funct3 = binary & ((1U << 3) - 1); // 3 bits
        binary >>= 3;

        inst.Sformat.rs1 = binary & ((1U << 5) - 1); // 5 bits
        binary >>= 5;

        inst.Sformat.rs2 = binary & ((1U << 5) - 1); // 5 bits
        binary >>= 5;

        inst.Sformat.imm2 = binary & ((1U << 7) - 1);// 7 bits
        break;

    case 0x63: //SB Format
        inst.SBformat.imm1 = binary & 1; // 1 bit
        binary >>= 1;

        inst.SBformat.imm2 = binary & ((1U << 4) - 1); // 4 bits
        binary >>= 4;

        inst.SBformat.funct3 = binary & ((1U << 3) - 1); // 3 bits
        binary >>= 3;

        inst.SBformat.rs1 = binary & ((1U << 5) - 1); // 5 bits
        binary >>= 5;

        inst.SBformat.rs2 = binary & ((1U << 5) - 1); // 5 bits
        binary >>= 5;

        inst.SBformat.imm3 = binary & ((1U << 6) - 1); // 6 bits
        binary >>= 6;

        inst.SBformat.imm4 = binary & 1; // 1 bit
        break;


    case 0x6F: //UJ format
        inst.UJformat.rd = binary & ((1U << 5) - 1); // 5 bit
        binary >>= 5;

        inst.UJformat.imm1 = binary & ((1U << 8) - 1); // 8 bits
        binary >>= 8;

        inst.UJformat.imm2 = binary & 1; // 1 bits
        binary >>= 1;

        inst.UJformat.imm3 = binary & ((1U << 10) - 1); // 10 bits
        binary >>= 10;
        
        inst.UJformat.imm4 = binary & 1; // 1 bit
        break;

    case 0x37: //U format
    case 0x17:
        inst.Uformat.rd = binary & ((1U << 5) - 1); // 5 bit
        binary >>= 5;
        
        inst.Uformat.imm = binary & ((1U << 20) - 1); // 20 bit
        break;
        }
        return;
    }
};

int main(int argc,char* argv[]){

    int instNum;
    int instcount = 0;
    int PC = 0;

    RiscvSim Riscv = RiscvSim();
    if(argc == 2){
        instNum = stoi(argv[2]);
    }
    else{
        instNum = stoi(argv[3]);
        /*
        int data;
        int address = 1<<28;
        ifstream df(argv[2], ios::binary);
        while(df.is_open()){
            df.read(reinterpret_cast<char*>(&data), sizeof(data));
            Riscv.write_Mem(address,data);
            address += 4;
        }
        df.close();
        */
    }

    ifstream fp(argv[1], ios::binary);   
    /*
    while(fp.is_open() && instcount < instNum){
        fp.seekg(PC,ios::beg); //update PC
        Riscv.read_one_inst(fp);
        instcount++;
    }
    Riscv.printRegs();
    */
    return 0;
}