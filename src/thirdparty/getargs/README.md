# getargs
A simple C++ library for setting up and parsing command line arguments.

I've included some code as an example use-case in `src/example_main`. You can build the example code with either the dynamic or static versions of 'getargs' by running `make static_example` or `make dynamic_example`, respectively (make sure you built the necessary library, first!).

## A Note to Windows Users
I'm 99% sure that building a dynamic library for Windows does not work. Just build and/or use the static library instead (it's not that large, and I doubt I'm going to make very many changes). Once I can be arsed to get the DLL compiling correctly, I'll remove this section.
