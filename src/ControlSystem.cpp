#include "ControlSystem.hpp"

ControlSystem::ControlSystem(double dt)
      : handle("can0"),
        canSend(handle.getSocket(), {node1, node2}),
        canReceive(handle.getSocket(), {node1, node2}),
        timedomain("Main time domain", ts, true)
{
    pos.setValue({0, 0});
    canSend.setName("CAN send");
    canReceive.setName("CAN receive");
    canReceive.getOut().getSignal().setName("position");
    Matrix<2,1,double> scale({10, 10});    // accounts for encoder resolution and gear box
    canSend.setScale(scale);
    canSend.getIn().connect(pos.getOut());

    timedomain.addBlock(canReceive);
    timedomain.addBlock(pos);
    timedomain.addBlock(canSend);

    Executor::instance().add(timedomain);

    /*
    // Name all blocks
    myConstant.setName("My constant");
    myGain.setName("My gain");

    // Name all signals
    myConstant.getOut().getSignal().setName("My constant value");
    myGain.getOut().getSignal().setName("My constant value multiplied with my gain");

    // Connect signals
    myGain.getIn().connect(myConstant.getOut());

    // Add blocks to timedomain
    timedomain.addBlock(myConstant);
    timedomain.addBlock(myGain);

    // Add timedomain to executor
    eeros::Executor::instance().add(timedomain);
    */

}