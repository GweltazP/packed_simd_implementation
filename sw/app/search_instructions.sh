#!/bin/bash

# List of instructions to search for
instructions=("sra16" "srai16" "srl16" "srli16" "sll16" "slli16" "ksll16" "kslli16" "kslra16"
              "sra8" "srai8" "srl8" "srli8" "sll8" "slli8" "ksll8" "kslli8" "kslra8"
              "khm16" "khmx16" "khm8" "khmx8"
              "sclip16" "uclip16" "kabs16" "clrs16" "clz16" "clo16" "swap16"
              "sclip8" "uclip8" "kabs8" "clrs8" "clz8" "clo8" "swap8"

              "smmul" "kmmac" "kmmsb" "kwmmul"
            )

# Path to the disassembly file
file="mnist.D"

# Loop through each instruction and search
for instr in "${instructions[@]}"; do
    echo "===== Searching for '$instr' ====="
    grep -n "$instr" "$file"
    echo ""
done
