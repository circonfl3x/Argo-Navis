# Argo-Navis
### Experimental compiler (transpiler?) written in pure C

This is a small compiler/transpiler that compiles/transpiles to C. (you get the point)
Right now its feature set is really small (it was put together in like a few-ish days), the code is messy (very) and
the concepts still not well developed, but my aim is just a simple programming language. You can't even pass arguments as of now,
it just reads from a file called test.ag and writes out to one called out.c


## Features
- Memory safety (obviously)... It's built off another memory management framework that I've made (Lyra). I'll avoid weird C undefined behaviour and all variables will probably have to be declared
- Garbage collector - the framework for one already exists, but making a smart one will probably be much tougher
- Simplicity, it most likely won't have all the OOP features. Maybe I'll look into putting structs? It will be C like in structure but much much simpler
- Strong typed. All types are defined explicitly (maybe might compromise on this?), anyway its not like there will be many types

## What works?
- Variable declaration. The syntax is just like C so for example - `string a = "Hello"`
- function declaration, but for now that does nothing... There is just a framework to handle reading functions... The syntax is  `fn name(arg1, arg2, ....){}`
- there is one "primitive function" for now... `externC.print(arg)` it takes one argument that can be anything and prints it out (no the "externC." doesn't mean that there will be classes)
- For now there are 3 types - string, char and int (more to come ofc)

## Plans
- Add floats, doubles
- Add proper function declarations
- Add argument passing to the binary so the user can declare their input/output files, compiler flags etc
- Add more primitive functions
- Add rudimentary memory management (just clears all memory at the end of the program)
- Add scoped variables
- Add proper file inclusion
- And I'll think of more as it goes along
