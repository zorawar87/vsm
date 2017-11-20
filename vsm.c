/*
 * vsm.c: simulates a very simple machine.
 * Authors: Zorawar Moolenaar'20 && James Rodiger'19
 * Submission Date: 2017.11.19
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
 * Define supported instructions as macros
 */
#define EOC 0x0
#define LOAD 0x1
#define STORE 0x2
#define READ 0x3
#define WRITE 0x4
#define ADD 0x5
#define SUB 0x6
#define MUL 0x7
#define DIV 0x8
#define MOD 0x9
#define NEG 0xa
#define NOP 0xb
#define JUMP 0xc
#define JNEG 0xd
#define JZERO 0xe
#define HALT 0xf

/*
* Create aliases for relevant types
 */
typedef uint16_t word; // 16 bit integer
typedef int16_t sword; // signed word

/*
 * Declare registers
 */
static sword accumulator;
static word instructionCounter;
static word instructionRegister;
static word opCode;
static word operand;
static word m;

/*
 * Allocate Memory
 * * VSM supports a memory system comprised of 2,048 bytes
 * * Memory is an aggregation of all the words it stores
 *   * Here, memory is an aggregate of (2048*8/16==2048>>1) words
 * * Memory contains 1024 bits for CODE and DATA each.
 */
word memory[2048] = {0};

int loadProgramIntoMemory();
void fetch();
int decode();
void execute();
void coreDump();
/* Helpers */
void readFromStdin();
word getRealOperand();
int operandIsValidAddr();
void error (char *);

/**
 * Manages the VSM Simulator:
 *  [1] Load Program into Memory
 *  [2] Fetch instruction
 *  [3] Decode Instruction
 *  [4] Execute Instruction, if executable
 *  [5] Show Core Dump
 */
void main() {
  if (!loadProgramIntoMemory()) error ("Load Failure");
  int isExecutable = 0;
  do {
    fetch();
    isExecutable = decode();
    if (isExecutable) execute();
  } while (isExecutable);
  coreDump();
}

/*
 * Scans program from stdin and loads it into memory
 * Return 1 if any instructions were loaded; 0 otherwise
 * Responsibilities of a loader: (source: wikipedia)
 *  [N/A] validation (permissions, memory requirements etc.)
 *  [ X ] copying the program image from the disk (stdin) into main memory
 *  [N/A] copying command line args onto stack
 *  [ X ] initializing registers (e.g., the stack pointer)
 *  [ X ] Jump to start of Program (set instructionCounter to 0)
 * Returns 1 if code was successfully loaded; 0 otherwise
 */
int loadProgramIntoMemory() {
  instructionCounter = 0;
  while (scanf ("%04x", &instructionRegister) != EOF) {
    memory[instructionCounter++] = instructionRegister;
    if (instructionRegister == EOC) break;
  }
  // If fewer than two instruction were loaded, exit
  if (instructionCounter < 2) return 0;

  accumulator = 0;
  instructionCounter = 0;
  instructionRegister = 0;
  opCode = 0;
  operand = 0;
  return 1;
}

/*
 * Fetches an instruction:
 *  [X] Place an instruction from memory into the instruction register
 */
void fetch() {
  instructionRegister = memory[instructionCounter];
}

/*
 * Decodes an instruction from the instruction register and sets:
 *  [X] operation code
 *  [X] m-bit
 *  [X] operand
 * Returns 1 if opCode is executable; 0 otherwise
 */
int decode() {
  opCode = instructionRegister >> 12;
  printf("opCode: %x",opCode);
  if (opCode == HALT || opCode <= EOC) return 0;
  m = instructionRegister & 0x0800;
  operand = instructionRegister & 0x07ff;
  return 1;
}

/*
 * Executes an action based on opcode:
 *  [X] Implements execution
 *  [X] Handles validation of operand when necessary
 * Note: Exits in case of unexpected attempted execution
 */
void execute() {
  int i = 0;
  i++;
  printf("\n%d\n",i);
  switch (opCode) {
    case LOAD:
      accumulator = getRealOperand();
      break;
    case STORE:
      //exit if operand is outside the data region
      if (!m && !operandIsValidAddr());
      memory[operand] = accumulator;
      break;
    case READ:
      //exit if operand is outside the data region
      if (!m && !operandIsValidAddr())
	readFromStdin();
      break;
    case WRITE:
      //exit if operand is outside the data region
      if (!m && !operandIsValidAddr());
      printf ("%d\n", memory[operand]);
      break;
    case ADD:
      accumulator += getRealOperand();
      break;
    case SUB:
      accumulator -= getRealOperand();
      break;
    case MUL:
      accumulator *= getRealOperand();
      break;
    case DIV:
      // exit if operand is zero
      if (!getRealOperand()) error ("division by zero");
      accumulator /= getRealOperand();
      break;
    case MOD:
      accumulator %= getRealOperand();
      break;
    case NEG:
      accumulator = ~accumulator;
      break;
    case NOP:
      accumulator = accumulator;
      break;
    case JUMP:
      //subtract by one to offset increment at the end of every execution
      instructionCounter = operand / 2 - 1;
      break;
    case JNEG:
      //subtract by one to offset increment at the end of every execution
      if (accumulator < 0) instructionCounter = operand / 2 - 1;
      break;
    case JZERO:
      //subtract by one to offset increment at the end of every execution
      if (accumulator == 0) instructionCounter = operand / 2 - 1;
      break;
    default:
      error ("unknown opcode encountered");
  }
  instructionCounter++;
}

/*
 * Produces core dump (register+memory status) in accordance with given format
 */
void coreDump() {
  int i, j;
  int ncols = 10, nrows = 10;
  word value;

  printf ("\nREGISTERS:\n");
  printf ("accumulator               0x%04x\n", accumulator);
  printf ("instructionCounter        0x%04x\n", instructionCounter);
  printf ("instructionRegister       0x%04x\n", instructionRegister);
  printf ("operationCode             0x%1x \n", opCode);
  printf ("operand                   0x%04x\n", operand);

  printf ("\nCODE:\n   ");
  for (j = 0; j < ncols; j++)
    printf ("%3d", j); // print column number
  printf ("\n");

  for (i = 0; i < nrows; i++) {
    printf ("%04d ", i * 10); //print code location (precise to 10s place)
    for (j = 0; j < ncols / 2; j++) {
      value = memory[i * ncols + j];
      printf ("%02x ", value >> 8);  // prints MSBs
      printf ("%02x ", value & 0xFF); // prints LSBs
    }
    printf ("\n");
  }

  printf ("...\nDATA:\n   ");
  for (j = 0; j < ncols; j++)
    printf ("%3d", j); // print column number
  printf ("\n");
  for (i = 0; i < nrows; i++) {
    printf ("%04d ", 1024 + i * 10); //print data location (precise to 10s place)
    for (j = 0; j < ncols; j++) {
      value = memory[1024 + i * ncols + j];
      printf ("%02x ", value);  //print data value
    }
    printf ("\n");
  }
  printf ("...\n");
}

/*
 *
 * HELPER METHODS
 *
 */
/*
 * [X] Reads a 4 hex values from stdin
 * [X] Assigns value to location in memory specified as operand
 */
void readFromStdin() {
  word input;
  if (scanf ("%04x", &input) != 1 )
    error ("error reading");
  memory[operand] = input;
}

/*
 * Returns the true operand by checking the m-bit (intention)
 *  If m-bit is 0, then value in memory at operand is returned;
 *  otherwise operand is returned
 */
word getRealOperand() {
  if (!m && operandIsValidAddr())
    return memory[operand];
  return operand;
}

/*
 * checks if operand is an address within the data region
 * return 1 if it is; 0 otherwise
 */
int operandIsValidAddr() {
  if (operand < 1024 || operand > 2038) 
    error ("Invalid address is being accessed.");
  return 1;
}

void error (char *s) {
  printf ("ERROR: %s\n", s);
  printf ("Execution terminated abnormally. ");
  printf ("View core dump for more info:\n");
  coreDump();
  exit (EXIT_FAILURE);
}
