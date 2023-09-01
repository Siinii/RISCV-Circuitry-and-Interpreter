# RISCV-Circuitry-and-Interpreter
RISCV CPU Circuitry in Logisim, and RISCV Instruction Interpreter in C

# Circuitry
- For implementation details, see the pdf write-up file.
- Parts implemented:
  - Steps:
    - Fetch Step
    - Decode Step
    - Execute Step
    - Access Memory Step
    - Write Back Step
  - Instructions:
    -     Format	Instructions
          R-type:	ADD, SUB, AND, OR, XOR, SLT, SLL, SRA
          I-type:	ADDI, ANDI, ORI, XORI, SLTI, LW, LB
          S-type:	SW, SB
          U-type:	LUI

 # Interpreter
 - The interpreter is written in C, and supports the following instructions:
   
      - R-type:	ADD, SUB, AND, OR, XOR, SLT, SLL, SRA
      - I-type:	ADDI, ANDI, ORI, XORI, SLTI
      - Memory-type:	LW, LB, SW, SB
      - U-type:	LUI
  - Additionally, two ways to simulate the memory the processor will use have been implemented: as a Linked List, and as a Hash Table
  
