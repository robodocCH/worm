#ifndef CUSTOMSTEPTEMPLATE_HPP_
#define CUSTOMSTEPTEMPLATE_HPP_

#include <eeros/sequencer/Step.hpp>

class Tremor : public eeros::sequencer::Step
{
public:
    Move(std::string name, eeros::sequencer::Sequence *caller, ControlSystem &cs)
        : eeros::sequencer::Step(name, caller), cs(cs)
    {
        log.info() << "Step created: " << name;
    }

    int operator() (std::string filename, double time, double startPos, double deltaPos) {
        this->filename = filename;    
        this->time = time;
        this->startPos = startPos;
        this->deltaPos = deltaPos;
        return start();  // this will start the step or sequence
    }

    int action()
    {
        cs.ppq.init(filename);
        cs.ppq.move(time, startPos, deltaPos);
        return 0;
    }

    bool checkExitCondition() {
        return cs.ppq.endReached();
    }

private:
    std::string filename; 
    double time; 
    double startPos;	
    double deltaPos;
    ControlSystem &cs;
};

#endif // CUSTOMSTEPTEMPLATE_HPP_
