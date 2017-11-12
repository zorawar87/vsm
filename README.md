# VSM: Very Simple Machine (VSM) #
## Introduction ##
An opportunity to "peel open" a computer and look at its internal structure by
 programming in machine language, and simulating a VSM to execute machine
 language programs.
VSM uses a RISC based instruction set

## Part I: VSM Language (VSML) ##
### VSM Instruction Set Architecture ###
* Contains an _accumulator_, a special register which stores information 
 before the VSM uses it (???)
* All information is handled as a machine instruction
* Each machine instruction is a **word** long.
 
#### Components of Machine Instruction ####
* Bits 0 - 3: **op-code** (*operation code*)
  * Width: 1 nibble
  * indicates the action requested by the instruction
  * @see list of supported instructions
* Bits 3 - 5: **m** (*middle bit*)
  * Width: 2bits
  * LSB corresponds to 2nd operand
  * dictates whether operand is address (0) or value (1)

  * used only with `LOAD` and five supported arithmetic operations (`ADD`,
`SUB`,`MUL`,`DIV`,`MOD`)
  * 

#### Instruction Set ####
Op-code | Mnemonic  | Description
------- | --------- | -----------
`0000`  | `EOC`     | 
`0001`  | `LOAD`    | 
`0010`  | `STORE`   | 
`0011`  | `READ`    | 
`0100`  | `WRITE`   | 
`0101`  | `ADD`     | 
`0110`  | `SUB`     | 
`0111`  | `MUL`     | 
`1000`  | `DIV`     | 
`1001`  | `MOD`     | 
`1010`  | `NEG`     | 
`1011`  | `NOP`     | 
`1100`  | `JUMP`    | 
`1101`  | `JNEG`    | 
`1101`  | `JZERO`   | 
`1111`  | `HALT`    | 

### VSM Memory Layout ###
Suppot a memory system comprised of 2048 bytes, partitioned
into the `code` section (bits `0 - 1023`) and `data` section
(bits `1024-2047`)
  * a VSML program must be loaded into memory before running
  * first VSML instruction is placed at `bit 0`.
  * data required by the program should be stored in the `data` section.
  * all temporary data (variables) must be stored in the `data` section.

## Part II: The VSM Simulator ##
Create a software model of the VSM.
