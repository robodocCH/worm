#ifndef FLAPS_HPP_
#define FLAPS_HPP_

#include <eeros/sequencer/Sequence.hpp>
#include "customSteps/Move.hpp"
#include <eeros/sequencer/Wait.hpp>

class Flaps : public eeros::sequencer::Sequence
{
public:
    Flaps (std::string name, eeros::sequencer::Sequence *caller, ControlSystem &cs)
        :   eeros::sequencer::Sequence(name, caller, true),
            cs(cs),
            move("move", this, cs),
            wait("wait", this)
    {
        log.info() << "Sequence created: " << name;
    }

    int action()
    {
        log.warn() << "aufschwingen started";
        move(1.0, "aufschwingen.txt", 0.0, 0.3, "aufschwingen.txt", 0.0, 1.5);
        log.warn() << "aufschwingen ended";
        log.warn() << "daempendschwingen started";
        move(1.0, "daempendschwingen.txt", 0.3, 0.0, "daempendschwingen.txt", 1.5, 0.0);
        log.warn() << "daempendschwingen ended";
        wait(1);
        return 0;
    }

private:
    // Define steps, sub-sequences, variables, conditions, monitors, exception sequences, ...
    ControlSystem & cs;
    Move move;
    eeros::sequencer::Wait wait;
};

#endif // FLAPS_HPP_
