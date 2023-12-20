This was a project in CSE 40243 at the University of Notre Dame taken under Douglas Thain (http://compilerbook.org). Given the specifications of a C-like language (Bminor), we wrote the stages of a compiler: scanner, parser, printer, type checker, and code generator. We used tools like Flex to parse tokens for the scanner, and Bison to write the grammar for the parser. By the end, we were able to generate X86 assembly which could then be linked with the standard C library to be turned into an executable.

To make and run different flags to run stages of the compiler:

```
> make
> ./bminor -codegen program.bminor program.s
> gcc -g myprogram.s library.c -o myprogram
> ./myprogram
```

Language Overview: https://dthain.github.io/compilers-fa23/bminor
