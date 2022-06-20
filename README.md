![Tests linux](https://github.com/red-panda-productions/SDALib/actions/workflows/test-linux.yml/badge.svg)
![Coverage linux](https://github.com/red-panda-productions/SDALib/actions/workflows/code-coverage-linux.yml/badge.svg) 
![Tests windows](https://github.com/red-panda-productions/SDALib/actions/workflows/test-windows.yml/badge.svg)
![Coverage windows](https://github.com/red-panda-productions/SDALib/actions/workflows/code-coverage-windows.yml/badge.svg) 

# SDALib

A framework to create AI for assisted cars for Linux (64 bit) and Windows (32 bit) in C++ and Python.
This library can connect to the [DAISI](https://github.com/red-panda-productions/DAISI) simulation to adjust the controls of the human player in the simulation. The AI (currently) has full access to edit steer, acceleration and brake of the car.

## Download

You can download the library from the [Releases](https://github.com/red-panda-productions/SDALib/releases) page on the repository. There you will find 2 different kind of packages:
- premade (can be seen by having 'premade' in their file name)
- premade-python
- standalone

Premade files contain premade projects so that you can immediately start with creating your AI. The standalone files are there for when you have an existing project that you want to add this library to.

Furthermore, you can always create your project within the source code of SDALib by downloading or cloning the repository.

## Using SDALib in your C++ project

When you use the project on Windows, you will need to use a 32-bit compiler. On Linux you can use 64 bit. Furthermore, we suggest that you use CMake, and that you copy over the different .cmake files into a CMake folder in your repository. The SDAMacros.cmake file consists of macros like `FIND_SDA_PACKAGES` and `INSTALL_SDA_PACKAGES(target)` that can easily find and install all of the dependencies needed for SDALib into your project. After calling these macros the setup of SDALib is done, and you can begin programming your AI.

## Using SDALib in your Python project

On Windows you will need to have a 32 bit version of python installed, on Linux you can use 64 bit. When you use the python project you will get an executable and 2 files, SDATypes.py and SDADriver.py. You can build your AI in SDADriver.py. When you want to use your driver with [DAISI](https://github.com/red-panda-productions/DAISI) be sure to select the SDALib-Python.exe executable. The executable will load your driver into the Python runtime and give data to your python driver

## Building on Linux

On Linux, you will need to pass the -pthread compiler flag to your compiler, and you will need to link your driver executable with the rt library (which is installed on every distro of Linux). 

## Documentation

We suggest reading the [documentation](https://github.com/red-panda-productions/SDALib/blob/main/DOCUMENTATION.md) before you start creating your AI.

## Samples

The library also consists of 2 samples, namely [SDASteerSample](https://github.com/red-panda-productions/SDALib/tree/main/SDALib-CPP/Samples/SDASteerSample) and [SDABrakeSample](https://github.com/red-panda-productions/SDALib/tree/main/SDALib-CPP/Samples/SDABrakeSample). 

## Notes

This project was created by students of Utrecht University.
