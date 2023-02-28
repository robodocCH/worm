#include "ControlSystem.hpp"

ControlSystem::ControlSystem(double dt)
/*
      : handle("can0"),
        canSend(handle.getSocket(), {node1, node2}),
        canReceive(handle.getSocket(), {node1, node2}),
*/
      : encoder1("enc1"),
        encoder2("enc2"),
        servo3("servo3"),
        servo4("servo4"),
        ppq1(dt),
        ppq2(dt),
        breath(2.544690049, -0.0045, 0.0201, 0.005, 0.002, 0.0016), 
        tremor1(2.544690049, -0.0045, 0.0201, 0.005, 0.002, 0.0016), // ++++++++++ Zahlen aendern ++++++++++++++++++++++++++
        tremor2(2.544690049, -0.0045, 0.0201, 0.005, 0.002, 0.0016), // ++++++++++ Zahlen aendern ++++++++++++++++++++++++++
        // tremor: FourierSignalSource(double omega, double a0, double a1=0, double a2=0, double a3=0, double a4=0)
        // 2.544690049	-0.0045	0.0201	0.005	0.002	0.0016
        timedomain("Main time domain", dt, true)
{
    pos.setValue(0.0);
    ppq1.getPosOut().getSignal().setValue(0.0); // Startposition setzen
    ppq2.getPosOut().getSignal().setValue(0.0); // Startposition setzen
    breath.getOut().getSignal().setValue(0.0); // Startposition setzen
    tremor1.getOut().getSignal().setValue(0.0); // Startposition setzen
    tremor2.getOut().getSignal().setValue(0.0); // Startposition setzen

    /*
    canSend.setName("CAN send");
    canReceive.setName("CAN receive");
    canReceive.getOut().getSignal().setName("position");
    Matrix<2,1,double> scale({10, 10});    // accounts for encoder resolution and gear box
    canSend.setScale(scale);
    canSend.getIn().connect(pos.getOut());
    */

    // Name all blocks
    pos.setName("SetpointBlock");
    encoder1.setName("Encoder 1");
    encoder2.setName("Encoder 2");
    servo3.setName("Servo 3");
    servo4.setName("Servo 4");

    breath.setName("Breathing Signal Source");

    ppq1.setName("Pathplanner Cubic 1");
    tremor1.setName("Tremor1 Signal Source 1");
    sum1.setName("Summe 1");  

    ppq2.setName("Pathplanner Cubic 2");
    tremor2.setName("Tremor1 Signal Source 2");    
    sum2.setName("Summe 2");


    // Name all signals
    ppq1.getPosOut().getSignal().setName("Pathplanner Cubic 1 pos out");
    ppq1.getVelOut().getSignal().setName("Pathplanner Cubic 1 vel out");
    ppq1.getAccOut().getSignal().setName("Pathplanner Cubic 1 acc out");
    ppq1.getJerkOut().getSignal().setName("Pathplanner Cubic 1 jerk out");

    ppq2.getPosOut().getSignal().setName("Pathplanner Cubic 2 pos out");
    ppq2.getVelOut().getSignal().setName("Pathplanner Cubic 2 vel out");
    ppq2.getAccOut().getSignal().setName("Pathplanner Cubic 2 acc out");
    ppq2.getJerkOut().getSignal().setName("Pathplanner Cubic 2 jerk out");

    // ppq1.init("path01.txt");
    // ppq2.init("path01.txt");
    // Dieser Init sollte nichts tun!    


    // Connect signals
    //servo3.getIn().connect(encoder1.getOut());
    // servo4.getIn().connect(pos.getOut());  
    // servo3.getIn().connect(ppq1.getPosOut());
    // servo4.getIn().connect(tremor1.getOut());

////////////////////////////

    sum1.getIn(0).connect(ppq1.getPosOut());
    sum1.getIn(1).connect(breath.getOut());
    sum1.getIn(2).connect(tremor1.getOut());
    servo3.getIn().connect(sum1.getOut());

    sum2.getIn(0).connect(ppq2.getPosOut());
    sum2.getIn(1).connect(tremor2.getOut());
    servo4.getIn().connect(sum2.getOut());

/////////////////////////////

    // servo3.getIn().connect(ppq1.getPosOut());
    // servo4.getIn().connect(tremor2.getOut());

    

    // Add blocks to timedomain
    // timedomain.addBlock(canReceive);
    // timedomain.addBlock(canSend);
    timedomain.addBlock(encoder1);    
    timedomain.addBlock(encoder2);    
    timedomain.addBlock(ppq1);    
    timedomain.addBlock(ppq2);    
    timedomain.addBlock(breath);    
    timedomain.addBlock(tremor1);
    timedomain.addBlock(tremor2);
    timedomain.addBlock(sum1);
    timedomain.addBlock(sum2);
    timedomain.addBlock(pos);
    timedomain.addBlock(servo3);
    timedomain.addBlock(servo4);

    // Add timedomain to executor
    eeros::Executor::instance().add(timedomain);
}