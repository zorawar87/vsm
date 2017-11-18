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
typedef uint8_t   byte; // 08 bit integer
typedef uint16_t  word; // 16 bit integer

/*
 * Initialise registers
 */
static word accumulator;
static word instructionCounter;
static word instructionRegister;
static word opCode;
static word operand;


/*
 * Allocate Memory
 * * VSM supports a memory system comprised of 2,048 bytes
 * * Memory is an aggregation of all the words it stores
 *   * Here, memory is an aggregate of (2048*8/16==2048>>1) words
 * * Memory contains 1024 bits for CODE and DATA each.
 */
struct {
  word code[2048>>1];
  word data[2048>>1];
} memory;


int loadProgramIntoMemory();
int executeInstruction();

void main() {
  if (!loadProgramIntoMemory()) exit (EXIT_FAILURE);
  while (executeInstruction());
}

/*
 * Scans program from stdin and loads it into memory
 * Return 1 if any instructions were loaded; 0 otherwise
 * Responsibilities of a loader:
 *  ? validation (permissions, memory requirements etc.)
 *  * copying the program image from the disk (stdin) into main memory
 *  - XXX copying the command-line arguments on the stack XXX
 *  * initializing registers (e.g., the stack pointer)
 *  ? ??? jumping to the program entry point (_start) ???
 */
int loadProgramIntoMemory() {
  instructionCounter = 0;
  while (scanf ("%04x", &instructionRegister) != EOC)
    memory.code[instructionCounter++] = instructionRegister;

  /* Quit if nothing was loaded */
  if (!instructionCounter) return 0;

  accumulator = 0;
  instructionCounter = 0;
  instructionRegister = 0;
  opCode = 0;
  operand = 0;
  return 1;
}

int executeInstruction() {
  instructionRegister = memory.code[instructionCounter];
  //instruction_t.op_code = instructionRegister;
  //printf("%x\n\n",(nibble)instruction_t.op_code);
  // get 
  opCode = instructionRegister >> 12;
  printf("OPCODE: %x\n",opCode);
  operand = instructionRegister % 100000;
  printf("OP%x\n",operand);
  switch (opCode){
    case EOC:
      printf("%s\n","EOC");
      break;
    case LOAD:
      printf("%s\n","Load");
      break;
    case STORE:
      printf("%s\n","Load");
      break;
    case READ:
      break;
    case WRITE:
      break;
    case ADD:
      break;
    case SUB:
      break;
    case MUL:
      break;
    case DIV:
      break;
    case MOD:
      break;
    case NEG:
      break;
    case NOP:
      break;
    case JUMP:
      break;
    case JNEG:
      break;
    case JZERO:
      break;
    case HALT:
      return 0;
    default: exit(EXIT_FAILURE);
  }
  instructionCounter++;
  return 1;
}
