#ifndef TESTMOTIONS_HPP_
#define TESTMOTIONS_HPP_

#include <eeros/sequencer/Sequence.hpp>
#include "customSteps/Move.hpp"
#include <eeros/sequencer/Wait.hpp>

class Testmotions : public eeros::sequencer::Sequence
{
public:
    Testmotions (std::string name, eeros::sequencer::Sequence *caller, ControlSystem &cs)
        :   eeros::sequencer::Sequence(name, caller, true),
            cs(cs),
            move("move", this, cs),
            wait("wait", this)
    {
        log.info() << "Sequence created: " << name;
    }

    int action()
    {
        move(2.0, "path01.txt", 0.0, 0.3, "path01.txt", 0.0, 1.5);
        wait(1);
        move(3.0, "path01.txt", 0.3, 0.9, "path01.txt", 1.5, -1.0);
        move(3.0, "path01.txt", 0.9, -0.3, "path01.txt", -1.0, 0.5);
        wait(3);
        move(3.0, "path01.txt", -0.3, 0.0, "path01.txt", 0.5, 0.0);
        return 0;
    }

private:
    // Define steps, sub-sequences, variables, conditions, monitors, exception sequences, ...
    ControlSystem & cs;
    Move move;
    eeros::sequencer::Wait wait;
};

#endif // TESTMOTIONS_HPP_
