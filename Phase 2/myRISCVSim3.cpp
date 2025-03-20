/* RISC-V Functional Simulator */

#include "myRISCVSim.h"
#include <stdio.h>
#include <stdlib.h>

// Register file
static unsigned int R[32];
static unsigned int PC = 0x0;
static unsigned char MEM[4000];
static unsigned int instruction_word;
static unsigned int operand1, operand2;
static unsigned int clock_cycles = 0;

void run_RISCVsim() {
    while (1) {
        fetch();
        decode();
        execute();
        mem();
        write_back();
        clock_cycles++;
    }
}

void reset_proc() {
    for (int i = 0; i < 32; i++) R[i] = 0;
    for (int i = 0; i < 4000; i++) MEM[i] = 0;
    PC = 0x0;
    clock_cycles = 0;
}

void load_program_memory(char *file_name) {
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("Error opening input file\n");
        exit(1);
    }
    unsigned int address, instruction;
    while (fscanf(fp, "%x %x", &address, &instruction) != EOF) {
        write_word(MEM, address, instruction);
    }
    fclose(fp);
}

void write_data_memory() {
    FILE *fp = fopen("data_out.mem", "w");
    if (fp == NULL) {
        printf("Error writing to output file\n");
        return;
    }
    for (unsigned int i = 0; i < 4000; i += 4) {
        fprintf(fp, "%x %x\n", i, read_word(MEM, i));
    }
    fclose(fp);
}

void swi_exit() {
    printf("Total clock cycles: %d\n", clock_cycles);
    write_data_memory();
    exit(0);
}

void fetch() {
    instruction_word = read_word(MEM, PC);
    printf("FETCH: Instruction 0x%x from PC 0x%x\n", instruction_word, PC);
    PC += 4;
}

void decode() {
    unsigned int opcode = instruction_word & 0x7F;
    unsigned int rd = (instruction_word >> 7) & 0x1F;
    unsigned int funct3 = (instruction_word >> 12) & 0x7;
    unsigned int rs1 = (instruction_word >> 15) & 0x1F;
    unsigned int rs2 = (instruction_word >> 20) & 0x1F;
    unsigned int funct7 = (instruction_word >> 25) & 0x7F;
    unsigned int imm;
    
    if (opcode == 0x33) { // R-type
        operand1 = R[rs1];
        operand2 = R[rs2];
    } else if (opcode == 0x13) { // I-type
        operand1 = R[rs1];
        imm = (instruction_word >> 20) & 0xFFF;
    } else if (opcode == 0x3) { // Load
        operand1 = R[rs1];
        imm = (instruction_word >> 20) & 0xFFF;
    } else if (opcode == 0x23) { // Store
        operand1 = R[rs1];
        operand2 = R[rs2];
        imm = ((instruction_word >> 7) & 0x1F) | ((instruction_word >> 25) << 5);
    } else if (opcode == 0x63) { // Branch
        operand1 = R[rs1];
        operand2 = R[rs2];
        imm = ((instruction_word >> 7) & 0x1E) | ((instruction_word >> 25) << 5);
    }
}

void execute() {
    if ((instruction_word & 0x7F) == 0x33) { // R-type
        switch ((instruction_word >> 12) & 0x7) {
            case 0x0: R[(instruction_word >> 7) & 0x1F] = operand1 + operand2; break; // ADD
            case 0x1: R[(instruction_word >> 7) & 0x1F] = operand1 << (operand2 & 0x1F); break; // SLL
            case 0x4: R[(instruction_word >> 7) & 0x1F] = operand1 ^ operand2; break; // XOR
        }
    } else if ((instruction_word & 0x7F) == 0x63) { // Branch
        if (((instruction_word >> 12) & 0x7) == 0x0 && operand1 == operand2) { // BEQ
            PC += ((instruction_word >> 7) & 0x1E) | ((instruction_word >> 25) << 5);
        }
    }
}

void mem() {
    if ((instruction_word & 0x7F) == 0x3) { // Load
        R[(instruction_word >> 7) & 0x1F] = read_word(MEM, operand1);
    } else if ((instruction_word & 0x7F) == 0x23) { // Store
        write_word(MEM, operand1, operand2);
    }
}

void write_back() {
    printf("WRITEBACK: Completing instruction cycle\n");
}

int read_word(unsigned char *mem, unsigned int address) {
    return *((int *)(mem + address));
}

void write_word(unsigned char *mem, unsigned int address, unsigned int data) {
    *((int *)(mem + address)) = data;
}