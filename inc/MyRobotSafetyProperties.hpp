#ifndef MyRobotSAFETYPROPERTIES_HPP_
#define MyRobotSAFETYPROPERTIES_HPP_

#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/hal/HAL.hpp>
#include "ControlSystem.hpp"

class MyRobotSafetyProperties : public eeros::safety::SafetyProperties
{
public:
    MyRobotSafetyProperties(ControlSystem &cs, double dt);

    // Define all possible events
    eeros::safety::SafetyEvent doSystemOn; 
    eeros::safety::SafetyEvent switchedOn;
    eeros::safety::SafetyEvent doSystemOff;
    eeros::safety::SafetyEvent switchedOff;
    

    // Defina all possible levels
    eeros::safety::SafetyLevel slSystemOff;
    eeros::safety::SafetyLevel slSwitchingOff;
    eeros::safety::SafetyLevel slSwitchingOn;
    eeros::safety::SafetyLevel slSystemOn;
    

private:
    // Define all critical outputs
    // eeros::hal::Output<bool>* ...;

    // Define all critical inputs
    // eeros::hal::Input<bool>* ...;

    ControlSystem &cs;
};

#endif // MyRobotSAFETYPROPERTIES_HPP_
