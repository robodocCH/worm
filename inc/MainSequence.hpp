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
                //cs.pos.setValue({500, 300});
                cs.pos.setValue(1.0);
                wait(3);
                //cs.pos.setValue({1000, 400});
                cs.pos.setValue(0.0);
                wait(3);
                //cs.pos.setValue({0, 0});
                cs.pos.setValue(-1.0);
                wait(3);

                log.warn() << "pos at beginning" << cs.ppq.getPosOut().getSignal();
                log.warn() << "move start ok:";
                // move("path01.txt", 3, 10, 20);
                cs.ppq.init("path01.txt");
                cs.ppq.reset();
                cs.ppq.move(3, 10, 20);
                log.warn() << "end reached:" << cs.ppq.endReached();
                log.warn() << "pos at end" << cs.ppq.getPosOut().getSignal();
                wait(3);
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