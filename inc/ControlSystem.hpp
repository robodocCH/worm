#ifndef CONTROLSYSTEM_HPP_
#define CONTROLSYSTEM_HPP_

#include <eeros/control/TimeDomain.hpp>
#include <eeros/core/Executor.hpp>
#include <eeros/control/Constant.hpp>
#include <eeros/control/Gain.hpp>
#include <eeros/control/can/CanHandle.hpp>
#include <eeros/control/PeripheralInput.hpp>
#include <eeros/control/PeripheralOutput.hpp>
#include <eeros/control/PathPlannerCubic.hpp>
#include "CanSendRaw.hpp"
#include "CanReceiveRaw.hpp"

static constexpr uint8_t node1 = 1;           // id of node 1
static constexpr uint8_t node2 = 2;           // id of node 2
static constexpr uint8_t nofNodes = 2;        // nof nodes

using namespace eeros::control;

class ControlSystem
{
public:
    ControlSystem(double dt);
    /*
    CanHandle handle;
    CanSendRaw<nofNodes> canSend;
    CanReceiveRaw<nofNodes> canReceive;
    */

    
    PeripheralOutput<> servo3;
    PeripheralOutput<> servo4;
    PeripheralInput<> encoder1;
//    Constant<Matrix<nofNodes,1,double>> pos;
    Constant<> pos;
    PathPlannerCubic ppq;
    TimeDomain timedomain;

    /*
    // Define Blocks
    Constant<> myConstant;
    Gain<> myGain;

    TimeDomain timedomain;
    */
};

#endif // CONTROLSYSTEM_HPP