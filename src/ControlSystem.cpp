#include "ControlSystem.hpp"
#include <cmath>

ControlSystem::ControlSystem(double dt)

      : handle("can0"),
        canSend(handle.getSocket(), {node1, node2}),
        canReceive(handle.getSocket(), {node1, node2}),
        encoder1("enc1"),
        encoder2("enc2"),
        servo3("servo3"),
        servo4("servo4"),
        ppq1(dt),
        ppq2(dt),
        gain1(4),
        gain2(8),
        preSaturation1(-2.5, 2.5),
        preSaturation2(-2.5, 2.5),
        saturation1(-3.0, 3.0),
        saturation2(-3.0, 3.0),
        breath(2.513274123, -0.009, 0.0403, 0.01, 0.004, 0.0032),
        tremor1(12.72345025, -0.0015, 0.0041, 0.0013, 0.001, 0.0006),
//        tremor2(13.66592804, -0.0006, 0.0047, 0.0022, 0.0009, 0.0023),
        tremor2(1, 0.0, 0.5, 0.0, 0.0, 0.0),
// ----------------------------------------------------------------
        filter1(0.1),
        filter2(0.1),
        pos(),
        timedomain("Main time domain", dt, true)
{
    pos.setValue(0.0);
    ppq1.getPosOut().getSignal().setValue(0.0); // Startposition setzen
    ppq2.getPosOut().getSignal().setValue(0.0); // Startposition setzen
    breath.getOut().getSignal().setValue(0.0); // Startposition setzen
    tremor1.getOut().getSignal().setValue(0.0); // Startposition setzen
    tremor2.getOut().getSignal().setValue(0.0); // Startposition setzen

    canSend.setName("CAN send");
    canReceive.setName("CAN receive");
    canReceive.getOut().getSignal().setName("position");

    double scaleMotorSetAngle = 180/0.01/M_PI; // Scaling for motor setting angle 
    Matrix<2,1,double> scaleVWrite({scaleMotorSetAngle, scaleMotorSetAngle});    // accounts for encoder resolution and gear box
    canSend.setScale(scaleVWrite);

    double offsetMotorAngle1 = 0; // Offset of motor 1 setting angle 
    // double offsetMotorAngle2 = 2*M_PI; // Offset of motor 2 setting angle 
    double offsetMotorAngle2 = 0; // Offset of motor 2 setting angle 
    Matrix<2,1,double> offsetVWrite({offsetMotorAngle1, offsetMotorAngle2});    // accounts for encoder resolution and gear box
    canSend.setOffset(offsetVWrite);

    double scaleActualMotorAngle = M_PI/180;  // Scaling for actual motor angle  
    Matrix<2,1,double> scaleVRead({scaleActualMotorAngle, scaleActualMotorAngle});    // accounts for encoder resolution and gear box
    canReceive.setScale(scaleVRead);

    double offsetMotorAngleRead1 = 0; // Offset of motor 1 setting angle 
    // double offsetMotorAngleRead2 = -2*M_PI; // Offset of motor 2 setting angle 
    double offsetMotorAngleRead2 = 0; // Offset of motor 2 setting angle 
    Matrix<2,1,double> offsetVRead({offsetMotorAngleRead1, offsetMotorAngleRead2});    // accounts for encoder resolution and gear box
    canReceive.setOffset(offsetVRead);
 
    // Name all blocks
    pos.setName("SetpointBlock");
    encoder1.setName("Encoder 1");
    encoder2.setName("Encoder 2");
    servo3.setName("Servo 3");
    servo4.setName("Servo 4");

    breath.setName("Breathing Signal Source");

    ppq1.setName("Pathplanner Cubic 1");
    gain1.setName("Gain 1");
    preSaturation1.setName("Pre-Saturation 1");
    saturation1.setName("Saturation 1");
    tremor1.setName("Tremor1 Signal Source 1");
    sum1.setName("Summe 1");  

    ppq2.setName("Pathplanner Cubic 2");
    gain2.setName("Gain 2");
    preSaturation2.setName("Pre-Saturation 2");
    saturation2.setName("Saturation 2");
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

    sum1.getOut().getSignal().setName("Sum1 Pos Setpoint");
    sum2.getOut().getSignal().setName("Sum2 Pos Setpoint");

    // Verbinden der Blocks
    gain1.getIn().connect(ppq1.getPosOut());
    preSaturation1.getIn().connect(gain1.getOut());
    filter1.getIn().connect(preSaturation1.getOut());
    sum1.getIn(0).connect(filter1.getOut());
    sum1.getIn(1).connect(breath.getOut());
    sum1.getIn(2).connect(tremor1.getOut());
    saturation1.getIn().connect(sum1.getOut());
    servo3.getIn().connect(saturation1.getOut());

    gain2.getIn().connect(ppq2.getPosOut());
    preSaturation2.getIn().connect(gain2.getOut());
    filter2.getIn().connect(preSaturation2.getOut());
    sum2.getIn(0).connect(filter2.getOut());
    sum2.getIn(1).connect(tremor2.getOut());
    saturation2.getIn().connect(sum2.getOut());
    servo4.getIn().connect(saturation2.getOut());

    mux.getIn(0).connect(saturation1.getOut());
    mux.getIn(1).connect(saturation2.getOut());

    canSend.getIn().connect(mux.getOut());

    // Add blocks to timedomain
    timedomain.addBlock(pos);
    timedomain.addBlock(canReceive);
    timedomain.addBlock(encoder1);
    timedomain.addBlock(encoder2);

    timedomain.addBlock(ppq1);
    timedomain.addBlock(ppq2);

    timedomain.addBlock(gain1);
    timedomain.addBlock(gain2);    
    timedomain.addBlock(preSaturation1);
    timedomain.addBlock(preSaturation2);    
    timedomain.addBlock(filter1);
    timedomain.addBlock(filter2);

    timedomain.addBlock(breath);
    timedomain.addBlock(tremor1);
    timedomain.addBlock(tremor2);
    timedomain.addBlock(sum1);
    timedomain.addBlock(sum2);
    timedomain.addBlock(saturation1);
    timedomain.addBlock(saturation2);

    timedomain.addBlock(mux);
    timedomain.addBlock(canSend);
    timedomain.addBlock(servo3);
    timedomain.addBlock(servo4);

    // Add timedomain to executor
    eeros::Executor::instance().add(timedomain);
}