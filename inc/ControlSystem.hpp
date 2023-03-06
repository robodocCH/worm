#ifndef CONTROLSYSTEM_HPP_
#define CONTROLSYSTEM_HPP_

#include <eeros/control/TimeDomain.hpp>
#include <eeros/core/Executor.hpp>
#include <eeros/control/Constant.hpp>
#include <eeros/control/Gain.hpp>
#include <eeros/control/Sum.hpp>
#include <eeros/control/Mux.hpp>
#include <eeros/control/filter/LowPassFilter.hpp>
#include <eeros/control/Saturation.hpp>
#include <eeros/control/Gain.hpp>
#include <eeros/control/PathPlannerConstAcc.hpp>
#include <eeros/control/PeripheralInput.hpp>
#include <eeros/control/PeripheralOutput.hpp>
#include <eeros/control/PathPlannerCubic.hpp>
#include <customBlocks/FourierSignalSource.hpp>
#include <eeros/control/can/CanHandle.hpp>
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
    CanHandle handle;
    CanSendRaw<nofNodes> canSend;
    CanReceiveRaw<nofNodes> canReceive;
    Constant<Matrix<nofNodes,1,double >> pos;
    
    // Define Blocks    
    PeripheralInput<> encoder1;
    PeripheralInput<> encoder2;
    PeripheralOutput<> servo3;
    PeripheralOutput<> servo4;
    PathPlannerCubic ppq1;
    PathPlannerCubic ppq2;
    Gain<> gain1;
    Gain<> gain2;
    Saturation<> preSaturation1;
    Saturation<> preSaturation2;
    Saturation<> saturation1;
    Saturation<> saturation2;
    FourierSignalSource<> breath;  
    FourierSignalSource<> tremor1;  
    FourierSignalSource<> tremor2;  
    Mux<2> mux;     
    Sum<3> sum1;
    Sum<2> sum2;
    LowPassFilter<> filter1;
    LowPassFilter<> filter2;

 //   Constant<> pos;
    
    TimeDomain timedomain;
};

#endif // CONTROLSYSTEM_HPP