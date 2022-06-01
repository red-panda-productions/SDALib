# Documentation

This is the documentation of the SDALib library, the layout of this document is the following:
- SDAData
  - Car
  - Situation
  - Tickcount
- SDAAction
  - Steer
  - Accel
  - Brake
  - Gear 
- SDADriver
- SDASimulate
- Example

## SDAData

A class that contains all of the data needed for a driver to understand the gamestate. It contains a Car struct, a Situation struct and a tickcount.

### Car

### Situation

### Tickcount

Represents the tick of the simulation on which the data was send. 

## SDAAction

An action struct contianing variables that edit the inputs of the car in the simulation. The different variables will be expained here as well as how they influence the system.

### Steer

This variable edits the steer input of the car by adding the value given to this variable to the input of the user. E.g. if the user input was 0.5 and the Steer variable is -0.5 the resulting steer would be 0.

### Accel

This variable edits the accel input of the car by adding the value given to this variable to the input of the user. E.g. if the user input was 0.5 and the Accel variable is 0.2 the resulting accel would be 0.7.

### Brake

This variable edits the brake input of the car by taking the maximum value of the user input and this variable. E.g. if the user input was 0.5 and the Brake variable is 0.7 the resulting brake would be 0.7.

### Gear

This variable currently does nothing, but might be added to the control of the AI in the future.

## SDADriver

A class that represents an SDADriver that can drive and edit inputs in the [DAISI](https://github.com/red-panda-productions/speed-dreams) simulator. This class will handle all of the communication and data transfer between DAISI and SDALib so that a developer can easily create an AI. A developer only has to inherit form this class and call the Run() function to begin.

## SDASimulate

A C function that can simulate an action in the Simulator.

## Example




