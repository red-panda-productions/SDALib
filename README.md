# SDALib

A framework to create AI for assisted cars for Linux and Windows. 
This Library can connect to the [DAISI](https://github.com/red-panda-productions/speed-dreams) simulation to adjust the controls of the human player in the simulation. The AI (currently) has full acces to edit steer, acceleration and brake of the car.

## Download

You can download the library from the [Releases](https://github.com/red-panda-productions/SDALib/releases) page on the repository. There you will find 2 different kind of packages:
- premade (can be seen by having 'premade' in their file name)
- standalone

Premade files contain premade projects so that you can immediately start with creating your AI. The standalone files are there for when you have an existing project that you want to add this library to.

Furthermore you can always create your project within the source code of SDALib by downloading or cloning the repo.

## Using SDALib in your project

When you use the project we suggest you use CMake and copy over the different .cmake files into a cmake folder in your repository. The SDAMacros.cmake file consists of macros like FIND_SDA_PACKAGES and INSTALL_SDA_PACKAGES(target) that can easily find and install all of the dependencies needed for SDALib into your project. After calling these macros the setup of SDALib is done and you can begin programming your AI.

## Documentation

We suggest reading the documentation before you start creating your AI.

## Samples

The library also consists of 2 Samples namely [SDASteerSample](https://github.com/red-panda-productions/SDALib/tree/main/SDALib-CPP/Samples/SDASteerSample) and [SDABrakeSample]([SDASteerSample](https://github.com/red-panda-productions/SDALib/tree/main/SDALib-CPP/Samples/SDABrakeSample). 

## Notes

This project was created by students of Utrecht University.
