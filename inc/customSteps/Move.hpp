#ifndef MOVE_HPP_
#define MOVE_HPP_

#include <eeros/sequencer/Step.hpp>
#include <cmath>
#include <eeros/core/Fault.hpp>


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
        double pos1Actual = cs.ppq1.getPosOut().getSignal().getValue();
        double pos2Actual = cs.ppq2.getPosOut().getSignal().getValue();
        double limit = 0.05;            
        startPos1 = pos1Actual;
        startPos2 = pos2Actual;
        if (std::abs(((startPos1-pos1Actual) > limit) || ((startPos2-pos2Actual) > limit))){
            throw eeros::Fault("Robot is not at the desired starting position: Position step too big");
            //
        };
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
