#ifndef MAINSEQUENCE_HPP_
#define MAINSEQUENCE_HPP_

#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include "MyRobotSafetyProperties.hpp"
#include "ControlSystem.hpp"
#include <eeros/sequencer/Wait.hpp>
#include "customSteps/Move.hpp"

class MainSequence : public eeros::sequencer::Sequence
{
public:
    MainSequence(std::string name, eeros::sequencer::Sequencer &seq,
                 eeros::safety::SafetySystem &ss,
                 MyRobotSafetyProperties &sp, ControlSystem &cs)
        : eeros::sequencer::Sequence(name, seq),
          ss(ss),
          sp(sp),
          cs(cs),
          move("move", this, cs),
          wait("Wait", this)
    {
        log.info() << "Sequence created: " << name;
    }

    int action() {
        // Vector2 speed{5000, 5000};
        // cs.canSend.setSpeed(speed);
        // cs.canSend.enable();
        // cs.canReceive.enable();
        while(eeros::sequencer::Sequencer::running) {
            if (ss.getCurrentLevel() == sp.slSystemOn) {
                // mit CAN
                // cs.pos.setValue({500, 300});
                // cs.pos.setValue({1000, 400});               

                // wait(1);
                // cs.pos.setValue(0.0);
                // wait(1);
                //cs.pos.setValue({0, 0});
                // wait(1);                
                // cs.pos.setValue(-1.0);

                move(5.0, "path01.txt", 0, 0.3, "path01.txt", 0, 1.0);
                move(3.0, "path01.txt", 0.3, -0.5, "path01.txt", 1.0, -1.5);
                move(1.5, "path01.txt", -0.2, -0.1, "path01.txt", -0.5, 1.5);
                move(0.5, "path01.txt", -0.3, 0.5, "path01.txt", 1, 0.5);
                move(2.0, "path01.txt", 0.2, 0.6, "path01.txt", 1.5, -2.0);
                move(0.5, "path01.txt", 0.8, -0.8, "path01.txt", -0.5, 1.0);
                move(1.0, "path01.txt", 0.0, 0.2, "path01.txt", 0.5, -1.5);
                move(1.5, "path01.txt", 0.2, 0.8, "path01.txt", -1, 2);
                move(0.5, "path01.txt", 1, 0.5, "path01.txt", 1, 0.5);
                move(0.2, "path01.txt", 1.5, -2.0, "path01.txt", 1.5, -2.0);                
                move(0.5, "path01.txt", -0.5, 0.5, "path01.txt", -0.5, 1.0);
                move(0.5, "path01.txt", 0.0, 0.2, "path01.txt", 0.5, -1.5);
                move(0.1, "path01.txt", 0.2, 0.8, "path01.txt", -1, 2);
                move(0.5, "path01.txt", 1, 0.5, "path01.txt", 1, 0.5);
                move(0.7, "path01.txt", 1.5, -1.5, "path01.txt", 1.5, -1.5);
                // log.warn() << "pos at end" << cs.ppq1.getPosOut().getSignal();
            }
        }
        return 0;
    }

private:
    eeros::safety::SafetySystem &ss;
    ControlSystem &cs;
    MyRobotSafetyProperties &sp;
    Move move;
    eeros::sequencer::Wait wait;
};

#endif // MAINSEQUENCE_HPP_