#include "MyRobotSafetyProperties.hpp"

MyRobotSafetyProperties::MyRobotSafetyProperties(ControlSystem &cs, double dt)
    : cs(cs),
    
      slSystemOff("System is off"),
      slStop("Stop"),
      slStopping("Stopping"),
      slSwitchingOn("System is switching on"),
      slSystemOn("System is on"),

      seDoSystemOn("Startup the system"),
 //     seSwitchedOn("System is switched on"),
      seDoStop("do Stop"),
      seStopped("top do done"),
      seDoSystemOff("Shutdown the system")

{
    eeros::hal::HAL &hal = eeros::hal::HAL::instance();

    // Declare and add critical outputs
    // ... = hal.getLogicOutput("...");

    // criticalOutputs = { ... };

    // Declare and add critical inputs
    
    // ... = eeros::hal::HAL::instance().getLogicInput("...", ...);

    // criticalInputs = { ... };

    // Add all safety levels to the safety system
    addLevel(slSystemOff);
    addLevel(slStop);
    addLevel(slStopping);
    addLevel(slSwitchingOn);  
    addLevel(slSystemOn);

    // Add events to individual safety levels
    slStop.addEvent(seDoSystemOn, slSwitchingOn, kPublicEvent);
    slStopping.addEvent(seStopped, slStop, kPrivateEvent);
    slSwitchingOn.addEvent(seSwitchedOn, slSystemOn, kPrivateEvent);


    // Add events to multiple safety levels
    addEventToLevelAndAbove(slSwitchingOn, seDoStop, slStopping, kPublicEvent);
    // addEventToAllLevelsBetween(lowerLevel, upperLevel, event, targetLevel, kPublicEvent/kPrivateEvent);

    // Define input actions for all levels
    // level.setInputActions({ ... });

    // Define output actions for all levels
    // level.setOutputActions({ ... });

    // Define and add level actions
    slSystemOff.setLevelAction([&](SafetyContext *privateContext) {
        cs.timedomain.stop();
        eeros::Executor::stop();
    });

    slStop.setLevelAction([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(seStart);
    });

    slStopping.setLevelAction([&](SafetyContext *privateContext) {
    // Hier sollte CAN Stopbefehl "Motor stop (0x81)" oder "Motor shutdown (0x80)" ausgelöst werden"
    });

    slSwitchingOn.setLevelAction([&](SafetyContext *privateContext) {
    });

    slSystemOn.setLevelAction([&](SafetyContext *privateContext) {
        cs.timedomain.start();
    });

    // Define entry level
    setEntryLevel(slStop);

    // Define exit function
    exitFunction = ([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(doSwitchingOff);
    });
}
