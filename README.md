# VSM: Very Simple Machine (VSM) #
Implemented by Zorawar Moolenaar'20 && James Rodiger'19

## Introduction ##
A very simple machine that uses RISC instruction set, and supports VSML (VSM language) with comments :)

## VSM Instruction Set Architecture ##
* All information is handled as a machine instruction
* Each machine instruction is a **word** long.

### Registers
  * accumulator
  * instructionCounter
  * instructionRegister
  * opCode
  * operand

### Supported Instructions ###
op-code | Mnemonic  | Description
------- | --------- | -----------
`0000`  | `EOC`     | End of code section
`0001`  | `LOAD`    | Load a word at a specific location in memory (or a number) into the accumulator
`0010`  | `STORE`   | Store a word in the accumulator into a specific location in memory
`0011`  | `READ`    | Read a word from the standard input into a specific location in memory
`0100`  | `WRITE`   | Write a word at a specific location in memory to the standard output
`0101`  | `ADD`     | Add a word at a specific location in memory (or a number) to the word in the accumulator, leaving the sum in the accumulator.
`0110`  | `SUB`     | Subtract a word at a specific location in memory (or a number) from the word in the accumulator, leaving the difference in the accumulator.
`0111`  | `MUL`     | Multiply the word in the accumulator by a word at a specific location in memory (or a number), leaving the product in the accumulator.
`1000`  | `DIV`     | Divide the word in the accumulator by a word at a specific location in memory (or a number), leaving the quotient in the accumulator.
`1001`  | `MOD`     | Divide the word in the accumulator by a word at a specific location in memory (or a number), leaving the remainder in the accumulator
`1010`  | `NEG`     | Negate the word in the accumulator
`1011`  | `NOP`     | No operation (sets accumulator to value of accumulator)
`1100`  | `JUMP`    | Branch to a specific location in memory.
`1101`  | `JNEG`    | Branch to a specific location in memory if the accumulator is negative.
`1110`  | `JZERO`   | Branch to a specific location in memory if the accumulator is zero
`1111`  | `HALT`    | The program is terminated.

### VSM Memory Layout ###
  * VSM has 2048 bytes of memory.
    * 1024 bytes for code,
    * 1024 bytes for data.
  * a VSML program is loaded into memory before running
  * first VSML instruction is placed at `bit 0`.
  * data required by the program is stored in the `data` section.
  * all temporary data (variables) is stored in the `data` section.

## VSML ##
  * all instructions must be 16-digit binary strings
  * anything after 16 digits is assumed to be a comment --- no special syntax is requried
 
### VSML Instruction ###
  * first four bits represent the operation code
  * fifth bit represent the m-bit
    * m-bit = 0 ==> operand is an address
    * m-bit = 1 ==> operand is a value (only valid for `LOAD`,`ADD`,)
  * sixth to sixteenth bits represent the operand, which can be:
    * A literal binary number
    * An address in the data section
    * An address in the code section (valid for `/JXXXX?/`, but not enforced)

# Note #
* VSM is obviously insecure. Jumps are unchecked. The entire memory is virtually executable.
* For more details, see `spec.pdf`
