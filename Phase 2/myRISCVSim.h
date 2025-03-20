#ifndef MYRISCVSIM_H
#define MYRISCVSIM_H

// Function Prototypes
void run_RISCVsim();
void reset_proc();
void load_program_memory(char* file_name);
void write_data_memory();
void swi_exit();

// Instruction Execution Functions
void fetch();
void decode();
void execute();
void mem();
void write_back();

// Memory Functions
unsigned int read_word(char *mem, unsigned int address,unsigned int instructn);
void write_word(char *mem, unsigned int address, unsigned int data, int instruntn);

#endif // MYRISCVSIM_H
