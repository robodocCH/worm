#ifndef BEHAVIOUR_1_HPP_
#define BEHAVIOUR_1_HPP_

#include <eeros/sequencer/Sequence.hpp>
#include "customSteps/Move.hpp"
#include <eeros/sequencer/Wait.hpp>

class Behaviour_1 : public eeros::sequencer::Sequence
{
public:
    Behaviour_1 (std::string name, eeros::sequencer::Sequence *caller, ControlSystem &cs)
        :   eeros::sequencer::Sequence(name, caller, true),
            cs(cs),
            move("move", this, cs),
            wait("wait", this)
    {
        log.info() << "Sequence created: " << name;
    }

    int action()
    {
        // do something
        
        // cs.tremor1.enable();
        // Hier könnte man Aenderungen am Tremor oder der Atmung vornehmen (ist in FourierSignalSource noch nicht implementiert)
        move(0.7, "basePath.txt", 0.0, 0.8, "basePath.txt", 0.0, 1.8);
        move(0.5, "basePath.txt", 0.8, 1.0, "basePath.txt", 0.8, 1.0);
        move(0.9, "path01.txt", 1.0, 0.0, "path01.txt", 1.0, 0.0);
        move(1.2, "path01.txt", 0.0, -1.0, "path01.txt", 0.0, -1.0);
        move(1.0, "path01.txt", -1.0, -0.9, "path01.txt", -1.0, -0.7);
        move(0.8, "path01.txt", -0.9, 1.5, "path01.txt", -0.7, 0.8);
        move(0.6, "path01.txt", 1.5, 0.0, "path01.txt", 0.8, 0.0);
        return 0;
    }

private:
    // Define steps, sub-sequences, variables, conditions, monitors, exception sequences, ...
    ControlSystem & cs;
    Move move;
    eeros::sequencer::Wait wait;
};

#endif // BEHAVIOUR_1_HPP_
