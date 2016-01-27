#ifndef __VASS_H__
#define __VASS_H__

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <set>
#include <map>

#ifdef __SAFE_COMPUTATION__
#define __SAFE_COMPUTE__(stmt) stmt
#define __SAFE_ASSERT__(cond) assert(cond)  // an abbreviation for: __SAFE_COMPUTE__ (assert cond)
#else
#define __SAFE_COMPUTE__(stmt)
#define __SAFE_ASSERT__(cond)
#endif

/*To make your own VASS for a specific front-end, you have to implemented the 
 virtual methods of class AdditionSet and implement your own local state class 
 and shared state class with methods "operator ==", "operator <" and "out".
 
 Please refer to bp.h or ttd.h for example. 
 */
namespace vass
{
	template<class Local, class Shared>
	class ThreadState;
	
	//////////////////////////////////////////
	// class Counter
	//////////////////////////////////////////
	class Counter
	{
		unsigned count;
		//0 represents infinity and exact counts = value - 1
		public:
			Counter(){count = 1;};
			Counter(unsigned initCount){count = initCount + 1;};
			bool isInfinite()const {return (count == 0);};
			bool isZero()const{return (count == 1);};
			void setToInfinite(){count = 0;};
			void setToZero(){count = 1;};
			void increase(){if(!isInfinite()) count++;};
			bool decrease()
			{
				if(isZero()) return false;
				else
				{
					if(!isInfinite()) count--;
					return true;
				}
			};
			bool operator< (const Counter& other)const
			{
				if(isInfinite()) return false;
				else
				{
					if(other.isInfinite()) return true;
					else return (count < other.count);
				}
			};
			bool operator> (const Counter& other)const
			{	
				if(*this < other)	return false;
				if(*this == other) return false;
				return true;
			};
			bool operator== (const Counter& other)const
			{
				return (count == other.count);
			};
			void out(std::ostream& os)const
			{
				if(count == 0)	os << "w";
					else os << count - 1;
			};
					
	};
					
					
	//////////////////////////////////////////
	// type for States
	/////////////////////////////////////////
					
	//Ordering structure for pointers to states.
	template<class State>
	struct StatePtrLess: std::binary_function<const State *,const State *,bool> {
		bool operator()(const State *s1, const State *s2) const 
		{
			return *s1 < *s2;
		}
	};
					
	template<class State>
	class StatePtrMapCounter: public std::map<const State*, Counter, StatePtrLess<State> > 
	{};
					
	//////////////////////////////////////////
	// class Config
	//////////////////////////////////////////
					
	template<class Local, class Shared>
	class Config
	{
	public:
					
		//shared state and local state counter
		Shared state;
		//std::set<StateCounter<Local> > counters;
					
		StatePtrMapCounter<Local> counters;
					
		bool invalided;
					
		Config(){invalided = false;};
		Config (const Config<Local, Shared>& other) //:state(other.state), counters(other,counters){};
		{state = other.state; counters = other.counters;invalided = false;};
		bool covers(const Config<Local, Shared>*)const;
		void omega(const Config<Local, Shared>*);
		void out(std::ostream&) const;
		Config<Local, Shared>* clone()const{return new Config<Local,Shared>(*this);};
	};
					
	//////////////////////////////////////////
	// class ConfigNode
	//////////////////////////////////////////
					
	template<class Local, class Shared>
	class ConfigNode
	{
		Config<Local, Shared>* config;
		ConfigNode<Local, Shared>* predecessor;
		//ConfigNode<Local, Shared>* omegaPredecessor; this technique is not working, 
		//CE 1,10w -> 2,01w -> 1,00w, then 1,10w would be deleted
		//   1,10w -> 3,20w -> 1,21w and we still neex 1,10w in another path
		std::list<ConfigNode<Local, Shared>*> successors;
		unsigned ID;
	public:
		ConfigNode(unsigned id, Config<Local, Shared>* _config, ConfigNode<Local, Shared>* _predecessor)
		{ID = id; config = _config; predecessor = _predecessor;};
		~ConfigNode()
		{
			//delete config; :: delegate this job to the ConfigPtrCollection
			freeSuccessors();
		};
		unsigned getID()const{return ID;};
		//void setOmegaPredecessor(ConfigNode<Local,Shared>* _omegaPredecessor){omegaPredecessor = _omegaPredecessor;};
		ConfigNode<Local,Shared>* getPredecessor()
		{
			return predecessor;
		};
		Config<Local, Shared>* getConfig()const{return config;};
		void addSuccessor(ConfigNode<Local, Shared>* successor)
		{successors.push_back(successor);};

		void freeSuccessors()
		{
			while(successors.size()>0)
			{
				delete successors.back();
				successors.pop_back();
			};			
		};
	};
					
	//////////////////////////////////////////
	// class configPtrCollection
	//////////////////////////////////////////
	
	template<class Local, class Shared>
	class ConfigPtrCollection
	{
		unsigned count;
		//limit optimization :: one of the bottleneck procedure
		//std::list<std::list<Config<Local, Shared>*> > ptrCollection;
	public:
		std::list<std::list<Config<Local, Shared>*> > ptrCollection;
		ConfigPtrCollection(){count = 0;}
		~ConfigPtrCollection();
		unsigned size() const{return count;};
		bool insert(Config<Local, Shared>* config, bool sort=true);
	};
		
	//////////////////////////////////////////
	// class AdditionSet
	//////////////////////////////////////////
	
	template<class Local, class Shared>
	class AdditionSet
	{
	protected:
		AdditionSet(){};
	public:
		virtual std::list<Config<Local, Shared>*>& getInitialConfigs(unsigned) = 0;
		virtual std::list<Config<Local, Shared>*>& getSuccessors(const Config<Local, Shared>*) = 0;
	};
		
	//////////////////////////////////LocalPtrMapCounter////////
	// class ThreadState
	///////////////////////////////////////StatePtrMapCounter///

	template<class Local, class Shared>
	class ThreadState
	{
	public:
		Local local;
		Shared shared;
		bool invalided;
		ThreadState(){invalided = false;};
		bool operator < (const ThreadState<Local, Shared>& other) const
		{
			if(!(shared < other.shared || shared == other.shared)) return false;
			if(shared == other.shared && (!(local < other.local))) return false;
			return true;
		};
		void out(std::ostream& os)const
		{
			std::cout<< "[";
			shared.out(std::cout);std::cout<<","; local.out(std::cout);
			std::cout << "]";
		};
	};
					
	//////////////////////////////////////////
	// class ThreadStateSet
	//////////////////////////////////////////
	
	template<class Local, class Shared>
	class ThreadStateSet
	{
	std::set<ThreadState<Local,Shared> > threadStates;
	public:
		unsigned size()const {return threadStates.size();};
		void insert(const Config<Local, Shared>*);
		void out(std::ostream&)const;
	};
		
		//////////////////////////////////////////
		// class System
		//////////////////////////////////////////
		
	template<class Local, class Shared>
	class System
	{
		AdditionSet<Local, Shared>* additionSet;
		//ConfigPtrCollection<Local, Shared>* configPtrCollection;
		ThreadStateSet<Local, Shared>* threadStateSet;
		std::ofstream graphStream;
		unsigned exploredNodes;
		unsigned nodeID;
		
		void BreathFirstExploration(std::list<ConfigNode<Local, Shared>*>*, bool);
		
		void DepthFirstExploration(ConfigNode<Local, Shared>* configNode, bool);
		
		void FastDFS(ConfigNode<Local, Shared>* configNode);
		
		void FastBFS(std::list<ConfigNode<Local, Shared>*>*);
		
		void PrintTrace(ConfigNode<Local, Shared>* terminal);
		
	public:
		
		ConfigPtrCollection<Local, Shared>* configPtrCollection;
		typedef std::list<Config<Local,Shared>*> ConfigPtrList;
		
		System(AdditionSet<Local, Shared>* _additonSet)
		{
			graphFile = std::string("");
			exploredNodes = 0;
			nodeID= 0;
			additionSet = _additonSet;
			configPtrCollection = new ConfigPtrCollection<Local, Shared>();
			threadStateSet = new ThreadStateSet<Local, Shared>();
		};
		
		~System()
		{
			delete additionSet;
			delete configPtrCollection;
			delete threadStateSet;
		};
		
		std::string graphFile;
		unsigned numOfThreadState(){return threadStateSet->size();};
		unsigned numOfConfig(){return configPtrCollection->size();};
		unsigned numOfExploredNode(){return exploredNodes;};
		
		void outputThreadStates(std::ostream& os)const{threadStateSet->out(os);};
		
		void coverability(Config<Local, Shared>* init, bool, bool,bool);
	};
		
		
		///////////////////////////////////////////////////////////////
		// the implementation of some interface
		//////////////////////////////////////////////////////////////
		
	template<class Local, class Shared>
	void System<Local, Shared>::coverability(Config<Local, Shared>* init, bool km, bool depthFirst, bool fast)
	{
		exploredNodes = 1;
		configPtrCollection->insert(init);
		threadStateSet->insert(init);
		
		if(graphFile != "")
		{
			graphStream.open(graphFile.c_str());
			if (!graphStream) {
				std::cout << "Unable to open the graph file";
				graphFile = ""; // continue without generating graph
			}
			else
				graphStream << "digraph G" << std::endl << "{" 
				<< std::endl << "node [shape=record, width=.1, height=.1];" 
				<< std::endl;
		}
		
		if(depthFirst)
		{
			if(fast)
				FastDFS(new ConfigNode<Local, Shared>(nodeID++,init,0));
			else
				DepthFirstExploration(new ConfigNode<Local, Shared>(nodeID++,init,0), km);
		}
		else
		{
			std::list<ConfigNode<Local, Shared>*>* configNodes = new std::list<ConfigNode<Local, Shared>*>;
			configNodes->push_back(new ConfigNode<Local, Shared>(nodeID++,init,0));
			if(fast)
				FastBFS(configNodes);
			else
				BreathFirstExploration(configNodes, km);
		}
		
		if(graphFile != "")
		{
			graphStream << "}" << std::endl;
			graphStream.close();
		}
	};
		
	template<class Local, class Shared>
	void System<Local, Shared>::FastDFS(ConfigNode<Local, Shared>* configNode)
	{
		
		static std::list<ConfigPtrList> accelerateSet;
		//static double p1 = 0,p2 = 0,p3 = 0;
		
		bool comparable = false;
		//clock_t start,finish;
		//start = clock();
		typename std::list<ConfigPtrList>::iterator outer;
		typename ConfigPtrList::iterator inner;
		
		for(outer = accelerateSet.begin(); outer != accelerateSet.end(); outer++)
		{
			if(outer->size() > 0)
			{
				comparable = false;
				
				for(inner = outer->begin(); inner!= outer->end(); inner++)
				{
					if(configNode->getConfig()->covers(*inner))
						comparable = true;
					else
					{
						if((*inner)->covers(configNode->getConfig()))
							comparable = true;
						break;
					}
				}
				
				if(comparable)
				{
					inner = outer->insert(inner,configNode->getConfig());
					break;
				}
			}
		}
		
		if(!comparable)
		{
			ConfigPtrList emptyList;
			accelerateSet.push_back(emptyList);
			accelerateSet.back().push_back(configNode->getConfig());
		}
		
		//p1+= (double)(clock()-start)/CLOCKS_PER_SEC;
		
		if(graphFile != "")
		{
			graphStream << "node" << configNode->getID() << "  [label=" << '"'; 
			configNode->getConfig()->out(graphStream);
			graphStream<< '"' << "];" << std::endl;
		}
		
		static bool invalidStateFound = false;
		
		//by checking if the successors should be explored
		
		std::list<Config<Local, Shared> *> results = additionSet->getSuccessors(configNode->getConfig());
		
		for(typename std::list<Config<Local, Shared>* >::iterator it = results.begin(); 
				it!=results.end() && !invalidStateFound;it++)
		{
			//the main bottleneck of the whole procedure, takes about 75% time 
			//'omega' the successors
			//start = clock();
			for(typename std::list<ConfigPtrList>::iterator path = accelerateSet.begin();
					path != accelerateSet.end(); path++)			
			{
				////with the accelerator
				if((*it)->covers(*path->begin()))
					(*it)->omega(*path->begin());
			}
			
			//p2+= (double)(clock()-start)/CLOCKS_PER_SEC;
			exploredNodes++;
			
			//check existence of the current successors in the set of explored config
			//start = clock();
			bool todo = configPtrCollection->insert(*it);
			//p3+= (double)(clock()-start)/CLOCKS_PER_SEC;
			if(todo)
			{
				if(graphFile != "")
				{
					graphStream << "node" << nodeID << "  [label=" << '"'; 
					(*it)->out(graphStream);
					graphStream<< '"' << "];" << std::endl;
					graphStream << "node" << configNode->getID() 
					<< " -> " << "node" << nodeID<< ";" << std::endl;		
				}
				threadStateSet->insert(*it);
				FastDFS(new ConfigNode<Local, Shared>(nodeID++,*it,configNode));
			}
			else delete (*it);
		}
		
		if(outer != accelerateSet.end())
		{
			if(inner != outer->end())
				outer->erase(inner);
			
			if(outer->size() == 0)
				accelerateSet.erase(outer);
		}
		
		//std::cout << "p1=" << p1 << ", p2=" << p2 << ", p3=" << p3 << std::endl;
		delete configNode;
	};
		
	template<class Local, class Shared>
	void System<Local, Shared>::FastBFS(std::list<ConfigNode<Local, Shared>*>* configNodes)
	{		
		std::list<ConfigNode<Local, Shared>*>* newConfigNodes = new std::list<ConfigNode<Local, Shared>*>();
		static bool invalidStateFound = false;
		
		for(typename std::list<ConfigNode<Local, Shared>*>::const_iterator orig = configNodes->begin();
				orig!=configNodes->end()&& !invalidStateFound; orig++)
		{
			ConfigNode<Local, Shared>* configNode = *orig;
			
			if(graphFile != "")
			{
				graphStream << "node" << configNode->getID() << "  [label=" << '"'; 
				configNode->getConfig()->out(graphStream);
				graphStream<< '"' << "];" << std::endl;
			}
			
			std::list<Config<Local, Shared> *> results;
			results = additionSet->getSuccessors(configNode->getConfig());
			
			for(typename std::list<Config<Local, Shared>* >::iterator it = results.begin(); 
					it!=results.end()&& !invalidStateFound;it++)
			{
				//check assertion
				if((*it)->invalided)
				{
					ConfigNode<Local, Shared> *terminal = 
					new ConfigNode<Local, Shared>(nodeID++,*it,configNode);
					std::cout << "Assertion violated by trace(terminated):\n";
					PrintTrace(terminal);
					
					if(graphFile != "")
					{
						graphStream << "node" << nodeID << "  [label=" << '"'; 
						(*it)->out(graphStream);
						graphStream << "::assertion violated!" << '"' << "];" << std::endl;
						graphStream << "node" << configNode->getID() 
						<< " -> " << "node" << nodeID<< ";" << std::endl;		
					}						
					
					delete terminal;
					invalidStateFound = true;
					break;
				}		
				
				//the main bottleneck of the whole procedure, takes about 75% time 
				//'omega' the successors
				ConfigNode<Local, Shared>* prev = configNode;
				while(prev != 0)				
				{
					//(*it)->covers(prev->getConfig(),true);
					if((*it)->covers(prev->getConfig()))
						(*it)->omega(prev->getConfig());
					prev = prev->getPredecessor();
				}
				exploredNodes++;
				
				//check existence of the current successors in the set of explored config
				if(configPtrCollection->insert(*it))
				{
					
					if(graphFile != "")
					{
						graphStream << "node" << nodeID << "  [label=" << '"'; 
						(*it)->out(graphStream);
						graphStream<< '"' << "];" << std::endl;
						graphStream << "node" << configNode->getID() 
						<< " -> " << "node" << nodeID<< ";" << std::endl;		
					}
					
					threadStateSet->insert(*it);
					newConfigNodes->push_back(new ConfigNode<Local, Shared>(nodeID++,*it,configNode));
				}
				else
					delete (*it);//useless, i.e would not be accessed again
			}
		}
		
		if(newConfigNodes->size() > 0 )
			FastBFS(newConfigNodes);
		
		while(newConfigNodes->size()>0)
		{
			delete newConfigNodes->back();
			newConfigNodes->pop_back();
		}
		delete newConfigNodes;
	}
		
	template<class Local, class Shared>
	void System<Local, Shared>::DepthFirstExploration(ConfigNode<Local, Shared>* configNode, bool km)
	{
		
		/*	configNode->getConfig()->out(std::cout);
		 std::cout << std::endl << "-------" << std::endl;
		 std::cin.get();*/
		
		if(graphFile != "")
		{
			graphStream << "node" << configNode->getID() << "  [label=" << '"'; 
			configNode->getConfig()->out(graphStream);
			graphStream<< '"' << "];" << std::endl;
		}
		
		static bool invalidStateFound = false;
		
		//by checking if the successors should be explored
		
		if(km)
		{
			std::list<Config<Local, Shared> *> results = additionSet->getSuccessors(configNode->getConfig());
			for(typename std::list<Config<Local, Shared>* >::iterator it = results.begin(); 
					it!=results.end() && !invalidStateFound;it++)
			{
				
				//check assertion
				if((*it)->invalided)
				{
					ConfigNode<Local, Shared> *terminal = 
					new ConfigNode<Local, Shared>(nodeID++,*it,configNode);
					std::cout << "Assertion violated by trace(terminated):\n";
					PrintTrace(terminal);
					
					if(graphFile != "")
					{
						graphStream << "node" << nodeID << "  [label=" << '"'; 
						(*it)->out(graphStream);
						graphStream << "::assertion violated!" << '"' << "];" << std::endl;
						graphStream << "node" << configNode->getID() 
						<< " -> " << "node" << nodeID<< ";" << std::endl;		
					}					
					
					delete terminal;
					invalidStateFound = true;
					break;
				}
				
				exploredNodes++;
				
				bool toExplore = true;
				//check existence of the successor in previous path
				ConfigNode<Local, Shared>* prev = configNode;
				while(prev != 0 && toExplore)				
				{
					if ( (*it)->covers(prev->getConfig()) )
					{
						if (prev->getConfig()->covers(*it))
							toExplore = false;
						else
							(*it)->omega(prev->getConfig());
					}
					
					prev = prev->getPredecessor();
				}
				
				if (toExplore)
				{
					if(graphFile != "")
					{
						graphStream << "node" << nodeID << "  [label=" << '"'; 
						(*it)->out(graphStream);
						graphStream<< '"' << "];" << std::endl;
						graphStream << "node" << configNode->getID() 
						<< " -> " << "node" << nodeID<< ";" << std::endl;		
					}
					configPtrCollection->insert(*it, false);
					threadStateSet->insert(*it);
					DepthFirstExploration(new ConfigNode<Local, Shared>(nodeID++,*it,configNode), km);
				}
				else delete (*it);//useless, i.e would not be accessed again
			}
		}
		else
		{
			std::list<Config<Local, Shared> *> results = additionSet->getSuccessors(configNode->getConfig());
			
			for(typename std::list<Config<Local, Shared>* >::iterator it = results.begin(); 
					it!=results.end() && !invalidStateFound;it++)
			{
				
				//check assertion
				if((*it)->invalided)
				{
					ConfigNode<Local, Shared> *terminal = 
					new ConfigNode<Local, Shared>(nodeID++,*it,configNode);
					std::cout << "Assertion violated by trace(terminated):\n";
					PrintTrace(terminal);
					
					if(graphFile != "")
					{
						graphStream << "node" << nodeID << "  [label=" << '"'; 
						(*it)->out(graphStream);
						graphStream << "::assertion violated!" << '"' << "];" << std::endl;
						graphStream << "node" << configNode->getID() 
						<< " -> " << "node" << nodeID<< ";" << std::endl;		
					}
					
					delete terminal;
					invalidStateFound = true;					
					break;
				}
				
				//the main bottleneck of the whole procedure, takes about 75% time 
				//'omega' the successors
				ConfigNode<Local, Shared>* pred = configNode;
				ConfigNode<Local, Shared>* prev = pred;
				while(prev != 0)				
				{
					////without the accelerator
					if((*it)->covers(prev->getConfig()))
						(*it)->omega(prev->getConfig());
					prev = prev->getPredecessor();
				}
				exploredNodes++;
				
				//check existence of the current successors in the set of explored config
				if(configPtrCollection->insert(*it))
				{
					if(graphFile != "")
					{
						graphStream << "node" << nodeID << "  [label=" << '"'; 
						(*it)->out(graphStream);
						graphStream<< '"' << "];" << std::endl;
						graphStream << "node" << configNode->getID() 
						<< " -> " << "node" << nodeID<< ";" << std::endl;		
					}
					threadStateSet->insert(*it);
					DepthFirstExploration(new ConfigNode<Local, Shared>(nodeID++,*it,pred), km);
				}
				else delete (*it);//useless, i.e would not be accessed again
			}
		}
		delete configNode;
	};	
		
	template<class Local, class Shared>
	void System<Local, Shared>::BreathFirstExploration(std::list<ConfigNode<Local, Shared>*>* configNodes, bool km)
	{		
		std::list<ConfigNode<Local, Shared>*>* newConfigNodes = new std::list<ConfigNode<Local, Shared>*>();
		static bool invalidStateFound = false;
		
		if(km)
		{
			for(typename std::list<ConfigNode<Local, Shared>*>::const_iterator orig = configNodes->begin();
					orig!=configNodes->end() && !invalidStateFound; orig++)
			{
				ConfigNode<Local, Shared>* configNode = *orig;
				
				if(graphFile != "")
				{
					graphStream << "node" << configNode->getID() << "  [label=" << '"'; 
					configNode->getConfig()->out(graphStream);
					graphStream<< '"' << "];" << std::endl;
				}
				
				std::list<Config<Local, Shared> *> results;
				results = additionSet->getSuccessors(configNode->getConfig());
				
				for(typename std::list<Config<Local, Shared>* >::iterator it = results.begin(); 
						it!=results.end()&& !invalidStateFound;it++)
				{
					//check assertion
					if((*it)->invalided)
					{
						ConfigNode<Local, Shared> *terminal = 
						new ConfigNode<Local, Shared>(nodeID++,*it,configNode);
						std::cout << "Assertion violated by trace(terminated):\n";
						PrintTrace(terminal);
						
						if(graphFile != "")
						{
							graphStream << "node" << nodeID << "  [label=" << '"'; 
							(*it)->out(graphStream);
							graphStream << "::assertion violated!" << '"' << "];" << std::endl;
							graphStream << "node" << configNode->getID() 
							<< " -> " << "node" << nodeID<< ";" << std::endl;		
						}
						
						delete terminal;
						invalidStateFound = true;
						
						break;
					}
					
					exploredNodes++;
					bool toExplore = true;
					//check existence of the successor in previous path
					ConfigNode<Local, Shared>* prev = configNode;
					while(prev != 0 && toExplore)				
					{
						if ( (*it)->covers(prev->getConfig()) )
						{
							if (prev->getConfig()->covers(*it))
								toExplore = false;
							else
								(*it)->omega(prev->getConfig());
						}
						
						prev = prev->getPredecessor();
					}
					
					if (toExplore)
					{
						
						if(graphFile != "")
						{
							graphStream << "node" << nodeID << "  [label=" << '"'; 
							(*it)->out(graphStream);
							graphStream<< '"' << "];" << std::endl;
							graphStream << "node" << configNode->getID() 
							<< " -> " << "node" << nodeID<< ";" << std::endl;		
						}
						
						configPtrCollection->insert(*it, false);
						threadStateSet->insert(*it);
						newConfigNodes->push_back(new ConfigNode<Local, Shared>(nodeID++,*it,configNode));
					}
					else delete (*it);//useless, i.e would not be accessed again
				}
			}
		}
		else
		{
			for(typename std::list<ConfigNode<Local, Shared>*>::const_iterator orig = configNodes->begin();
					orig!=configNodes->end()&& !invalidStateFound; orig++)
			{
				ConfigNode<Local, Shared>* configNode = *orig;
				
				if(graphFile != "")
				{
					graphStream << "node" << configNode->getID() << "  [label=" << '"'; 
					configNode->getConfig()->out(graphStream);
					graphStream<< '"' << "];" << std::endl;
				}
				
				std::list<Config<Local, Shared> *> results;
				results = additionSet->getSuccessors(configNode->getConfig());
				
				for(typename std::list<Config<Local, Shared>* >::iterator it = results.begin(); 
						it!=results.end()&& !invalidStateFound;it++)
				{
					//check assertion
					if((*it)->invalided)
					{
						ConfigNode<Local, Shared> *terminal = 
						new ConfigNode<Local, Shared>(nodeID++,*it,configNode);
						std::cout << "Assertion violated by trace(terminated):\n";
						PrintTrace(terminal);
						
						if(graphFile != "")
						{
							graphStream << "node" << nodeID << "  [label=" << '"'; 
							(*it)->out(graphStream);
							graphStream << "::assertion violated!" << '"' << "];" << std::endl;
							graphStream << "node" << configNode->getID() 
							<< " -> " << "node" << nodeID<< ";" << std::endl;		
						}						
						
						delete terminal;
						invalidStateFound = true;
						break;
					}		
					
					//the main bottleneck of the whole procedure, takes about 75% time 
					//'omega' the successors
					ConfigNode<Local, Shared>* prev = configNode;
					while(prev != 0)				
					{
						//(*it)->covers(prev->getConfig(),true);
						if((*it)->covers(prev->getConfig()))
							(*it)->omega(prev->getConfig());
						prev = prev->getPredecessor();
					}
					exploredNodes++;
					
					//check existence of the current successors in the set of explored config
					if(configPtrCollection->insert(*it))
					{
						
						if(graphFile != "")
						{
							graphStream << "node" << nodeID << "  [label=" << '"'; 
							(*it)->out(graphStream);
							graphStream<< '"' << "];" << std::endl;
							graphStream << "node" << configNode->getID() 
							<< " -> " << "node" << nodeID<< ";" << std::endl;		
						}
						
						threadStateSet->insert(*it);
						newConfigNodes->push_back(new ConfigNode<Local, Shared>(nodeID++,*it,configNode));
					}
					else
						delete (*it);//useless, i.e would not be accessed again
				}
			}
		}
		
		if(newConfigNodes->size() > 0 )
			BreathFirstExploration(newConfigNodes, km);
		
		while(newConfigNodes->size()>0)
		{
			delete newConfigNodes->back();
			newConfigNodes->pop_back();
		}
		delete newConfigNodes;
		
	}
		
	template<class Local, class Shared>
	void System<Local, Shared>::PrintTrace(ConfigNode<Local, Shared>* terminal)
	{
		
		ConfigNode<Local, Shared>* current = terminal;
		ConfigNode<Local, Shared>* predecessor = current->getPredecessor();
		
		ThreadState<Local, Shared> newState;
		bool newStateFound = false;
		while(predecessor!=0)
		{
			newState.shared = current->getConfig()->state;
			
			typename StatePtrMapCounter<Local>::const_iterator it
			= current->getConfig()->counters.begin();
			newStateFound = false;
			for(;it!=current->getConfig()->counters.end();it++)
			{
				if(predecessor->getConfig()->counters.find(it->first) 
					 == predecessor->getConfig()->counters.end())
				{
					newState.local = *it->first;
					newStateFound = true; 
					break;
				}
				else if(it->second 
								> predecessor->getConfig()->counters.find(it->first)->second)
				{
					newState.local = *it->first;
					newStateFound = true; 
					break;
				}
			}
			
			if(newStateFound)
				newState.out(std::cout << " <- ");
			else
			{
				std::cout<< " <- [";
				newState.shared.out(std::cout);std::cout<<","; 
				std::cout <<"unknown";
				std::cout << "]";
			}
			
			std::cout << std::endl ;
			
			current = predecessor;
			predecessor = current->getPredecessor(); 
		}
		
		newState.shared = current->getConfig()->state;
		newState.local = *current->getConfig()->counters.begin()->first;
		newState.out(std::cout << " <- ");
		
		std::cout << std::endl;
		
	};
		
		
	template<class Local, class Shared>
	void Config<Local, Shared>::omega(const Config<Local, Shared>* other)
	{
		for(typename StatePtrMapCounter<Local>::iterator it = counters.begin(); 
				it!=counters.end(); it++)
		{
			typename StatePtrMapCounter<Local>::const_iterator oit = other->counters.find(it->first);
			
			if (oit == other->counters.end())
				it->second.setToInfinite();
			else if(oit->second < it->second)
				it->second.setToInfinite();
		}
		
	}
		
	template<class Local, class Shared>
	bool Config<Local, Shared>::covers(const Config<Local, Shared>* other)const
	{
		/*
		 out(std::cout<<"is ");
		 other->out(std::cout << " covers ");
		 std::cout << std::endl;
		 std::cin.get();
		 */		
		if(!(state == other->state))
			return false;
		
		for(typename StatePtrMapCounter<Local>::const_iterator it = other->counters.begin(); 
				it!=other->counters.end(); it++)
		{
			typename StatePtrMapCounter<Local>::const_iterator cit = counters.find(it->first);
			if (cit == counters.end())
				return false;
			
			if(cit->second < it->second)
				return false;
		}
		
		return true;
	}
		
	template<class Local, class Shared>
	void Config<Local, Shared>::out(std::ostream& os) const
	{
		os<< '[';
		state.out(os);
		os << ": ";
		if(counters.size() > 0)
		{
			typename StatePtrMapCounter<Local>::const_iterator it = counters.begin();
			it->second.out(os);
			os << '@';
			it++->first->out(os);
			for(;it!=counters.end();)
			{
				os<< ',';
				it->second.out(os);
				os << '@';
				it++->first->out(os);
			}
			os << ']';
		}
		
	};
		
	template <class Local, class Shared>
	ConfigPtrCollection<Local, Shared>::~ConfigPtrCollection()
	{
		typename std::list<std::list<Config<Local,Shared>*> >::iterator outer;
		
		for(outer = ptrCollection.begin(); outer != ptrCollection.end(); outer++)
			if(outer->size() > 0)
				for(typename std::list<Config<Local,Shared>*>::iterator inner = outer->begin(); inner!= outer->end(); inner++)
					delete *inner;
	}
	
	//possible bottleneck procedure, takes about 50% of time
	template <class Local, class Shared>
    bool ConfigPtrCollection<Local, Shared>::insert(Config<Local, Shared>* config, bool sort)
	{
		bool comparable = false;
		if(sort)
		{
			typename std::list<std::list<Config<Local,Shared>*> >::iterator outer;
			
			for(outer = ptrCollection.begin(); outer != ptrCollection.end(); outer++)
			{
				if(outer->size() > 0)
				{
					if((*outer->begin())->state == config->state)
					{
						comparable = true;
						for(typename std::list<Config<Local,Shared>*>::iterator inner = outer->begin(); inner!= outer->end(); inner++)
							if((*inner)->covers(config))
								return false;
						outer->push_back(config);
						break;
					}
				}
			}
			
			if(!comparable)
			{
				std::list<Config<Local,Shared>* > emptyList;
				ptrCollection.push_back(emptyList);
				ptrCollection.back().push_back(config);
			}
		}
		else
		{
			if(ptrCollection.size() == 0)
			{
				std::list<Config<Local,Shared>* > emptyList;
				ptrCollection.push_back(emptyList);
			}
			
			ptrCollection.front().push_back(config);
		}
		
		count++;
		return true;
	};
	
	template <class Local, class Shared>
	void ThreadStateSet<Local, Shared>::insert(const Config<Local, Shared>* config)
	{

		ThreadState<Local, Shared> ts;
		ts.shared = config->state;
		for(typename StatePtrMapCounter<Local>::const_iterator it = config->counters.begin(); 
				it!= config->counters.end();it++)
		{
			ts.local = *it->first;
			threadStates.insert(ts);
		}
	}
		
	template <class Local, class Shared>
	void vass::ThreadStateSet<Local, Shared>::out(std::ostream& os)const
	{
		typename std::set<ThreadState<Local, Shared> >::const_iterator it =  threadStates.begin();
		if(it!=threadStates.end())
		{
			(it++)->out(os);
			for(;it!=threadStates.end();it++)
			{std::cout << ", "; it->out(os);}
		}
	}


};
#endif 
