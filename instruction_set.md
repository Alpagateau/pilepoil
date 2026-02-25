# VM Core Instruction Set

| Instruction | Op1 | Op2 | Descrition                     |
| ----------- | --- | --- | ------------------------------ |
| NOP         | x   | x   | Does nothing                   |
| LNK         | Str | x   | Checks function was linked     |
| PUSH        | Val | x   | Pushes Val on the stack        |
| POP         | x   | x   | Pops the head of the stack     |
| SWP         | x   | x   | Swaps the first 2 values       |
| ROLL        | N   | x   | "sinks" the N first values     |
| RWD         | N   | x   | "floats" the N first values    |
| DUP         | x   | x   | Duplicates the first value     |
| ADD         | x   | x   | Pops 2 values and adds them    |
| SUB         | x   | x   | pop a b, push b - a            |
| MUL         | x   | x   | pop a b, push b * a            |
| SHR         | x   | x   | pop a, push a / 2              |
| SHL         | x   | x   | pop a, push a * 2              |
| PPC         | x   | x   | Push program conter on stack   |
| JMP         | Add | x   | Jumps to said addresse         |
| JPS         | x   | x   | Jumps to addresse on the stack |
| JPZ         | Add | x   | Jumps to add if zero flag      |
| JNZ         | Add | x   | Jumps to add if !zero flag     |
| JPN         | Add | x   | Jumps to add if negative flag  |
| JPP         | Add | x   | Jumps to add if !negative flag |
| CALL        | Add | x   | PCC + JUMP                     |
| VMCALL      | Str | x   | Calls a linked func from vm    |
| DEBUG       | x   | x   | Prints the current stack       |
| HALT        | x   | x   | Stops the program              |
