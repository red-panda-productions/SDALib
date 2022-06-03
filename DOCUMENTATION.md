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

This struct stores all data from the car. It contains the following data from the Car:
- info
  - information about the name and car properties.
- pub
  - public information about the car
  - position, rotation
  - speed (DynGC.x, the speed float is bugged)
  - trkpos
    - Segments of the track. Has a lookahead of 10 segments.
- race
  -  (unnecessary) information about the race
- priv
  - Private information of the car
  - fuel, engine, gear information
- ctrl
  - information about the inputs of the user
  - steer, accel, brake, clutch information
- setup
  - (unnecessary) information about the setup of the car
  - gearratios, differential, etc...
- pitcmd
  - (unnecessary) information about going into th pits

### Situation

This struct stores all data from the current situation.
- raceInfo
  -  amount of cars, laps and total time
-  deltaTime
-  currentTime
-  accelTime
-  number of players
-  (unset) list of cars

### Tickcount

Represents the tick of the simulation on which the data was sent. 

## SDAAction

An action struct containing  variables that edit the inputs of the car in the simulation. The different variables will be expained here, as well as how they influence the system.

### Steer

This variable edits the steer input of the car by adding the value given to this variable to the input of the user. E.g. if the user input was 0.5 and the Steer variable is -0.5 the resulting steer would be 0.

### Accel

This variable edits the accel input of the car by adding the value given to this variable to the input of the user. E.g. if the user input was 0.5 and the Accel variable is 0.2 the resulting accel would be 0.7.

### Brake

This variable edits the brake input of the car by taking the maximum value of the user input and this variable. E.g. if the user input was 0.5 and the Brake variable is 0.7 the resulting brake would be 0.7.

### Gear

This variable currently does nothing, but might be added to the control of the AI in the future.

## SDADriver

A class that represents an SDADriver that can drive and edit inputs in the [DAISI](https://github.com/red-panda-productions/speed-dreams) simulator. This class will handle all of the communication and data transfer between DAISI and SDALib so that a developer can easily create an AI. A developer has to inherit from this class, implement and override the InitAI() and UpdateAI() functions and call the Run() function to begin.

## SDASimulate

A C function that can simulate an action in the Simulator.

## Example

Here is an example driver that will brake when the speed of the car is too high:

```
#include <iostream>
#include "SDADriver.hpp"

/// @brief A sample driver that brakes when driving above a set max speed
class BrakeSampleDriver : public SDADriver
{
public:
    /// @brief Constructor for setting the maximum allowed speed
    BrakeSampleDriver(float p_maxSpeed)
        : SDADriver()
    {
        m_maxSpeed = p_maxSpeed;
    }

protected:
    ///@brief Initializes the AI, but this AI does not need initialization (still needs to be overridden)
    void InitAI() override
    {
        // do nothing
    }
  
    ///@brief Updates the driver with p_data. The return type should be an SDAAction to give the choices back to the simulation
    SDAAction UpdateAI(SDAData& p_data) override
    {
        SDAAction action;

        // full brake when going 20km/h above max speed
        action.Brake = std::max(0.0f, (p_data.Car.pub.DynGC.vel.x - m_maxSpeed) / 20);
        std::cout << p_data.Car.pub.DynGC.vel.x << " " << action.Brake << std::endl;

        return action;
    }

private:
    float m_maxSpeed;
};

///@brief The main function that calls Run() of BrakeSampleDriver
int main()
{
    BrakeSampleDriver brakeSampleDriverDriver(80);
    brakeSampleDriverDriver.Run();

    return 0;
}
```


