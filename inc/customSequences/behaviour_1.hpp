#ifndef CUSTOMSEQUENCETEMPLATE_HPP_
#define CUSTOMSEQUENCETEMPLATE_HPP_

#include <eeros/sequencer/Sequence.hpp>
#include "customSteps/Move.hpp"

class Behaviour_1 : public eeros::sequencer::Sequence
{
public:
    Behaviour_1 (std::string name, eeros::sequencer::Sequence *caller, ControlSystem &cs)
        :   eeros::sequencer::Sequence(name, caller, true),
            cs(cs),
            move("move", this, cs)
    {
        log.info() << "Sequence created: " << name;
    }

    int action()
    {
        // do something
        
        cs.tremor1.enable();
        move(1.0, "path01.txt", 0.0, 0.8, "path01.txt", 0.0, 0.8);
        move(1.0, "path01.txt", 0.8, 1.0, "path01.txt", 0.8, 1.0);
        move(1.0, "path01.txt", 1.0, 0.0, "path01.txt", 1.0, 0.0);
        move(1.5, "path01.txt", 0.0, -1.0, "path01.txt", 0.0, -1.0);
        move(2.0, "path01.txt", -1.0, -0.7, "path01.txt", -1.0, -0.7);
        move(1.0, "path01.txt", -0.7, 1.5, "path01.txt", -0.7, 0.8);
        move(1.0, "path01.txt", 1.5, 0.0, "path01.txt", 0.8, 0.0);

        return 0;
    }

private:
    // Define steps, sub-sequences, variables, conditions, monitors, exception sequences, ...
    ControlSystem & cs;
    Move move;
};

#endif // CUSTOMSEQUENCETEMPLATE_HPP_
