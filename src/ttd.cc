#include "ttd.hh"

#include <stdlib.h>
#include <fstream>
#include <string>

TransitionSet::TransitionSet(std::string fileName)
{
	std::ifstream file;
	unsigned ns, nl;
	TTD_Config initConfig;

	file.open((fileName).c_str());
	if (!file) {
		std::cout << "Unable to open file";
		exit(1); // terminate with error
	}
			
	//read the number of shared states and number of local states
	file >> ns >> nl; 
	
	std::vector<Transition> tranVec;
	std::vector<std::vector<Transition> > tranVecVec;
	for (unsigned i = 0 ; i < nl; i++)
	{
		tranVecVec.clear();
		for(unsigned j = 0; j < ns; j++)
			tranVecVec.push_back(tranVec);
		transitions.push_back(tranVecVec);
	}

	Transition t;
	std::string sep;
	while (file)
		if (file >> t.fromSS >> t.fromLS >> sep >> t.toSS >> t.toLS)
		{
			__SAFE_ASSERT__ (sep == "->");
			insertTransition(t);
		};
		
	file.close();
}

void TransitionSet::insertTransition(const Transition& t)
{
	transitions[t.fromLS][t.fromSS].push_back(t);
}

std::list<TTD_Config*>& TransitionSet::getInitialConfigs(unsigned numThread = 0)
{
	std::list<TTD_Config*>* result = new std::list<TTD_Config*>();

	TTD_Local local; TTD_Shared shared;
	TTD_Config* config = new TTD_Config();
	
	vass::Counter counter(numThread);
	
	if(numThread == 0)
		counter.setToInfinite();

	config->state = shared;
	config->counters[new TTD_Local(local)] = counter;
	result->push_back(config);
	return *result;
}

std::list<TTD_Config*>& TransitionSet::getSuccessors(const TTD_Config* currentConfig)
{
	
	std::list<TTD_Config*>* successors = new std::list<TTD_Config*>();
	
	if (currentConfig == 0)
		return *successors;
	
	for(vass::StatePtrMapCounter<TTD_Local>::const_iterator itLS =  currentConfig->counters.begin(); 
		itLS!= currentConfig->counters.end(); itLS++)
	{
		unsigned ss = currentConfig->state.getValue();
		unsigned ls = itLS->first->getValue();
		
		for(std::vector<Transition>::const_iterator it = transitions[ls][ss].begin();
			it!=transitions[ls][ss].end(); it++)
		{
			TTD_Config* newConfig =currentConfig->clone();
			

			vass::StatePtrMapCounter<TTD_Local>::iterator source = newConfig->counters.find(itLS->first);
			source->second.decrease();
			if(source->second.isZero())
				newConfig->counters.erase(source);
			
			TTD_Local local(it->toLS); TTD_Shared shared(it->toSS);
			newConfig->state = shared;
			newConfig->counters[new TTD_Local(local)].increase();
			
			//optimization on the searching strategy
			if(newConfig->state == currentConfig->state)
				successors->push_front(newConfig);
			else
				successors->push_back(newConfig);
		   
		}
		
	}

	return *successors;

}

