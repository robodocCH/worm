#ifndef MAINSEQUENCE_HPP_
#define MAINSEQUENCE_HPP_

#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/Sequence.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include "MyRobotSafetyProperties.hpp"
#include "ControlSystem.hpp"
#include <eeros/sequencer/Wait.hpp>
#include "customSteps/Move.hpp"
#include "customSequences/behaviour_1.hpp"
#include "customSequences/Testmotions.hpp"
#include "customSequences/TestmotionsFast.hpp"
#include "customSequences/Flaps.hpp"


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
          behave1("behave1", this, cs),          
          testmotions("testmotions", this, cs),
          testmotionsFast("testmotionsFast", this, cs),
          flaps("flaps", this, cs),
          wait("Wait", this)
    {
        log.info() << "Sequence created: " << name;
    }

    int action() {

        // *********************************************************************************      
        // cs.canSend.disable();
        // cs.canReceive.disable();  
        cs.canSend.enable();
        cs.canReceive.enable();  
        // *********************************************************************************    

        // Geschwindigkeit für Aufstarten       
        // *********************************************************************************      
        Vector2 speed{1000, 1000};
        // Vector2 speed{30000, 30000};
        // *********************************************************************************      
        cs.canSend.setSpeed(speed);
        // Aufstarten, auf 0 fahren

        // Funktioniert nicht!! gefährlich!
        double pos1Actual = cs.ppq1.getPosOut().getSignal().getValue();
        double pos2Actual = cs.ppq2.getPosOut().getSignal().getValue();
        log.warn() << "position 1 = " << pos1Actual;
        log.warn() << "position 2 = " << pos2Actual;
        wait(5);
        log.warn() << "Start homing in 5 s";
        wait(5);
        log.warn() << "Start homing";
        move(5.0, "basePath.txt", 0.1, 0.0, "basePath.txt", 0.1, 0.0);
        log.warn() << "End homing";
        // Ab hier Geschwindigket freigeben
        // Achtung Gefahr: Höhere Werte führen zu Fehler, die Geschwindigkeit ist noch nicht skalliert und das Problem ist noch nicht abgefangen.
        //----------------------------------------------------
        speed[0] = 30000; // 30000
        speed[1] = 30000; // 30000
        // --------------------------------------------------------------------------------------------------------------------------------------------------
        // cs.canSend.setSpeed(speed);
        while(eeros::sequencer::Sequencer::running) {
            if (ss.getCurrentLevel() == sp.slSystemOn) {
                        log.warn() << "testmotions started";
                testmotions();           
                        log.warn() << "testmotions ended";
                wait(2);
                        log.warn() << "behave1 started";
                behave1();
                        log.warn() << "behave1 ended";
                        log.warn() << "testmotionsFast started";
                testmotionsFast();
                        log.warn() << "testmotionsFast ended";
                wait(2);
                       log.warn() << "testmotionsFast started";
                flaps();                    
                        log.warn() << "testmotionsFast ended";
                        log.warn() << "testmotionsFast started";
                testmotionsFast();
                        log.warn() << "testmotionsFast ended";
                wait(2);
            }
        }
        return 0;
    }
private:
    eeros::safety::SafetySystem &ss;
    ControlSystem &cs;
    MyRobotSafetyProperties &sp;
    Move move;
    Behaviour_1 behave1;
    Testmotions testmotions;
    TestmotionsFast testmotionsFast;
    Flaps flaps;
    eeros::sequencer::Wait wait;
};

#endif // MAINSEQUENCE_HPP_