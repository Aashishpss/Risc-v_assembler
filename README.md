# Cs204_risc-v Assembler 
# Assembly to Machine Code Converter

## Overview
This project is an **Assembly to Machine Code Converter** that takes an assembly language file (`.asm`) as input and generates the corresponding machine code in an output file (`.mc`).

## Project Structure
```
|-- final_code.cpp        # Main program file
|-- prefinal_code.cpp     # Intermediate processing file
|-- prefinal_with_binary_mc.cpp     # Another intermediate processing file
|-- merged_without_water.cpp   #Intermediate progress file
|-- conv_label.cpp            # Intermediate progress file
|-- input.asm            # Sample input assembly file
|-- output.mc            # Generated machine code output
|-- README.md            # Project documentation
```

## Getting Started
To use this project, you need to:
1. **Download the `final_code.cpp` file** (This is the main program file that handles the assembly conversion).
2. **Compile the file using a C++ compiler** (such as g++ or clang++).
3. **Create an input file (`input.asm`)** and write the assembly instructions.
4. **Run the executable and get the output in `output.mc`**.

## Installation & Compilation
### Using g++:
```bash
# Clone the repository
git clone https://github.com/your-repo/assembly-to-mc.git
cd assembly-to-mc

# Compile the final_code.cpp file
g++ -o assembler final_code.cpp

# Run the assembler
./assembler input.asm output.mc
```

## Usage
1. **Prepare an input file:**
   - Create a new file `input.asm` and write your assembly instructions.
   - Example `input.asm`:
     ```assembly
     MOV R1, #5
     ADD R2, R1, #3
     SUB R3, R2, #2
     ```

2. **Run the assembler:**
   ```bash
   ./assembler input.asm output.mc
   ```
   This will generate a `output.mc` file with the translated machine code.

3. **Check the output file (`output.mc`)**
   - The file will contain the generated machine code along with a final line:
     ```
     done assembling
     ```

## Notes
- **Intermediate files (`intermediate1.cpp`, `intermediate2.cpp`, etc.)** are used only for internal processing and should not be run separately.
- **Make sure to download `final_code.cpp` and follow the instructions above to execute it properly.**





