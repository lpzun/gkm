/*******************************************************************\

File   : ast.h

This file contains the definition of the abstract syntax tree

\*******************************************************************/

#ifndef __AST_H__
#define __AST_H__

#ifdef __SAFE_COMPUTATION__
#define __SAFE_COMPUTE__(stmt) stmt
#define __SAFE_ASSERT__(cond) assert(cond)  // an abbreviation for: __SAFE_COMPUTE__ (assert cond)
#else
#define __SAFE_COMPUTE__(stmt)
#define __SAFE_ASSERT__(cond)
#endif
	
#include <string>
#include <list>
#include <iostream>
#include <ctime> 
#include <cstdlib>
using namespace std;

namespace ast {
	
	class visitor;

	enum NodeType{PROG, CODE, STATEMENT, ID, IDS, EXPRS
		, CONJ, DISJ, NEG, CONS, ASSIGN, IF_BEGIN,IF_END, JUMP, SKIP, ASSERT,START_THREAD,END_THREAD, ASSUME
		,ATOMIC_BEGIN, ATOMIC_END, BP_RETURN};

class expressions;

	class node
	{
	protected:
	  virtual ostream& print (ostream&) const = 0;
	public:
	  virtual ~node () { }
	  virtual node *clone () = 0;
	  virtual void accept (visitor&) = 0;
	  virtual NodeType getType() = 0;
	  friend ostream& operator<< (ostream&, const node&);
	};

	/*******************************************************************\

	identifier class. Used to represent identifiers and labels. Use get_name to
	get the name of the variable as a string object

	\*******************************************************************/
	
	class identifier: public node
	{
	protected:
	  string name;
	  virtual ostream& print (ostream& s) const;

	public:
	  identifier (const identifier& other);
	  identifier (const string& _name);
	  identifier& operator= (const identifier& other);
	  virtual node* clone ();
	  virtual void accept (visitor& _visitor);
	  NodeType getType() {return ID;}
	  string get_name () { return name; }
	};

	/*******************************************************************\

	binary_expr class. The base class for all binary expressions
	and for the assignment. Use get_left and get_right to obtain
	the corresponding sub-trees

	\*******************************************************************/
	class binary_expr: public node
	{
	protected:
	  node *left, *right;

	public:
	  binary_expr (node *_left, node *_right); 
	  binary_expr (const binary_expr& other);
	  virtual ~binary_expr ();
	  binary_expr& operator= (const binary_expr& other);

	  virtual node* get_left () { return left; }
	  virtual node* get_right () { return right; }
	};

	/*******************************************************************\

	unary_expr class. The base class for all unary expressions
	(i.e., only negations, currently. Use get_operand
	to obtain the operand of the element

	\*******************************************************************/
	class unary_expr: public node
	{
	protected:
	  node *operand;

	public:
	  unary_expr (node *_operand); 
	  unary_expr (const unary_expr& other);
	  virtual ~unary_expr ();
	  unary_expr& operator= (const unary_expr& other);

	  virtual node* get_operand () { return operand; }
	};

	/*******************************************************************\

	conj class. Used to represent conjunctions

	\*******************************************************************/
	class conj: public binary_expr
	{
	protected:
	  ostream& print (ostream& s) const;

	public:
	  conj (node* _left, node* _right): binary_expr (_left, _right) { }
	  virtual node* clone ();
	  virtual void accept (visitor& _visitor);
	  NodeType getType() {return CONJ;}
	};

	/*******************************************************************\

	disj class. Used to represent disjunctions

	\*******************************************************************/
	class disj: public binary_expr
	{
	protected:
	  ostream& print (ostream& s) const;

	public:
	  disj (node* _left, node* _right): binary_expr (_left, _right) { }
	  virtual node* clone ();
	  virtual void accept (visitor& _visitor);
	  NodeType getType() {return DISJ;}
	};


	/*******************************************************************\

	neg class. Used to represent negations

	\*******************************************************************/
	class neg: public unary_expr
	{
	protected:
	  ostream& print (ostream& s) const;

	public:
	  neg (node* _operand): unary_expr (_operand) { }
	  virtual node* clone ();
	  virtual void accept (visitor& _visitor);
	  NodeType getType() {return NEG;}
	};

	/*******************************************************************\

	constant class. Used to represent constants. Constants in our
	programming language can have three values, namely FALSE (for false),
	TRUE (for true), and STAR (for non-deterministic value). Use
	get_value to get the value of type ast::constant::trival.

	\*******************************************************************/
	class constant: public node
	{
	public:
	  typedef enum {FALSE, TRUE, STAR} trival;
	  constant (const constant& other);
	  constant (trival _value);
	  constant& operator= (const constant& other);
	  constant& operator! ()
	  {
		  if(value == FALSE)
		  {
			  value = TRUE;
		  }
		  else 
		  {
			  if(value == TRUE)
				  value = FALSE;
		  }
		  return *this;
	  }
	  virtual node* clone ();
	  virtual void accept (visitor& _visitor);

	  trival get_value () { return value; }
	  NodeType getType() {return CONS;}

	protected:
	  trival value;
	  ostream& print (ostream& s) const;
	};

	/*******************************************************************\

	identifiers class. Used to represent a sequence of identifiers, and
	currently only used to represent the declarations. The class
	provides begin and end methods to access the list<var*>
	iterators 

	\*******************************************************************/

	class identifiers: public node
	{
	enum ID_TYPE{UNSPECIFIC, SHARED, LOCAL};
	protected:
		list<identifier*> nodes;
		ID_TYPE type;
	public:
		identifiers () { }
		identifiers (identifier* _identifier);
		identifiers (const identifiers& other);
		identifiers& operator= (const identifiers& other);
		identifiers* push_front (identifier* _identifier);
		identifiers* setToShared () {type = SHARED; return this;};
		identifiers* setToLocal () {type = LOCAL; return this;};
		bool isShared()const {return (type == SHARED);};
		unsigned size()const {return nodes.size();};
		list<identifier*>::const_iterator begin () { return nodes.begin (); };
		list<identifier*>::const_iterator end () { return nodes.end (); };
		virtual node* clone ();
		virtual void accept (visitor& _visitor);
		virtual ~identifiers ();
	  NodeType getType() {return IDS;}

	protected:
		ostream& print (ostream& s) const;
	};

	/*******************************************************************\

	assignment class. Used to represent assignments

	\*******************************************************************/
	class assignment: public binary_expr
	{
	protected:
		ast::node* constraint;
		ostream& print (ostream& s) const;

	public:
		assignment (node* _lhs, node* _rhs, node* _constraint): binary_expr (_lhs, _rhs) 
		{ constraint = _constraint; }
		
		assignment (const assignment& other): binary_expr (other) 
		{   
			constraint = 0;
			if(other.constraint != 0)
				constraint = other.constraint->clone();
		}
				
		assignment (node* _lhs, node* _rhs): binary_expr (_lhs, _rhs) 
		{ constraint = 0; }
		~assignment() 
		{if (constraint !=0 ) delete constraint;};
		ast::node* getConstraint(){return constraint;};
		identifiers* getVars(){return (identifiers*)(get_left());};
		expressions* getExprs(){return (expressions*)(get_right());};
		virtual node* clone ();
		virtual void accept (visitor& _visitor);
		NodeType getType() {return ASSIGN;}
	};


	/*******************************************************************\

	bp_assert class. Used to represent bp_assert statements

	\*******************************************************************/
	class bp_assert: public node
	{
	protected:
		ast::node* constraint;
		virtual ostream& print (ostream& s) const;
	public:
		bp_assert (const bp_assert& other);
		bp_assert (node* _constraint): constraint(_constraint) {}
		~bp_assert () {delete constraint;};
		bp_assert& operator= (const bp_assert& other);
		virtual node* clone ();
		virtual void accept (visitor& _visitor);
		NodeType getType() {return ASSERT;}
	};
	/*******************************************************************\

	start_thread class. Used to represent start_thread statements

	\*******************************************************************/
	class start_thread: public node
	{
	protected:
		ast::identifiers* targets;
		virtual ostream& print (ostream& s) const;

	public:
		start_thread (const ast::start_thread& other) ;
		start_thread (identifiers* _targets): targets(_targets) {}
		~start_thread () {delete targets;};
		start_thread& operator= (const start_thread& other);
		list<ast::identifier*>::const_iterator begin(){return targets->begin();};
		list<ast::identifier*>::const_iterator end(){return targets->end();};
		virtual node* clone ();
		virtual void accept (visitor& _visitor);
		NodeType getType() {return START_THREAD;}
	};
	
	/*******************************************************************\

	end_thread class. Used to represent end_thread statements

	\*******************************************************************/
	class end_thread: public node
	{
	protected:
		virtual ostream& print (ostream& s) const;
	public:
		virtual node* clone ();
		virtual void accept (visitor& _visitor);
		NodeType getType() {return END_THREAD;}
	};
	
	/*******************************************************************\

	bp_assume class. Used to represent bp_assume statements

	\*******************************************************************/
	class bp_assume: public node
	{
	protected:
		ast::node* constraint;
		virtual ostream& print (ostream& s) const;
	public:
		bp_assume (const bp_assume& other);
		bp_assume (node* _constraint): constraint(_constraint) {}
		~bp_assume () {delete constraint;};
		bp_assume& operator= (const bp_assume& other);
		virtual node* clone ();
		virtual void accept (visitor& _visitor);
		NodeType getType() {return ASSUME;}
	};	
	
	/*******************************************************************\

	jump class. Used to represent jump statements

	\*******************************************************************/
	class jump: public node
	{
	protected:
		ast::identifiers* targets;
		virtual ostream& print (ostream& s) const;

	public:
		jump (const ast::jump& other) ;
		jump (identifiers* _targets): targets(_targets) {}
		~jump () {delete targets;};
		jump& operator= (const jump& other);
		list<ast::identifier*>::const_iterator begin(){return targets->begin();};
		list<ast::identifier*>::const_iterator end(){return targets->end();};
		virtual node* clone ();
		virtual void accept (visitor& _visitor);
		NodeType getType() {return JUMP;}
	};

	/*******************************************************************\

	skip class. Used to represent skip statements

	\*******************************************************************/
	class skip: public node
	{
	protected:
		virtual ostream& print (ostream& s) const;
	public:
		virtual node* clone ();
		virtual void accept (visitor& _visitor);
		NodeType getType() {return SKIP;}
	};
	
	/*******************************************************************\
	 
	 bp_return class. Used to represent skip statements
	 
	 \*******************************************************************/
	class bp_return: public node
		{
		protected:
			virtual ostream& print (ostream& s) const;
		public:
			virtual node* clone ();
			virtual void accept (visitor& _visitor);
			NodeType getType() {return BP_RETURN;}
		};
	
	/*******************************************************************\

	atomic_begin class. Used to represent atomic_begin statements

	\*******************************************************************/
	class atomic_begin: public node
	{
	protected:
		virtual ostream& print (ostream& s) const;
	public:
		virtual node* clone ();
		virtual void accept (visitor& _visitor);
		NodeType getType() {return ATOMIC_BEGIN;}
	};
	
	/*******************************************************************\

	atomic_end class. Used to represent atomic_end statements

	\*******************************************************************/
	class atomic_end: public node
	{
	protected:
		virtual ostream& print (ostream& s) const;
	public:
		virtual node* clone ();
		virtual void accept (visitor& _visitor);
		NodeType getType() {return ATOMIC_END;}
	};

	/*******************************************************************\

	if_begin class. Used to represent if statement begin

	\*******************************************************************/
	class if_begin: public node
	{
		ast::node* condition;
		unsigned jumpTo; //where to jump when consition is not satified
	protected:
		virtual ostream& print (ostream& s) const;
	public:
		if_begin (const ast::if_begin& other) ;
		if_begin (node* _condition): condition(_condition) {jumpTo = 0;}
		~if_begin () {delete condition;};
		if_begin& operator= (const if_begin& other);
		void JumpTo(unsigned _jumpTo){jumpTo = _jumpTo;}
		unsigned getJumpTo()const{return jumpTo;}
		virtual node* clone ();
		virtual void accept (visitor& _visitor);
		NodeType getType() {return IF_BEGIN;}
	};

	/*******************************************************************\

	if_end class. Used to represent if statement end

	\*******************************************************************/
	class if_end: public node
	{
	protected:
		virtual ostream& print (ostream& s) const;
	public:
		virtual node* clone ();
		virtual void accept (visitor& _visitor);
		NodeType getType() {return IF_END;}
	};

	/*******************************************************************\

	statement class. Used to represent statements

	\*******************************************************************/

	class statement: public node
	{
	protected:

		ostream& print (ostream& s) const;

	public:
		std::list<ast::node*> labels;
		ast::node* code;
		statement (const ast::statement& other);
		
		statement(node* _code):code(_code){ }
		~statement() {
			while(labels.size() > 0)
			{
				delete labels.back();
				labels.pop_back();
			}
			if (code != 0 ) delete code;
		};
		statement& operator= (const statement& other);
		virtual node* clone ();
		virtual void accept (visitor& _visitor);
		NodeType getType() {return STATEMENT;}
	};

	/*******************************************************************\

	code class. Used to represent a list of assignments.
	This object is used to represent the initialization code 
	as well as the loop body. The begin and end methods provide
	the list<assignment*> iterators for the list of assignments.

	\*******************************************************************/
	class code: public node
	{
	protected:
		list<statement*> nodes;
	public:
		
	  code () { }
	  code (statement* _statement);
	  code (const code& other);
	  code& operator= (const code& other);
	  statement* front(){return nodes.front();}
	  code* push_front (statement* _statement);
	  list<statement*>::const_iterator begin () { return nodes.begin (); }
	  list<statement*>::const_iterator end () { return nodes.end (); };
	  virtual node* clone ();
	  virtual void accept (visitor& _visitor);
	  virtual ~code ();
		NodeType getType() {return CODE;}

	protected:
	  ostream& print (ostream& s) const;
	};

	/*******************************************************************\

	expressions class. Used to represent a sequence of expressions

	\*******************************************************************/

	class expressions: public node
	{
	protected:
		list<node*> nodes;
	public:
		expressions () { }
		expressions (node* _expr);
		expressions (const expressions& other);
		expressions& operator= (const expressions& other);
		expressions* push_front (node* _expression);
		unsigned size()const {return nodes.size();};
		list<node*>::const_iterator begin () { return nodes.begin (); };
		list<node*>::const_iterator end () { return nodes.end (); };
		virtual node* clone ();
		virtual void accept (visitor& _visitor);
		virtual ~expressions ();
		NodeType getType() {return EXPRS;}

	protected:
		ostream& print (ostream& s) const;
	};

	/*******************************************************************\

	program class. Represents a crippled Boolean program that doesn't
	contain any loops or conditional statements. We assume that
	the body is executed until a fixpoint is reached

	\*******************************************************************/
	class program: public node
	{
	protected:
		assignment* sharedVars;
		assignment* localVars;

		code*      main;
		ostream& print (ostream& s) const;

	public:
		program (identifiers*, code*);
		program (identifiers*,identifiers*, code*);
		program (assignment*, code*){};
		program (assignment*,assignment*, code*);

		program (const program& other);
		program& operator= (const program& other);
		virtual node* clone ();
		virtual assignment* getSharedVars () { return sharedVars; }
		virtual assignment* getLocalVars () { return localVars; }
		virtual code* getMain () { return main; }
		virtual void accept (visitor& _visitor);
		virtual ~program ();
		NodeType getType() {return PROG;}
	};

	/*******************************************************************\

	visitor class. This is the interface for visitor classes that
	implement the visitor pattern (see Gamma et al., "Design Patterns").

	\*******************************************************************/
	class visitor
	{
	public:
		virtual void visit (identifier*)=0;
		virtual void visit (constant*) = 0;
		virtual void visit (neg*)= 0;
		virtual void visit (conj*)= 0;
		virtual void visit (disj*)= 0;
		virtual void visit (code*) = 0;
		virtual void visit (jump*) = 0;
		virtual void visit (skip*) = 0;
		virtual void visit (bp_return*) = 0;
		virtual void visit (atomic_begin*) = 0;
		virtual void visit (atomic_end*) = 0;
		virtual void visit (if_begin*) = 0;
		virtual void visit (if_end*) = 0;
		virtual void visit (assignment*) = 0;
		virtual void visit (bp_assert*) = 0;
		virtual void visit (bp_assume*) = 0;		
		virtual void visit (start_thread*) = 0;	
		virtual void visit (end_thread*) = 0;		
		virtual void visit (statement*){};// = 0;	  
		virtual void visit (expressions*){};// = 0;
		virtual void visit (identifiers*){};
		virtual void visit (program*){};

		virtual ~visitor ();
	protected:
	  visitor ();
	};
}

#endif
