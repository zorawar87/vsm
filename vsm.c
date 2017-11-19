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
static int16_t accumulator;
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
void error(char *);

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
  if (instructionCounter < 2) return 0;

  accumulator = 0;
  instructionCounter = 0;
  instructionRegister = 0;
  opCode = 0;
  operand = 0;
  return 1;
}

/*
 * Fetch an instruction
 */
void fetch() {
  instructionRegister = memory[instructionCounter];
}


/*
 * Decode an instruction
 */
int decode() {
  opCode = instructionRegister >> 12;
  if (opCode == HALT || opCode <= EOC) return 0;
  m = instructionRegister & 0x0800;
  operand = instructionRegister & 0x07ff;
  return 1;
}


void execute() {
  switch (opCode) {
    case LOAD:
      accumulator = getRealOperand();
      instructionCounter++;
      break;
    case STORE:
      if (!m && !operandIsValidAddr()) return;
      memory[operand] = accumulator;
      instructionCounter++;
      break;
    case READ:
      if (!m && !operandIsValidAddr()) return;
      readFromStdin();
      instructionCounter++;
      break;
    case WRITE:
      if (!m && !operandIsValidAddr()) return;
      printf ("%d\n", memory[operand]);
      instructionCounter++;
      break;
    case ADD:
      accumulator += getRealOperand();
      instructionCounter++;
      break;
    case SUB:
      accumulator -= getRealOperand();
      instructionCounter++;
      break;
    case MUL:
      accumulator *= getRealOperand();
      instructionCounter++;
      break;
    case DIV:
      if (!getRealOperand()) error ("division by zero");
      accumulator /= getRealOperand();
      instructionCounter++;
      break;
    case MOD:
      accumulator %= getRealOperand();
      instructionCounter++;
      break;
    case NEG:
      accumulator *= -1;
      instructionCounter++;
      break;
    case NOP:
      accumulator = accumulator;
      instructionCounter++;
      break;
    case JUMP:
      instructionCounter = operand/2;
      break;
    case JNEG:
      if (accumulator < 0) instructionCounter = operand/2;
      else instructionCounter++;
      break;
    case JZERO:
      if (accumulator == 0) instructionCounter = operand/2;
      else instructionCounter++;
      break;
    default:
      error("unknown opcode");
  }
}

void coreDump(){
    int i, j;
    int ncols = 10, nrows =10;
    word value;

    printf("\nREGISTERS:\n");
    printf("accumulator               0x%04x\n", accumulator);
    printf("instructionCounter        0x%04x\n", instructionCounter);
    printf("instructionRegister       0x%04x\n", instructionRegister);
    printf("operationCode             0x%1x \n", opCode);
    printf("operand                   0x%04x\n", operand);

    printf("\nCODE:\n");
    printf("   "); //padding for column number
    for (j = 0; j < ncols; j++)
        printf("%3d", j); // print column number
    printf("\n");

    for (i = 0; i < nrows; i++) {
        printf("%04d ", i * 10); //print memory precision to 10s place
        for (j = 0; j < ncols/2; j++) {
            value= memory[i * ncols + j];
            printf("%02x ", value >> 8);   // prints MSBs
            printf("%02x ", value & 0xFF); // prints LSBs
        }
        printf("\n");
    }

    printf("\nDATA:\n");
    printf("   ");
    for (j = 0; j < ncols; j++)
        printf("%3d", j);
    printf("\n");
    for (i = 0; i < nrows; i++) {
        printf("%04d ",1024+ i * 10);
        for (j = 0; j < ncols; j++) {
            value= memory[1024+i * ncols + j];
            printf("%02x ", value);
        }
        printf("\n");
    }
}

/* 
 * HELPER METHODS
 */
void readFromStdin() {
  word input;
  if (scanf ("%04x", &input) != 1 ) error("error reading");
  memory[operand] = input;
}

word getRealOperand() {
  // if operand is not a value (i.e. is an address),
  // try to fetch the value at the address
  if (!m && operandIsValidAddr())
    return memory[operand];
  return operand;
}

int operandIsValidAddr() {
  // not a valid address if beyond 2038 or below 1024
  if (operand < 1024 || operand > 2038) error("operand is invalid addr.");
  return 1;
}

void error(char *s){
  printf("ERROR: %s\n",s);
  printf("Execution terminated abnormally. View core dump for more info:\n");
  coreDump();
  exit(EXIT_FAILURE);
}
