#ifndef MOVE_HPP_
#define MOVE_HPP_

#include <eeros/sequencer/Step.hpp>

class Move : public eeros::sequencer::Step
{
public:
    Move(std::string name, eeros::sequencer::Sequence *caller, ControlSystem &cs)
        : eeros::sequencer::Step(name, caller), cs(cs)
    {
        log.info() << "Step created: " << name;
    }

    int operator() (double time, std::string filename1, double startPos1, double endPos1, std::string filename2, double startPos2, double endPos2) {
        this->time = time;
        this->filename1 = filename1;    
        this->startPos1 = startPos1;
        this->deltaPos1 = endPos1 - startPos1;
        this->filename2 = filename2;    
        this->startPos2 = startPos2;
        this->deltaPos2 = endPos1 - startPos1;
        return start();  // this will start the step or sequence
    }

    int action()
    {
        cs.ppq1.init(filename1);
        cs.ppq1.move(time, startPos1, deltaPos1);
        cs.ppq2.init(filename1);
        cs.ppq2.move(time, startPos1, deltaPos1);
        return 0;
    }

    bool checkExitCondition() {
        return cs.ppq1.endReached() & cs.ppq2.endReached();
    }

private:
    double time; 
    std::string filename1; 
    double startPos1;	
    double deltaPos1;
    std::string filename2; 
    double startPos2;	
    double deltaPos2;
    ControlSystem &cs;
};

#endif // MOVE_HPP_
