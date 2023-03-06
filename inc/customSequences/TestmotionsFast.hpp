#ifndef TESTMOTIONSFAST_HPP_
#define TESTMOTIONSFAST_HPP_

#include <eeros/sequencer/Sequence.hpp>
#include "customSteps/Move.hpp"
#include <eeros/sequencer/Wait.hpp>

class TestmotionsFast : public eeros::sequencer::Sequence
{
public:
    TestmotionsFast (std::string name, eeros::sequencer::Sequence *caller, ControlSystem &cs)
        :   eeros::sequencer::Sequence(name, caller, true),
            cs(cs),
            move("move", this, cs),
            wait("wait", this)
    {
        log.info() << "Sequence created: " << name;
    }

    int action()
    {
        move(0.5, "path01.txt", 0.0, 0.1, "path01.txt", 0.0, 2.0);
        wait(0.3);
        move(0.7, "path01.txt", 0.1, 0.3, "path01.txt", 1.5, -1.0);
        move(2.0, "path01.txt", 0.3, -0.3, "path01.txt", -1.0, -2);
        wait(1);
        move(1, "path01.txt", -0.3, -0.6, "path01.txt", -2, -1);

        move(1, "path01.txt", -0.6, -1.0, "path01.txt", -1, -1.5);
        move(3, "path01.txt", -1.0, 0.0, "path01.txt", -1.5, 0.0);
        return 0;
    }

private:
    // Define steps, sub-sequences, variables, conditions, monitors, exception sequences, ...
    ControlSystem & cs;
    Move move;
    eeros::sequencer::Wait wait;
};

#endif // TESTMOTIONSFAST_HPP_
