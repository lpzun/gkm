#ifndef __TTD_H__
#define __TTD_H__
#include <iostream>

#include "vass.hh"
class TTD_Local
{
	unsigned state;
public:
	TTD_Local(unsigned _state = 0):state(_state){};
	bool operator == (const TTD_Local& other)const
		{return (state == other.state);};
	bool operator < (const TTD_Local& other)const
		{return (state <other.state);};
	void out(std::ostream& os)const{os<< *(unsigned*)this;};
	unsigned getValue()const{return state;};
};

class TTD_Shared
{
	unsigned state;
public:
	TTD_Shared(unsigned _state = 0):state(_state){};
	bool operator == (const TTD_Shared& other)const
		{return (state == other.state);};
	bool operator < (const TTD_Shared& other)const
		{return (state <other.state);};
	void out(std::ostream& os)const{os<< *(unsigned*)this;};
	unsigned getValue()const{return state;};
};

typedef vass::Config<TTD_Local, TTD_Shared> TTD_Config;
typedef vass::AdditionSet<TTD_Local, TTD_Shared> TTD_AdditionSet;
typedef vass::System<TTD_Local, TTD_Shared> TTD_System;

class Transition
{
public:
	unsigned fromSS; // the shared state from
	unsigned toSS;   // the shared state to
	unsigned fromLS; // the local state from
	unsigned toLS;   // the local state to
	void out(std::ostream& os)const
	{
		os << "Shared state: " << fromSS << " -> " << toSS << "  "
		<< "Local state: " << fromLS << " -> " << toLS;
	};
};

class TransitionSet:public TTD_AdditionSet
{
	std::vector<std::vector<std::vector<Transition> > > transitions; // transitions in matrix
	void insertTransition(const Transition&);
public:
	TransitionSet(std::string fileName);
	~TransitionSet(){};
	std::list<TTD_Config*>& getInitialConfigs(unsigned);
	std::list<TTD_Config*>& getSuccessors(const TTD_Config*);
};

#endif 

