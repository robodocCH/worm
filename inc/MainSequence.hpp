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
        Vector2 speed{3000, 3000};
        // Geschwindigkeit für Aufstarten
        cs.canSend.setSpeed(speed);
        // --------------------------------------------------------------------------------------------------------------------------------------
        cs.canSend.enable();
        cs.canReceive.enable();
        while(eeros::sequencer::Sequencer::running) {
            if (ss.getCurrentLevel() == sp.slSystemOn) {
                // Aufstarten, auf 0 fahren
                move(30.0, "path01.txt", 0.1, 0.0, "path01.txt", 0.1, 0.0);
                // Ab hier Geschwindigket freigeben
                Vector2 speed{30000, 30000};
                // Achtung Gefahr: Höhere Werte führen zu Fehler, die Geschwindigkeit ist noch nicht skalliert und das Problem ist noch nicht abgefabgen.
                cs.canSend.setSpeed(speed);
                move(1.0, "path01.txt", 0.0, 0.8, "path01.txt", 0.0, 0.8);
                move(1.0, "path01.txt", 0.8, 1.0, "path01.txt", 0.8, 1.0);
                move(1.0, "path01.txt", 1.0, 0.0, "path01.txt", 1.0, 0.0);
                move(1.5, "path01.txt", 0.0, -1.0, "path01.txt", 0.0, -1.0);
                move(2.0, "path01.txt", -1.0, -0.7, "path01.txt", -1.0, -0.7);
                move(1.0, "path01.txt", -0.7, 0.8, "path01.txt", -0.7, 0.8);
                move(1.0, "path01.txt", 0.8, 0.0, "path01.txt", 0.8, 0.0);
                wait(2);
                log.warn() << "wormTest1Xppq.txt started";
                move(30.0, "wormTest1Xppq.txt", 0, 0.01, "wormTest1Xppq.txt", 0, 0.01);
                log.warn() << "wormTest1Xppq.txt ended";
                wait(2);
                move(5.0, "path01.txt", 0.01, 0.0, "path01.txt", 0.01, 0.0);
                wait(2);
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