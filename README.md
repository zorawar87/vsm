# VSM: Very Simple Machine (VSM) #
A 16-bit "very simple" machine that uses RISC instruction set, and 
supports VSML (VSM language) with comments :)

Implemented by Zorawar Moolenaar'20 && James Rodiger'19


## Build ##
Compile 
```
gcc binstr2hex.c -o binstr2hex
gcc vsm.c -o vsm
```

## Run ##
Every VSML program must be converted to hex for the vsm to parse instructions.
`binstr2hex` does just this. `run.sh` is an executable (read: `ugo+x`) utility
that combines binary string to hex conversion, and then asks vsm to interpret
the program.

To execute any VSML compliant program (see `spec.md` or `spec.pdf`)
execute `sh run.sh <program name>`.
Sample code has been provided: `prime.vsml`, `primes.vsml` (with comments),


## Specification ##
> "Code *is* Documentation" -- Matin Fowler
Going through the code might take less time to understand what's going on :D

That said, see `spec.md` or `spec.pdf`
