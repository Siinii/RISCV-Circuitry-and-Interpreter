#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "linkedlist.h"
#include "hashtable.h"
#include "riscv.h"
#include <string.h> // std::string, std::stoi

/************** BEGIN HELPER FUNCTIONS PROVIDED FOR CONVENIENCE ***************/
const int R_TYPE = 0;
const int I_TYPE = 1;
const int MEM_TYPE = 2;
const int U_TYPE = 3;
const int UNKNOWN_TYPE = 4;

/**
 * Return the type of instruction for the given operation
 * Available options are R_TYPE, I_TYPE, MEM_TYPE, UNKNOWN_TYPE
 */
static int get_op_type(char *op)
{
    const char *r_type_op[] = {"add", "sub", "and", "or", "xor", "slt", "sll", "sra"};
    const char *i_type_op[] = {"addi", "andi", "ori", "xori", "slti"};
    const char *mem_type_op[] = {"lw", "lb", "sw", "sb"};
    const char *u_type_op[] = {"lui"};
    for (int i = 0; i < (int)(sizeof(r_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(r_type_op[i], op) == 0)
        {
            return R_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(i_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(i_type_op[i], op) == 0)
        {
            return I_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(mem_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(mem_type_op[i], op) == 0)
        {
            return MEM_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(u_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(u_type_op[i], op) == 0)
        {
            return U_TYPE;
        }
    }
    return UNKNOWN_TYPE;
}
/*************** END HELPER FUNCTIONS PROVIDED FOR CONVENIENCE ****************/

registers_t *registers;
hashtable_t *mem;

void init(registers_t *starting_registers)
{
    registers = starting_registers;
    mem = ht_init(1000);
}

void end()
{
    ht_free(mem);
    free(registers);
}

int iImmDecode(char *immediate)
{
    // decodes a 12 bit immediate input into an int
    int val;
    if (immediate[1] == 'x')
    {
        val = strtol(immediate, NULL, 0);
    }
    else
        val = strtol(immediate, NULL, 0);
    return val;
}
long uImmDecode(char *immediate)
{
    // decodes a 20 bit immediate input into a long
    long val;
    if (immediate[1] == 'x')
    {
        val = strtol(immediate, NULL, 0);
    }
    else
        val = strtol(immediate, NULL, 0);
    return val;
}

void step(char *instruction)
{
    // Extracts and returns the substring before the first space character,
    // by replacing the space character with a null-terminator.
    // `instruction` now points to the next character after the space
    // See `man strsep` for how this library function works
    char *op = strsep(&instruction, " ");
    // Uses the provided helper function to determine the type of instruction
    int op_type = get_op_type(op);
    // Skip this instruction if it is not in our supported set of instructions
    if (op_type == UNKNOWN_TYPE)
    {
        return;
    }

    // Parsing R-Type
    if (op_type == R_TYPE)
    {
        char *trash = strsep(&instruction, "x");
        char *rd = strsep(&instruction, ",");
        trash = strsep(&instruction, "x");
        char *rs1 = strsep(&instruction, ",");
        trash = strsep(&instruction, "x");
        char *rs2 = instruction;

        int rdVal;
        if (strcmp(op, "add") == 0)
        {
            rdVal = registers->r[atoi(rs1)] + registers->r[atoi(rs2)];
        }
        if (strcmp(op, "sub") == 0)
        {
            rdVal = registers->r[atoi(rs1)] - registers->r[atoi(rs2)];
        }
        if (strcmp(op, "and") == 0)
        {
            rdVal = registers->r[atoi(rs1)] & registers->r[atoi(rs2)];
        }
        if (strcmp(op, "or") == 0)
        {
            rdVal = registers->r[atoi(rs1)] | registers->r[atoi(rs2)];
        }
        if (strcmp(op, "xor") == 0)
        {
            rdVal = registers->r[atoi(rs1)] ^ registers->r[atoi(rs2)];
        }
        if (strcmp(op, "sll") == 0)
        {
            rdVal = registers->r[atoi(rs1)] << registers->r[atoi(rs2)];
        }
        if (strcmp(op, "sra") == 0)
        {
            rdVal = (registers->r[atoi(rs1)] >> registers->r[atoi(rs2)]);
        }
        if (strcmp(op, "slt") == 0)
        {
            rdVal = (registers->r[atoi(rs1)] < registers->r[atoi(rs2)]) ? 1 : 0;
        }
        if (atoi(rd) != 0)
        {
            registers->r[atoi(rd)] = rdVal;
        }
    }

    // Parsing I-Type
    if (op_type == I_TYPE)
    {
        char *trash = strsep(&instruction, "x");
        char *rd = strsep(&instruction, ",");
        trash = strsep(&instruction, "x");
        char *rs1 = strsep(&instruction, ",");
        char *imm = strsep(&instruction, " ");

        int rdVal;

        int immVal = iImmDecode(instruction);

        if (strcmp(op, "addi") == 0)
        {

            rdVal = registers->r[atoi(rs1)] + immVal;
        }
        if (strcmp(op, "andi") == 0)
        {
            rdVal = registers->r[atoi(rs1)] & immVal;
        }
        if (strcmp(op, "ori") == 0)
        {
            rdVal = registers->r[atoi(rs1)] | immVal;
        }
        if (strcmp(op, "xori") == 0)
        {
            rdVal = registers->r[atoi(rs1)] ^ immVal;
        }
        if (strcmp(op, "slti") == 0)
        {
            rdVal = (registers->r[atoi(rs1)] < immVal) ? 1 : 0;
        }
        if (atoi(rd) != 0)
        {
            registers->r[atoi(rd)] = rdVal;
        }
    }

    // Parsing U-Type
    if (op_type == U_TYPE)
    {
        char *trash = strsep(&instruction, "x");
        char *rd = strsep(&instruction, ",");
        char *imm = instruction;

        long immVal = uImmDecode(imm);

        if (atoi(rd) != 0)
        {
            registers->r[atoi(rd)] = (immVal << 12);
        }
    }

    // Parsing Mem-type
    if (op_type == MEM_TYPE)
    {
        char *trash = strsep(&instruction, "x");
        char *rd = strsep(&instruction, ",");
        char *offset = strsep(&instruction, "x");
        char *rs2 = strsep(&instruction, ")");

        int address = iImmDecode(offset) + registers->r[atoi(rs2)];

        if (strcmp(op, "sb") == 0)
        {
            int val = registers->r[atoi(rd)];
            ht_add(mem, address, val);
        }
        if (strcmp(op, "lb") == 0)
        {
            int val = ht_get(mem, address);
            if (val > 127)
            {
                val = val << 24;
                val = val >> 24;
            }

            if (atoi(rd) != 0)
                registers->r[atoi(rd)] = val;
        }
        if (strcmp(op, "sw") == 0)
        {
            int val = registers->r[atoi(rd)];
            int b1 = val & 0xFF;
            int b2 = (val >> 8) & 0xFF;
            int b3 = (val >> 16) & 0xFF;
            int b4 = (val >> 24) & 0xFF;

            ht_add(mem, address, b1);
            ht_add(mem, address + 1, b2);
            ht_add(mem, address + 2, b3);
            ht_add(mem, address + 3, b4);
        }
        if (strcmp(op, "lw") == 0)
        {

            int word = 0;
            for (int i = 0; i < 4; i++)
            {
                int shift = i * 8;
                int single_byte = (ht_get(mem, address + i) & 255) << shift;

                word = word | single_byte;
            }
            if (atoi(rd) != 0)
                registers->r[atoi(rd)] = word;
        }
    }
}
