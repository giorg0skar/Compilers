
# Compiler for Dana language

Dana is a programming language supporting static type safety, low level programming and nested functions. It was created for the compilers class in NTUA. The lexer and parser are built using flex and bison and the code generation is done using LLVM.

# Building
Full compiler in LLVM directory. To build:

``cd LLVM``

``make``

# Execute

```
    ./danac [input file] [flags]
    ./a.out
```

input file: name of the input file (must end in .dna)

flags:
* -r : create IR file in output
* -f : create assembly file in output
* -O1: optimization flag
* -O2: optimization flag
* -O3: optimization flag

If **-r** or **-f** flags are chosen there is no a.out executable file. Instead a [input file].ll or [input file].s are created respectively.