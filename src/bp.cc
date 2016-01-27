#include "bp.hh"

#include <stdlib.h>
#include <iostream>
bool BP_Local::operator ==(const BP_Local& other) const {
    return (PC == other.PC && (*(std::vector<bool>*) this) == other);
}

bool BP_Local::operator <(const BP_Local& other) const {
    return (PC < other.PC
            || (PC == other.PC && (*(std::vector<bool>*) this) < other));
}

std::_Bit_reference BP_Local::operator[](unsigned pos) {
    return (*(std::vector<bool>*) this)[pos];
}

void BP_Local::out(std::ostream& os) const {
    os << '(';
    if (size() > 0) {
        vector<bool>::const_iterator it = begin();
        os << ((*it++) ? 'T' : 'F');
        for (; it != end(); it++)
            os << ((*it) ? "T" : "F");
    }
    os << ',' << PC << ')';
}

bool BP_Shared::operator ==(const BP_Shared& other) const {
    return ((*(std::vector<bool>*) this) == other);
}

std::_Bit_reference BP_Shared::operator[](unsigned pos) {
    return (*(std::vector<bool>*) this)[pos];
}

void BP_Shared::out(std::ostream& os) const {
    os << '(';
    if (size() > 0) {
        vector<bool>::const_iterator it = begin();
        os << ((*it++) ? 'T' : 'F');
        for (; it != end(); it++)
            os << ((*it) ? "T" : "F");
    }
    os << ')';
}

BPInterpretator::BPInterpretator(ast::program& _program,
        ast::constant::trival _non_determinitic) {
    //std::cout << _program << std::endl;
    successors = 0;
    non_determinitic = _non_determinitic;
    if (_program.getLocalVars() != 0) {
        localVars = (ast::assignment*) _program.getLocalVars()->clone();

        //'locate the local variables into bits'
        int index = 1;
        std::list<ast::identifier*>::const_iterator it =
                ((ast::identifiers*) localVars->getVars())->begin();
        for (; it != ((ast::identifiers*) localVars->getVars())->end(); it++)
            varsMap[(*it)->get_name()] = index++;
    } else
        localVars = 0;

    if (_program.getSharedVars() != 0) {
        sharedVars = (ast::assignment*) _program.getSharedVars()->clone();

        //'locate the shared variables into bits'
        int index = 1;
        std::list<ast::identifier*>::const_iterator it =
                ((ast::identifiers*) sharedVars->getVars())->begin();
        for (; it != ((ast::identifiers*) sharedVars->getVars())->end(); it++)
            varsMap[(*it)->get_name()] = 0 - index++;
    } else
        sharedVars = 0;

    stack<ast::if_begin*> if_begins;

    //locate the labeled statement and parse the if fi pair;
    unsigned pc = 0;
    for (std::list<ast::statement*>::const_iterator it =
            _program.getMain()->begin(); it != _program.getMain()->end();
            it++) {
        for (list<ast::node*>::const_iterator label = (*it)->labels.begin();
                label != (*it)->labels.end(); label++)
            labelsMap[((ast::identifier*) (*label))->get_name()] = pc;
        statements.push_back((ast::statement*) (*it)->clone());

        if ((*it)->code->getType() == ast::IF_BEGIN) {
            if_begins.push((ast::if_begin*) statements.back()->code);
        }

        if ((*it)->code->getType() == ast::IF_END) {
            if_begins.top()->JumpTo(pc + 1);
            if_begins.pop();
        }
        pc++;
    }

    if (statements.back()->code->getType() != ast::JUMP)
        statements.pop_back(); // ignore the last statement except if it is goto

    if (if_begins.size() != 0) {
        std::cout << "mismatch number of if and fi statement" << std::endl;
        exit(0);
    }
}

BPInterpretator::~BPInterpretator() {
    if (successors != 0)
        delete successors;

    if (localVars != 0)
        delete localVars;

    if (sharedVars != 0)
        delete sharedVars;

    while (results.size() > 0) {
        delete results.top();
        results.pop();
    };

    while (statements.size() > 0) {
        delete statements.back();
        statements.pop_back();
    };

    while (statePtrCollection.size() > 0) {
        delete statePtrCollection.back();
        statePtrCollection.pop_back();
    };

}

std::list<BP_Config*>& BPInterpretator::getInitialConfigs(unsigned numThread) {
    srand(time(0)); //intial the seed
    std::list<BP_Config*>* result = new std::list<BP_Config*>();
    BP_Config* config = new BP_Config();

    unsigned size = 0;
    if (sharedVars != 0)
        size = sharedVars->getVars()->size();
    BP_Shared shared(size);

    size = 0;
    if (localVars != 0)
        size = localVars->getVars()->size();
    BP_Local local(size);

    local.PC = 0;
    vass::Counter counter(numThread);

    if (numThread == 0)
        counter.setToInfinite();

    std::list<ast::identifier*>::const_iterator itVar;
    std::list<ast::node*>::const_iterator itValue;

    //read local variables
    if (localVars != 0) {
        ast::identifiers* vars = ((ast::identifiers*) localVars->getVars());
        ast::expressions* exprs = ((ast::expressions*) localVars->getExprs());
        itVar = vars->begin();
        itValue = exprs->begin();
        for (; itVar != vars->end() && itValue != exprs->end();)
            if (((ast::constant*) (*itValue))->get_value()
                    == ast::constant::STAR) {
                bool value;
                if (non_determinitic == ast::constant::STAR)
                    value = (bool) (rand() % 2);
                else
                    value = (bool) (non_determinitic);

                local[varsMap[(*(itVar++))->get_name()] - 1] = value; //(bool) (rand() % 2);
                itValue++;
            } else
                local[varsMap[(*(itVar++))->get_name()] - 1] =
                        (bool) ((ast::constant*) (*itValue++))->get_value();
    }

    if (sharedVars != 0) {
        //read shared variables
        ast::identifiers* vars = ((ast::identifiers*) sharedVars->getVars());
        ast::expressions* exprs = ((ast::expressions*) sharedVars->getExprs());
        itVar = vars->begin();
        itValue = exprs->begin();
        for (; itVar != vars->end() && itValue != exprs->end();)
            if (((ast::constant*) (*itValue))->get_value()
                    == ast::constant::STAR) {
                bool value;
                if (non_determinitic == ast::constant::STAR)
                    value = (bool) (rand() % 2);
                else
                    value = (bool) (non_determinitic);

                shared[0 - varsMap[(*(itVar++))->get_name()] - 1] = value; //(bool) (rand() % 2);
                itValue++;
            } else
                shared[0 - varsMap[(*(itVar++))->get_name()] - 1] =
                        (bool) ((ast::constant*) (*itValue++))->get_value();
    }

    config->state = shared;
    statePtrCollection.push_back(new BP_Local(local));
    config->counters[statePtrCollection.back()] = counter;
    result->push_back(config);
    return *result;
}

std::list<BP_Config*>& BPInterpretator::getSuccessors(
        const BP_Config* currentConfig) {
    successors = new std::list<BP_Config*>();

    if (currentConfig == 0)
        return *successors;

    //currentConfig->out(std::cout);

    bool invalidedStateFound = false;

    //currentState.shared = currentConfig->state;
    for (vass::StatePtrMapCounter<BP_Local>::const_iterator it =
            currentConfig->counters.begin();
            it != currentConfig->counters.end() && !invalidedStateFound; it++) {

        currentState.shared = currentConfig->state;
        currentState.local = *it->first;

        if (currentState.local.PC < statements.size()) {

//			currentConfig->out(std::cout << " ::::::: ");
            statements[currentState.local.PC]->accept(*this);
//			std::cout << std::endl << "PC=" << currentState.local.PC << ":" 
//						<< *statements[currentState.local.PC] << endl;

            for (unsigned i = 0;
                    i < resultingStates.size() && !invalidedStateFound; i++) {
                BP_Config* newConfig = currentConfig->clone();

                vass::StatePtrMapCounter<BP_Local>::iterator source =
                        newConfig->counters.find(it->first);

                //decrease the counter of source local state
                source->second.decrease();
                if (source->second.isZero())
                    newConfig->counters.erase(source);

                //update the valuation of shared state
                newConfig->state = resultingStates[i].front().shared;

                //and the counter for resulting local state
                for (unsigned j = 0; j < resultingStates[i].size(); j++) {
                    //vass::StateCounter<BP_Local> stateCounter(resultingStates[i][j].local);
                    //newConfig->counters.insert(stateCounter).first->counter.increase();

                    statePtrCollection.push_back(
                            new BP_Local(resultingStates[i][j].local));
                    newConfig->counters[statePtrCollection.back()].increase();

                    //terminate when invalided state reached
                    if (resultingStates[i][j].invalided) {
                        newConfig->invalided = true;
                        invalidedStateFound = true;
                    }
                }

                //optimization on the searching strategy
                if (newConfig->state == currentConfig->state
                        || invalidedStateFound)
                    successors->push_front(newConfig);
                else
                    successors->push_back(newConfig);
            }
        }
    };
    /*
     std::cout << std::endl << "=========" << std::endl;
     std::cin.get();
     */
    return *successors;
}

void BPInterpretator::visit(ast::identifier* _identifier) {
    int id = varsMap[_identifier->get_name()];
    if (id > 0)
        results.push(
                new ast::constant(
                        (ast::constant::trival) ((bool) currentState.local[id
                                - 1])));
    if (id < 0)
        results.push(
                new ast::constant(
                        (ast::constant::trival) ((bool) currentState.shared[0
                                - id - 1])));
}

void BPInterpretator::visit(ast::constant* _constant) {
    if (_constant->get_value() == ast::constant::STAR) {
        delete _constant;
        if (non_determinitic == ast::constant::STAR)
            results.push(
                    new ast::constant((ast::constant::trival) (rand() % 2)));
        else
            results.push(new ast::constant(non_determinitic));
    } else
        results.push(_constant);
}

void BPInterpretator::visit(ast::neg* _neg) {
    ast::constant* operand = pop();
    results.push(new ast::constant((!(*operand))));
}

void BPInterpretator::visit(ast::conj* _conj) {
    ast::constant* left = pop();
    ast::constant* right = pop();
    if (left->get_value() == ast::constant::TRUE
            && right->get_value() == ast::constant::TRUE) {
        results.push(new ast::constant(ast::constant::TRUE));
    } else {
        if (left->get_value() == ast::constant::FALSE
                || right->get_value() == ast::constant::FALSE) {
            results.push(new ast::constant(ast::constant::FALSE));
        } else {
            results.push(new ast::constant(ast::constant::STAR));
        }
    }
}

void BPInterpretator::visit(ast::disj* _disj) {
    ast::constant* left = pop();
    ast::constant* right = pop();
    if (left->get_value() == ast::constant::FALSE
            && right->get_value() == ast::constant::FALSE) {
        results.push(new ast::constant(ast::constant::FALSE));
    } else {
        if (left->get_value() == ast::constant::TRUE
                || right->get_value() == ast::constant::TRUE) {
            results.push(new ast::constant(ast::constant::TRUE));
        } else {
            results.push(new ast::constant(ast::constant::STAR));
        }
    }
}

void BPInterpretator::visit(ast::skip* _skip) {
    resultingStates.clear();
    std::vector<vass::ThreadState<BP_Local, BP_Shared> > states;

    states.push_back(currentState);
    states.back().local.PC++;

    resultingStates.push_back(states);
}

void BPInterpretator::visit(ast::if_begin* _if_begin) {
    resultingStates.clear();
    std::vector<vass::ThreadState<BP_Local, BP_Shared> > states;

    ast::constant* condition = pop();
    if (condition->get_value() == ast::constant::TRUE
            || condition->get_value() == ast::constant::STAR) {
        states.push_back(currentState);
        states.back().local.PC++;
    }

    if (condition->get_value() == ast::constant::FALSE
            || condition->get_value() == ast::constant::STAR) {
        states.push_back(currentState);
        states.back().local.PC = _if_begin->getJumpTo();
    }

    resultingStates.push_back(states);
}

void BPInterpretator::visit(ast::bp_assert* _assert) {
    resultingStates.clear();
    std::vector<vass::ThreadState<BP_Local, BP_Shared> > states;

    ast::constant* condition = pop();

    states.push_back(currentState);
    states.back().local.PC++;

    if (condition->get_value() == ast::constant::FALSE)
        states.back().invalided = true;

    resultingStates.push_back(states);
}

void BPInterpretator::visit(ast::bp_assume* _assume) {
    resultingStates.clear();
    std::vector<vass::ThreadState<BP_Local, BP_Shared> > states;

    ast::constant* condition = pop();

    if (condition->get_value() == ast::constant::TRUE) {
        states.push_back(currentState);
        states.back().local.PC++;
    }

    if (states.size() > 0)
        resultingStates.push_back(states);
}

void BPInterpretator::visit(ast::if_end* _if_end) {
    resultingStates.clear();
    std::vector<vass::ThreadState<BP_Local, BP_Shared> > states;

    states.push_back(currentState);
    states.back().local.PC++;

    resultingStates.push_back(states);
}

void BPInterpretator::visit(ast::jump* _jump) {
    resultingStates.clear();

    std::vector<vass::ThreadState<BP_Local, BP_Shared> > states;

    for (list<ast::identifier*>::const_iterator it = _jump->begin();
            it != _jump->end(); it++) {
        states.clear();
        states.push_back(currentState);
        states.back().local.PC = labelsMap[(*it)->get_name()];
        resultingStates.push_back(states);
    }
}

void BPInterpretator::visit(ast::start_thread* _start_thread) {
    resultingStates.clear();
    std::vector<vass::ThreadState<BP_Local, BP_Shared> > states;
    for (list<ast::identifier*>::const_iterator it = _start_thread->begin();
            it != _start_thread->end(); it++) {
        states.push_back(currentState);
        states.back().local.PC++;
        states.push_back(currentState);
        states.back().local.PC = labelsMap[(*it)->get_name()];
    }

    resultingStates.push_back(states);
}

void BPInterpretator::visit(ast::end_thread* _end_thread) {
    resultingStates.clear();
}

void BPInterpretator::visit(ast::bp_return* _bp_return) {
    resultingStates.clear();
}

void BPInterpretator::visit(ast::atomic_begin* _atomic_begin) {
    std::vector<std::vector<vass::ThreadState<BP_Local, BP_Shared> > > results;
    std::vector<vass::ThreadState<BP_Local, BP_Shared> > states;

    stack<vass::ThreadState<BP_Local, BP_Shared> > stateQueue;
    currentState.local.PC++;
    stateQueue.push(currentState);

    bool invalidedStateFound = false;

    while (stateQueue.size() > 0 && !invalidedStateFound) {
        resultingStates.clear();
        currentState = stateQueue.top();
        stateQueue.pop();
        if (statements[currentState.local.PC]->code->getType()
                != ast::ATOMIC_END) {
            statements[currentState.local.PC]->accept(*this);
            for (unsigned i = 0; i < resultingStates.size(); i++) {
                if (resultingStates[i][0].invalided) {
                    invalidedStateFound = true;
                    states.clear();
                    states.push_back(resultingStates[i][0]);
                    results.push_back(states);
                    break;
                }

                stateQueue.push(resultingStates[i][0]);
            }
        } else {
            states.clear();
            states.push_back(currentState);
            results.push_back(states);
        }
    }

    resultingStates = results;

}

void BPInterpretator::visit(ast::atomic_end* _atomic_end) {
    resultingStates.clear();
    std::vector<vass::ThreadState<BP_Local, BP_Shared> > states;

    states.push_back(currentState);
    states.back().local.PC++;

    resultingStates.push_back(states);
}

void BPInterpretator::visit(ast::assignment* _assignment) {
    list<ast::identifier*>::const_iterator itVar =
            ((ast::identifiers*) _assignment->get_left())->begin();
    list<ast::node*>::const_iterator itValue =
            ((ast::expressions*) _assignment->get_right())->begin();
    int id = 0;
    resultingStates.clear();

    std::vector<vass::ThreadState<BP_Local, BP_Shared> > states;

    bool constraint = true;

    //should be modified to accept constraint in ast.cpp's assignment::accept
    //check if the constraint is satisfied
    if (_assignment->getConstraint() != 0) {
        _assignment->getConstraint()->accept(*this);
        ast::constant* result = pop();
        constraint = (bool) (result->get_value());
    }

    if (constraint) {
        states.push_back(currentState);

        for (;
                itVar != ((ast::identifiers*) _assignment->get_left())->end()
                        && itValue
                                != ((ast::expressions*) _assignment->get_right())->end();
                ) {
            //accept the value of the assignment
            (*itValue++)->accept(*this);
            ast::constant* result = pop();
            //get the index of the variable in the bits
            int id = varsMap[(*itVar++)->get_name()];
            if (id > 0)
                states.back().local[id - 1] = (bool) (result->get_value());
            if (id < 0)
                states.back().shared[0 - id - 1] = (bool) (result->get_value());
        }

        states.back().local.PC++;
    }

    if (states.size() > 0)
        resultingStates.push_back(states);

}

