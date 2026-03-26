# RoseTinted
A proof of concept game using the Nostalgia game engine

## Building RoseTinted
RoseTinted uses CMake and supports both Windows and Linux. Currently, due to Microsoft not implementing C++26, you'll need to install the "C++ Clang Compiler for Windows" and "MSBuild support for LLVM (clang-cl) toolset" components for VisualStudio.

RoseTinted makes use of my custom game engine, [Nostalgia](https://github.com/Electron7-7/nostalgia-game-engine) and is currently built on version [0.7](https://github.com/Electron7-7/nostalgia-game-engine/releases/tag/v0.7.0). If you don't want to install Nostalgia to your computer (or you're compiling on Windows), you can just place the library files in `rose-tinted/src/lib/` (you'll need to create the 'lib/' folder first).

Once you're all set, all that's next is to use CMake to configure the project and generate the build files. I recommend using the standard Makefiles on Linux; on Windows, make sure you're using VisualStudio 18 as that's the version I use (you might succeed with earlier versions, but I haven't tested any so your mileage may vary).
