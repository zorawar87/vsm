/*
 * vsm.c: simulates a very simple machine.
 * Authors: James Rodiger'19 && Zorawar Moolenaar'20
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
typedef uint8_t  byte; // 08 bit integer
typedef uint16_t word; // 16 bit integer

/*
 * Declare registers
 */
static word accumulator;
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
/* Helpers */
word getRealOperand();
int isValidDataAddress();

void main() {
  if (!loadProgramIntoMemory()) exit (EXIT_FAILURE);
  int isExecutable = 0;
  do {
    fetch();
    isExecutable = decode();
    if (isExecutable) execute();
  } while (isExecutable);
}

/*
 * Scans program from stdin and loads it into memory
 * Return 1 if any instructions were loaded; 0 otherwise
 * Responsibilities of a loader: (source: wikipedia)
 *  [ ] validation (permissions, memory requirements etc.)
 *  [X] copying the program image from the disk (stdin) into main memory
 *  [X] initializing registers (e.g., the stack pointer)
 */
int loadProgramIntoMemory() {
  instructionCounter = 0;
  while (scanf ("%04x", &instructionRegister) != EOF) {
    memory[instructionCounter++] = instructionRegister;
    if (instructionRegister == EOC) break;
  }
  /* If only one instruction was loaded, exit */
  if (instructionCounter < 4) return 0;

  accumulator = 0;
  instructionCounter = 0;
  instructionRegister = 0;
  opCode = 0;
  operand = 0;
  return 1;
}

void fetch() {
  printf ("fetching instruction %s", instructionCounter);
  instructionRegister = memory[instructionCounter];
  instructionCounter += 2;
  // Plus two, instead of one, because of
  // how the output is meant to be organised
}


int decode() {
  opCode = instructionRegister >> 12;
  if (opCode == HALT || opCode == EOC) {
    printf ("stopped decoding at %d\n", instructionCounter);
    return 0;
  }
  m = instructionRegister & 0x0800;
  //?? should isValidDataAddrCheck be done here?
  operand = instructionRegister & 0x07ff;
  return 1;
}

void read() {
  printf ("READING\n");
  // checks if the operand is pointing to a valid place in memory
  if (!isValidDataAddress()) return;
  word input;
  if (scanf ("%04x", &input) != 1 ) return;
  memory[operand] = input;
  printf ("\t\tmemory[%d]: %d\n", operand, memory[operand]);
}

void write() {
  printf ("WRITIGN\n");
  if (!isValidDataAddress()) return;
  printf ("%d\n", memory[operand]);
}

void store() {
  if (!isValidDataAddress()) return;
  memory[operand] = accumulator;
}

void execute() {
  switch (opCode) {
    case LOAD:
      accumulator = getRealOperand();
      break;
    case STORE:
      store();
      break;
    case READ:
      printf ("reading\n");
      read();
      break;
    case WRITE:
      printf ("writing\n");
      write();
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
      word value = getRealOperand();
      if (!word) {printf ("ERR:division by zero"); exit (EXIT_FAILURE);}
      accumulator /= value;
      break;
    case MOD:
      accumulator %= getRealOperand();
      break;
    case NEG:
      accumulator *= -1;
      break;
    case NOP:
      accumulator = accumulator;
      break;
    case JUMP:
      instructionCounter = operand;
      break;
    case JNEG:
      if (accumulator < 0) instructionCounter = operand;
      break;
    case JZERO:
      if (accumulator == 0) instructionCounter = operand;
      break;
    default: printf ("EXITING: unknown opcode %d\n", opCode); exit (EXIT_FAILURE);
  }
}

word getRealOperand() {
  // if operand is not a value (i.e. is an address),
  // try to fetch the value at the address
  if (!m & isValidDataAddress())
    return memory[operand];
  return operand;
}

int isValidDataAddress() {
  // not a valid address if beyond 2038 or below 1024
  if (operand < 1024 || operand > 2038) return 0;
  return 1;
}
