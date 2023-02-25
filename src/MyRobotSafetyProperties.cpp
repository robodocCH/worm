#include "MyRobotSafetyProperties.hpp"

MyRobotSafetyProperties::MyRobotSafetyProperties(ControlSystem &cs, double dt)
    : cs(cs),
    
      slSystemOff("System is off"),
      slSwitchingOff("System is switching off"),
      slSwitchingOn("System is switching on"),
      slSystemOn("System is on"),

      doSystemOn("Startup the system"),
      switchedOn("System is switched on")
      doSystemOff("Shutdown the system")
      switchedOff("System is switched off")

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
    addLevel(slSwitchingOff);
    addLevel(slSwitchingOn);
    addLevel(slSystemOn);

    // Add events to individual safety levels
    slSystemOff.addEvent(doSystemOn, slSwitchingOn, kPublicEvent);
    slSwitchingOff.addEvent(switchedOff, slSystemOff, kPrivateEvent);
    slSwitchingOn.addEvent(switchedOn, slSystemOn, kPrivateEvent);

    // Add events to multiple safety levels
    addEventToLevelAndAbove(slSwitchingOn, slSwitchingOff, doSwitchingOff, kPublicEvent);
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

    slSwitchingOff.setLevelAction([&](SafetyContext *privateContext) {
        // Hier sollte CAN Stopbefehl "Motor stop (0x81)" oder "Motor shutdown (0x80)" ausgelöst werden"
    });

    slSwitchingOn.setLevelAction([&](SafetyContext *privateContext) {
    });

    slSystemOn.setLevelAction([&](SafetyContext *privateContext) {
        cs.timedomain.start();
    });

    // Define entry level
    setEntryLevel(slSystemOff);

    // Define exit function
    exitFunction = ([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(doSwitchingOff);
    });
}
