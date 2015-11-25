SimpleVM
=====
SVM is a simple stack based virtual machine. It uses a simple language that is compiled to bytecode. It's all written in C++ and will be ported to C if I get enough time :p

The sample scripts are in Scripts/ directory

Building
=====
You need premake5 (Better to clone and build the latest commit), clang++-3.6 (not GCC or VC or MinGW, just Clang) and an application that can execute GNU makefiles :)
```
premake5 gmake
make help
```
