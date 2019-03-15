
# Compiler for Dana language

Dana is a programming language supporting static type safety, low level programming and nested functions. It was created for the compilers class in NTUA. The lexer and parser are built using flex and bison and the code generation is done using LLVM.

# Building
Full compiler in LLVM directory. To build:

``cd LLVM``

``make``

# Execute
```bash
./danac input.dna [flags]
```

The executable file's name is  `a.out`.

The danac executable supports the following flags:
* -r : create LLVM IR file in output
* -f : create x86_64 assembly file in output
* -O1: optimization flag
* -O2: optimization flag
* -O3: optimization flag

If **-r** or **-f** flags are chosen there is no executable file. Instead an `input.ll` or `input.s` file is created respectively.