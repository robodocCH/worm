#include <signal.h>
#include <eeros/logger/StreamLogWriter.hpp>
#include <eeros/core/Executor.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/hal/HAL.hpp>
#include "ControlSystem.hpp"
#include "MyRobotSafetyProperties.hpp"
#include "MainSequence.hpp"
#include <eeros/task/Lambda.hpp>
#include <eeros/task/Periodic.hpp>


void signalHandler(int signum)
{
    eeros::safety::SafetySystem::exitHandler();
    eeros::sequencer::Sequencer::instance().abort();
}

int main(int argc, char **argv)
{
    const double dt = 0.005;
    eeros::logger::Logger::setDefaultStreamLogger(std::cout);
    eeros::logger::Logger log = eeros::logger::Logger::getLogger();

    log.info() << "Starting template project...";

    log.info() << "Initializing hardware...";
    eeros::hal::HAL& hal = eeros::hal::HAL::instance();
    hal.readConfigFromFile(&argc, argv);

    log.info() << "Initializing control system...";
    ControlSystem cs(dt);

    log.info() << "Initializing safety system...";
    MyRobotSafetyProperties sp(cs, dt);
    eeros::safety::SafetySystem ss(sp, dt);
    cs.timedomain.registerSafetyEvent(ss, sp.seDoExit); // fired if timedomain fails to run properly
    signal(SIGINT, signalHandler);

    // create periodic function for logging
    eeros::task::Lambda l1 ([&] () { });
    eeros::task::Periodic p2("p2", 0.1, l1); // t=0.02 (normaly 0.1)
    p2.monitors.push_back([&](eeros::PeriodicCounter &pc, Logger &log) {
        // log.info() << cs.ppq1.getJerkOut().getSignal().getTimestamp() << " "<< cs.ppq1.getJerkOut().getSignal().getValue() << " "<< cs.ppq1.getAccOut().getSignal().getValue() << " "<< cs.ppq1.getVelOut().getSignal().getValue() << " "<< cs.ppq1.getPosOut().getSignal().getValue();
        // log.info() << cs.tremor.getOut().getSignal();// 
        // log.info() << cs.ppq1.getPosOut().getSignal();        
        log.info() << cs.sum1.getOut().getSignal() << "     " << cs.canReceive.getOut().getSignal().getValue();
        cs.canSend().
    });

    log.info() << "Initializing sequencer...";
    auto &sequencer = eeros::sequencer::Sequencer::instance();
    MainSequence mainSequence("Main Sequence", sequencer, ss, sp, cs);
    mainSequence();

    log.info() << "Initializing executor...";
    auto &executor = eeros::Executor::instance();
    executor.setMainTask(ss);
    executor.add(p2);
    executor.run();

    sequencer.wait();

    log.info() << "Worm software has been terminated";

    return 0;
}