#include <stdio.h>

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
int sign_extension(int prev, int size){
    
    if((prev&(1<<(size-1)))!=0){
        return (prev | (-1 << size));
    }
    else return prev;
}

void printDisassembledInst(FORMAT inst){
    switch (inst.opcode){
        int imm;
    case 0x33: //R Format
        if(inst.Rformat.funct3==0x00 && inst.Rformat.funct7==0x00){ //add
            printf("add x%d, x%d, x%d",inst.Rformat.rd,inst.Rformat.rs1,inst.Rformat.rs2);
        }
        else if(inst.Rformat.funct3==0x00 && inst.Rformat.funct7==0x20){//sub
            printf("sub x%d, x%d, x%d",inst.Rformat.rd,inst.Rformat.rs1,inst.Rformat.rs2);
        }
        else if(inst.Rformat.funct3==0x4 && inst.Rformat.funct7==0x00){//xor
            printf("xor x%d, x%d, x%d",inst.Rformat.rd,inst.Rformat.rs1,inst.Rformat.rs2);
        }
        else if(inst.Rformat.funct3==0x6 && inst.Rformat.funct7==0x00){//or
            printf("or x%d, x%d, x%d",inst.Rformat.rd,inst.Rformat.rs1,inst.Rformat.rs2);
        }
        else if(inst.Rformat.funct3==0x7 && inst.Rformat.funct7==0x00){//and
            printf("and x%d, x%d, x%d",inst.Rformat.rd,inst.Rformat.rs1,inst.Rformat.rs2);
        }
        else if(inst.Rformat.funct3==0x1 && inst.Rformat.funct7==0x00){//sll
            printf("sll x%d, x%d, x%d",inst.Rformat.rd,inst.Rformat.rs1,inst.Rformat.rs2);
        }
        else if(inst.Rformat.funct3==0x5 && inst.Rformat.funct7==0x00){//srl
            printf("srl x%d, x%d, x%d",inst.Rformat.rd,inst.Rformat.rs1,inst.Rformat.rs2);
        }
        else if(inst.Rformat.funct3==0x5 && inst.Rformat.funct7==0x20){//sra
            printf("sra x%d, x%d, x%d",inst.Rformat.rd,inst.Rformat.rs1,inst.Rformat.rs2);
        }
        else if(inst.Rformat.funct3==0x2 && inst.Rformat.funct7==0x00){//slt
            printf("slt x%d, x%d, x%d",inst.Rformat.rd,inst.Rformat.rs1,inst.Rformat.rs2);
        }
        else if(inst.Rformat.funct3==0x3 && inst.Rformat.funct7==0x00){//sltu
            printf("sltu x%d, x%d, x%d",inst.Rformat.rd,inst.Rformat.rs1,inst.Rformat.rs2);
        }
        else{
            printf("unknown instruction");
        }
        break;
    
    case 0x13: //I Format
        imm = sign_extension(inst.Iformat.imm,12);
        if(inst.Iformat.funct3==0x0){ //addi
            printf("addi x%d, x%d, %d",inst.Iformat.rd,inst.Iformat.rs1,imm);
        }
        else if(inst.Iformat.funct3==0x2){//slti
            printf("slti x%d, x%d, %d",inst.Iformat.rd,inst.Iformat.rs1,imm);
        }
        else if(inst.Iformat.funct3==0x3){//sltiu
            printf("sltiu x%d, x%d, %d",inst.Iformat.rd,inst.Iformat.rs1,imm);
        }
        else if(inst.Iformat.funct3==0x4){//xori
            printf("xori x%d, x%d, %d",inst.Iformat.rd,inst.Iformat.rs1,imm);
        }
        else if(inst.Iformat.funct3==0x6){//ori
            printf("ori x%d, x%d, %d",inst.Iformat.rd,inst.Iformat.rs1,imm);
        }
        else if(inst.Iformat.funct3==0x7){//andi
            printf("andi x%d, x%d, %d",inst.Iformat.rd,inst.Iformat.rs1,imm);
        }
        else if(inst.Iformat.funct3==0x1){//slli
            printf("slli x%d, x%d, %d",inst.Iformat.rd,inst.Iformat.rs1,imm&31);
        }
        else if((inst.Iformat.funct3==0x5)&&(imm & (1<<10))){//srai
            printf("srai x%d, x%d, %d",inst.Iformat.rd,inst.Iformat.rs1,imm&31);
        }
        else if(inst.Iformat.funct3==0x5){//srli
            printf("srli x%d, x%d, %d",inst.Iformat.rd,inst.Iformat.rs1,imm&31);
        }
        else{
            printf("unknown instruction");
        }
        break;
    case 0x03: //I Format
        imm = sign_extension(inst.Iformat.imm,12);
        if(inst.Iformat.funct3==0x0){//lb
            printf("lb x%d, %d(x%d)",inst.Iformat.rd,imm,inst.Iformat.rs1);
        }
        else if(inst.Iformat.funct3==0x1){//lh
            imm = sign_extension(inst.Iformat.imm,12);
            printf("lh x%d, %d(x%d)",inst.Iformat.rd,imm,inst.Iformat.rs1);
        }
        else if(inst.Iformat.funct3==0x2){//lw
            printf("lw x%d, %d(x%d)",inst.Iformat.rd,imm,inst.Iformat.rs1);
        }
        else if(inst.Iformat.funct3==0x4){//lbu
            printf("lbu x%d, %d(x%d)",inst.Iformat.rd,imm,inst.Iformat.rs1);
        }
        else if(inst.Iformat.funct3==0x5){//lhu
            printf("lhu x%d, %d(x%d)",inst.Iformat.rd,imm,inst.Iformat.rs1);
        }
        else{
            printf("unknown instruction");
        }
        break;

    case 0x67: //I Format jalr
        imm = sign_extension(inst.Iformat.imm,12);
        printf("jalr x%d, %d(x%d)",inst.UJformat.rd,imm,inst.Iformat.rs1);
        break;
        

    case 0x23:
        //S Format
        imm = sign_extension(inst.Sformat.imm1|(inst.Sformat.imm2<<5),12);
        if(inst.Sformat.funct3==0x0){ //sb
            printf("sb x%d, %d(x%d)",inst.Sformat.rs2,imm,inst.Sformat.rs1);
        }
        else if(inst.Sformat.funct3==0x1){//sh
            printf("sh x%d, %d(x%d)",inst.Sformat.rs2,imm,inst.Sformat.rs1);
        }
        else if(inst.Sformat.funct3==0x2){//sw
            printf("sw x%d, %d(x%d)",inst.Sformat.rs2,imm,inst.Sformat.rs1);
        }
        else{
            printf("unknown instruction");
        }
        break;
    case 0x63: //SB Format
        imm = sign_extension((inst.SBformat.imm1<<11)|(inst.SBformat.imm2<<1)|(inst.SBformat.imm3<<5)|(inst.SBformat.imm4<<12),13);
        if(inst.SBformat.funct3==0x0){ //beq
            printf("beq x%d, x%d, %d",inst.SBformat.rs1,inst.SBformat.rs2,imm);
        }
        else if(inst.SBformat.funct3==0x1){//bne
            printf("bne x%d, x%d, %d",inst.SBformat.rs1,inst.SBformat.rs2,imm);
        }
        else if(inst.SBformat.funct3==0x4){//blt
            printf("blt x%d, x%d, %d",inst.SBformat.rs1,inst.SBformat.rs2,imm);
        }
        else if(inst.SBformat.funct3==0x5){//bge
            printf("bge x%d, x%d, %d",inst.SBformat.rs1,inst.SBformat.rs2,imm);
        }
        else if(inst.SBformat.funct3==0x6){//bltu
            printf("bltu x%d, x%d, %d",inst.SBformat.rs1,inst.SBformat.rs2,imm);
        }
        else if(inst.SBformat.funct3==0x7){//bgeu
            printf("bgeu x%d, x%d, %d",inst.SBformat.rs1,inst.SBformat.rs2,imm);
        }
        else{
            printf("unknown instruction");
        }
        break;

    case 0x17: //U Format auipc
        imm = inst.Uformat.imm<<12;
            printf("auipc x%d, %d",inst.Uformat.rd,imm);
            break;

    case 0x37: //U Format lui
        imm = inst.Uformat.imm<<12;
            printf("lui x%d, %d",inst.Uformat.rd,imm);
            break;

    case 0x6F: //UJ Format jal
        imm = sign_extension((inst.UJformat.imm1<<12)|(inst.UJformat.imm2<<11)|(inst.UJformat.imm3<<1)|(inst.UJformat.imm4<<20),21);
        printf("jal x%d, %d",inst.UJformat.rd,imm);
        break;

    default:
        printf("unknown instruction");
        break;
    
    }
    printf("\n");
    return;
}


FORMAT parse(unsigned int binary){
    FORMAT inst;
    inst.opcode = binary&((1U<<7)-1); //get last 7 bits
    binary >>= 7;
    switch (inst.opcode)
{
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
    return inst;
}



int main(int argc,char* argv[]){
    unsigned int inst;
    int i=0;

    FILE* fp = fopen(argv[1],"rb");
    if(fp==NULL) return 0;
   
    while(!feof(fp)){
        while(fread(&inst,sizeof(inst),1,fp) ==1 ){
            printf("inst %d: ",i);
            printf("%08x ",inst);
            
            printDisassembledInst(parse(inst));
            
            i++;
    }
    }
    fclose(fp);
    
    return 0;
}
