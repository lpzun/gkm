#ifndef __BP_H__
#define __BP_H__

#include <vector>
#include <map>
#include <set>
#include <stack>
#include <iostream>

#include "bp_parser/context.h"
#include <math.h>
#include "vass.hh"

using std::vector;

class BP_Local: private std::vector<bool>
{
public:
	unsigned PC;
	BP_Local(unsigned size = 0) : std::vector<bool>(size){};
	void out(std::ostream& os)const;
	std::_Bit_reference operator[](unsigned pos);
	bool operator == (const BP_Local& other)const;
	
	//note, the semantics of this < is not the comparison of between valuations but the ranks for ordering
	bool operator < (const BP_Local& other)const;
};

class BP_Shared: private std::vector<bool>
{
public:
	BP_Shared(unsigned size = 0) : std::vector<bool>(size){};
	bool operator == (const BP_Shared& other)const;
	bool operator < (const BP_Shared& other)const
	{return ((*(std::vector<bool>*)this) < other);};
	std::_Bit_reference operator[](unsigned pos);
	void out(std::ostream& os)const;
};

typedef vass::Config<BP_Local, BP_Shared> BP_Config;
typedef vass::AdditionSet<BP_Local, BP_Shared> BP_AdditionSet;
typedef vass::System<BP_Local, BP_Shared> BP_System;

class BPInterpretator:public BP_AdditionSet, public ast::visitor
{
	std::vector<ast::statement*> statements;
	ast::assignment* localVars;
	ast::assignment* sharedVars;
	std::map<std::string, signed> varsMap;
	std::map<string, unsigned> labelsMap;
	stack<ast::constant*> results;
	vass::ThreadState<BP_Local, BP_Shared> currentState;
	std::vector<std::vector<vass::ThreadState<BP_Local, BP_Shared> > > resultingStates;
	std::list<BP_Local*> statePtrCollection;
	std::list<BP_Config*>* successors;
	ast::constant::trival non_determinitic;
public:
	
	unsigned numOfshared()
	{
		if(sharedVars != 0)
		{
			return ((ast::identifiers*)sharedVars->getVars())->size();
		}
		else
			return 0;
	}
	
	unsigned numOflocal()
	{
		if(localVars != 0)
		{
			return ((ast::identifiers*)localVars->getVars())->size();
		}
		else
			return 0;
	}

	void printLocalStatesStat(std::ostream& os)
	{
		unsigned lvn = 0;
		if (localVars!=0)
			lvn = ((ast::identifiers*)localVars->getVars())->size();
		os << "#Local States: " << statements.size() << "*" << 2 << "^" << lvn << " = " << pow(2,lvn) * statements.size() << std::endl;
		
	};
	
	void printLocalVars(std::ostream& os)
	{
		if (localVars!=0)
		{
			std::list<ast::identifier*>::const_iterator itVar;
			for(itVar = ((ast::identifiers*)localVars->getVars())->begin();
				itVar!= ((ast::identifiers*)localVars->getVars())->end(); itVar++)
				os << (*itVar)->get_name();
		}
	};

	void printSharedVars(std::ostream& os)
	{
		if(sharedVars != 0)
		{
			std::list<ast::identifier*>::const_iterator itVar;
			for(itVar = ((ast::identifiers*)sharedVars->getVars())->begin();
				itVar!= ((ast::identifiers*)sharedVars->getVars())->end(); itVar++)
				os << (*itVar)->get_name();
		}
	};

	BPInterpretator(ast::program&,ast::constant::trival);
	~BPInterpretator();
	std::list<BP_Config*>& getInitialConfigs(unsigned numThread);
	std::list<BP_Config*>& getSuccessors(const BP_Config*);

	virtual void visit (ast::identifier*);
	virtual void visit (ast::constant*);
	virtual void visit (ast::neg*);
	virtual void visit (ast::conj*);
	virtual void visit (ast::disj*);
	virtual void visit (ast::jump*);
	virtual void visit (ast::assignment*);	
	virtual void visit (ast::skip*);
	virtual void visit (ast::bp_return*);
	virtual void visit (ast::if_begin*);
	virtual void visit (ast::if_end*);
	virtual void visit (ast::code*){};
	virtual void visit (ast::bp_assert*);
	virtual void visit (ast::bp_assume*);
	virtual void visit (ast::atomic_begin*);
	virtual void visit (ast::atomic_end*);
 	virtual void visit (ast::start_thread*);
 	virtual void visit (ast::end_thread*);
 	
protected:
	ast::constant* pop ()
	{
		ast::constant* result;
		result = results.top ();
		results.pop ();
		return result;
	}

};

#endif 
