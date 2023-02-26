#include "ControlSystem.hpp"

ControlSystem::ControlSystem(double dt)
/*
      : handle("can0"),
        canSend(handle.getSocket(), {node1, node2}),
        canReceive(handle.getSocket(), {node1, node2}),
*/
      : encoder1("enc1"),
        servo3("servo3"),
        servo4("servo4"),
        ppq(3.0), // dt = 3.0

        timedomain("Main time domain", dt, true)
{
    pos.setValue(0.0);
    ppq.getPosOut().getSignal().setValue(0.0); // Startposition setzen

    /*
    canSend.setName("CAN send");
    canReceive.setName("CAN receive");
    canReceive.getOut().getSignal().setName("position");
    Matrix<2,1,double> scale({10, 10});    // accounts for encoder resolution and gear box
    canSend.setScale(scale);
    canSend.getIn().connect(pos.getOut());
    */

    // Name all blocks
    encoder1.setName("Encoder 1");
    servo3.setName("Servo 3");
    servo4.setName("Servo 4");
    pos.setName("SetpointBlock");
    ppq.setName("ppcubic");
    ppq.getPosOut().getSignal().setName("pp pos out");
    ppq.getVelOut().getSignal().setName("pp vel out");
    ppq.getAccOut().getSignal().setName("pp acc out");
    ppq.getJerkOut().getSignal().setName("pp jerk out");
    ppq.init("path01.txt");
    
    //myConstant.setName("My constant");
    //myGain.setName("My gain");

    // Name all signals
    // Hier müssten die Bezeichnungen der Signale kommen
    // myConstant.getOut().getSignal().setName("My constant value");
    // myGain.getOut().getSignal().setName("My constant value multiplied with my gain"); 

    // Connect signals
    //servo3.getIn().connect(encoder1.getOut());
    servo3.getIn().connect(ppq.getPosOut());
    servo4.getIn().connect(pos.getOut());

    // Momentan wird nichts anderes verbunden
    // myGain.getIn().connect(myConstant.getOut()); 

    // Add blocks to timedomain
    // timedomain.addBlock(canReceive);
    // timedomain.addBlock(pos);
    // timedomain.addBlock(canSend);
    timedomain.addBlock(ppq);    
    timedomain.addBlock(encoder1);
    timedomain.addBlock(pos);
    timedomain.addBlock(servo3);
    timedomain.addBlock(servo4);

    // timedomain.addBlock(myConstant);
    // timedomain.addBlock(myGain);

    // Add timedomain to executor
    eeros::Executor::instance().add(timedomain);

}