#ifndef MyRobotSAFETYPROPERTIES_HPP_
#define MyRobotSAFETYPROPERTIES_HPP_

#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/hal/HAL.hpp>
#include "ControlSystem.hpp"

class MyRobotSafetyProperties : public eeros::safety::SafetyProperties
{
public:
    MyRobotSafetyProperties(ControlSystem &cs, double dt);

    // Defina all possible levels
    eeros::safety::SafetyLevel slSystemOff;
    eeros::safety::SafetyLevel slStop;
    eeros::safety::SafetyLevel slStopping;
    eeros::safety::SafetyLevel slSwitchingOn;    
    eeros::safety::SafetyLevel slSystemOn;
    
    // Define all possible events
    eeros::safety::SafetyEvent seDoSystemOn; 
 //   eeros::safety::SafetyEvent seSwitchedOn;
    eeros::safety::SafetyEvent seDoStop;
    eeros::safety::SafetyEvent seStopped;
    eeros::safety::SafetyEvent seDoSystemOff;
    

private:
    // Define all critical outputs
    // eeros::hal::Output<bool>* ...;

    // Define all critical inputs
    // eeros::hal::Input<bool>* ...;

    ControlSystem &cs;
};

#endif // MyRobotSAFETYPROPERTIES_HPP_
