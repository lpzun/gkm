/*******************************************************************\

File   : ast.cpp

Implementation of the classes defined in ast.h. Refer to ast.h
for detailed documentation!

\*******************************************************************/

#include "ast.h"

/* binary_expr */

ast::binary_expr::binary_expr (ast::node *_left, ast::node *_right): 
    left (_left), right (_right) 
{ 
}

ast::binary_expr::binary_expr (const ast::binary_expr& other)
{
  left = other.left->clone ();
  right = other.right->clone ();
}

ast::binary_expr::~binary_expr ()
{
  delete left; 
  delete right;
}

ast::binary_expr& ast::binary_expr::operator= (const ast::binary_expr& other)
{
  if (&other != this)
    {
      delete left;
      delete right;
      
      left = other.left->clone ();
      right = other.right->clone ();
    }
  return *this;
}

/* unary_expr */

ast::unary_expr::unary_expr (ast::node *_operand): 
  operand (_operand) 
{ 
}

ast::unary_expr::unary_expr (const ast::unary_expr& other)
{
  operand = other.operand->clone ();
}

ast::unary_expr::~unary_expr ()
{
  delete operand;
}

ast::unary_expr& ast::unary_expr::operator= (const ast::unary_expr& other)
{
  if (&other != this)
    {
      delete operand;
      
      operand = other.operand->clone ();
    }
  return *this;
}

/* conj */

ostream& ast::conj::print (ostream& s) const
{
  return s << "(" << *left << " && " << *right << ")";
}

void ast::conj::accept (visitor& _visitor)
{
  left->accept (_visitor);
  right->accept (_visitor);
   _visitor.visit (this);
}

ast::node* ast::conj::clone () 
{ 
  return new ast::conj (*this); 
}

/* disj */

ostream& ast::disj::print (ostream& s) const
{
  return s << "(" << *left << " || " << *right << ")";
}

void ast::disj::accept (ast::visitor& _visitor)
{
	left->accept (_visitor);
	right->accept (_visitor);
	_visitor.visit (this);
}

ast::node* ast::disj::clone () 
{ 
  return new ast::disj (*this); 
}

/* skip */
ostream& ast::skip::print (ostream& s) const
{
	return s << "skip";
}

ast::node* ast::skip::clone ()
{
	return ((ast::node*)(new skip()));
}

void ast::skip::accept (visitor& _visitor)
{
	_visitor.visit(this);
}

/* bp_return */
ostream& ast::bp_return::print (ostream& s) const
{
	return s << "bp_return";
}

ast::node* ast::bp_return::clone ()
{
	return ((ast::node*)(new bp_return()));
}

void ast::bp_return::accept (visitor& _visitor)
{
	_visitor.visit(this);
}

/* end_thread */
ostream& ast::end_thread::print (ostream& s) const
{
	return s << "end_thread";
}

ast::node* ast::end_thread::clone ()
{
	return ((ast::node*)(new end_thread()));
}

void ast::end_thread::accept (visitor& _visitor)
{
	_visitor.visit(this);
}

/* atomic_begin */
ostream& ast::atomic_begin::print (ostream& s) const
{
	return s << "atomic_begin";
}

ast::node* ast::atomic_begin::clone ()
{
	return ((ast::node*)(new atomic_begin()));
}

void ast::atomic_begin::accept (visitor& _visitor)
{
	_visitor.visit(this);
}

/* atomic_end */
ostream& ast::atomic_end::print (ostream& s) const
{
	return s << "atomic_end";
}

ast::node* ast::atomic_end::clone ()
{
	return ((ast::node*)(new atomic_end()));
}

void ast::atomic_end::accept (visitor& _visitor)
{
	_visitor.visit(this);
}

/* bp_assert */

ast::bp_assert::bp_assert (const ast::bp_assert& other) 
{
	constraint = other.constraint->clone();
}

ostream& ast::bp_assert::print (ostream& s) const
{
	return s << "bp_assert("<< *constraint << ")";
}

ast::bp_assert& ast::bp_assert::operator= (const ast::bp_assert &other)
{
  if (&other != this)
    {
      /* first delete all elements */
      delete constraint;

	  constraint = other.constraint->clone ();
    }
  return *this;
}

ast::node* ast::bp_assert::clone () 
{ 
  return new ast::bp_assert (*this); 
}

void ast::bp_assert::accept (visitor& _visitor)
{
	constraint->accept(_visitor);
	_visitor.visit(this);
};

/* bp_assume */

ast::bp_assume::bp_assume (const ast::bp_assume& other) 
{
	constraint = other.constraint->clone();
}

ostream& ast::bp_assume::print (ostream& s) const
{
	return s << "bp_assume("<< *constraint << ")";
}

ast::bp_assume& ast::bp_assume::operator= (const ast::bp_assume &other)
{
  if (&other != this)
    {
      /* first delete all elements */
      delete constraint;

	  constraint = other.constraint->clone ();
    }
  return *this;
}

ast::node* ast::bp_assume::clone () 
{ 
  return new ast::bp_assume (*this); 
}

void ast::bp_assume::accept (visitor& _visitor)
{
	constraint->accept(_visitor);
	_visitor.visit(this);
};

/* start_thread */

ast::start_thread::start_thread (const ast::start_thread& other) 
{
	targets = (ast::identifiers*)other.targets->clone();
}

ostream& ast::start_thread::print (ostream& s) const
{
	return s << "start_thread: "<< *targets;
}

ast::start_thread& ast::start_thread::operator= (const ast::start_thread &other)
{
  if (&other != this)
    {
      /* first delete all elements */
      delete targets;

	  targets = (ast::identifiers*)other.targets->clone ();
    }
  return *this;
}

ast::node* ast::start_thread::clone () 
{ 
  return new ast::start_thread (*this); 
}

void ast::start_thread::accept (visitor& _visitor)
{
	_visitor.visit(this);
};

/* jump */

ast::jump::jump (const ast::jump& other) 
{
	targets = (ast::identifiers*)other.targets->clone();
}

ostream& ast::jump::print (ostream& s) const
{
	return s << "goto: "<< *targets;
}

ast::jump& ast::jump::operator= (const ast::jump &other)
{
  if (&other != this)
    {
      /* first delete all elements */
      delete targets;

	  targets = (ast::identifiers*)other.targets->clone ();
    }
  return *this;
}

ast::node* ast::jump::clone () 
{ 
  return new ast::jump (*this); 
}

void ast::jump::accept (visitor& _visitor)
{
	_visitor.visit(this);
};

/* if_begin */

ast::if_begin::if_begin (const ast::if_begin& other) 
{
	condition = other.condition->clone();
	jumpTo = other.jumpTo;
}

ostream& ast::if_begin::print (ostream& s) const
{
	return s << "if ("<< *condition << ")";
}

ast::if_begin& ast::if_begin::operator= (const ast::if_begin &other)
{
  if (&other != this)
    {
      /* first delete all elements */
      delete condition;

	  condition = other.condition->clone ();
	  jumpTo = other.jumpTo;
    }
  return *this;
}

ast::node* ast::if_begin::clone () 
{ 
  return new ast::if_begin (*this); 
}

void ast::if_begin::accept (visitor& _visitor)
{
	condition->accept(_visitor);
	_visitor.visit(this);
};

/* if_end */
ostream& ast::if_end::print (ostream& s) const
{
	return s << "if_end";
}

ast::node* ast::if_end::clone ()
{
	return ((ast::node*)(new if_end()));
}

void ast::if_end::accept (visitor& _visitor)
{
	_visitor.visit(this);
}

/* statement */

ast::statement::statement (const ast::statement& other) 
{
	labels.clear();
	for(list<ast::node*>::const_iterator it = other.labels.begin(); it!=other.labels.end(); it++)
		labels.push_back((*it)->clone());
	code = other.code->clone();
}

ostream& ast::statement::print (ostream& s) const
{
	unsigned i = 0;
	for(list<ast::node*>::const_iterator it = labels.begin(); it!=labels.end(); it++)
	{
		s << **it << ": ";
		if(labels.size() > ++i)
			s << std::endl;
	}

	return s << *code;
}

ast::statement& ast::statement::operator= (const ast::statement &other)
{
  if (&other != this)
    {
      /* first delete all elements */
	
  		while(labels.size() > 0)
		{
			delete labels.back();
			labels.pop_back();
		}		
		if (code != 0 ) delete code;
		
		for(list<ast::node*>::const_iterator it = other.labels.begin(); it!=other.labels.end(); it++)
			labels.push_back((*it)->clone());
	  	code = (ast::statement*)other.code->clone ();
    }
  return *this;
}

ast::node* ast::statement::clone () 
{ 
  return new ast::statement (*this); 
}

void ast::statement::accept (visitor& _visitor)
{
	code->accept(_visitor);
	_visitor.visit(this);
}

/* assignment */

ostream& ast::assignment::print (ostream& s) const
{
	if(constraint!=0)
		return s << *left << " = " << *right << " ( " << *constraint << " )";
	else
		return s << *left << " = " << *right;
}

void ast::assignment::accept (ast::visitor& _visitor)
{
   _visitor.visit (this);
}

ast::node* ast::assignment::clone () 
{ 
  return new ast::assignment (*this); 
}

/* neg */

ostream& ast::neg::print (ostream& s) const
{
  return s << "!" << *operand;
}

void ast::neg::accept (ast::visitor& _visitor)
{
  operand->accept (_visitor);
   _visitor.visit (this);
}

ast::node* ast::neg::clone () 
{ 
  return new ast::neg (*this); 
}

/* identifier */

ast::identifier::identifier (const ast::identifier& other):
  name (other.name)
{
}

ast::identifier::identifier (const string& _name):
  name (_name)
{
}

ast::identifier& ast::identifier::operator= (const ast::identifier &other)
{
  if (&other != this)
    {
      name = other.name;
    }
  return *this;
}

ast::node* ast::identifier::clone () 
{ 
  return new ast::identifier (*this); 
}

ostream& ast::identifier::print (ostream& s) const
{
  return s << name;
}

void ast::identifier::accept (ast::visitor& _visitor)
{
  _visitor.visit (this);
}


/* constant */

ast::constant::constant (const ast::constant &other) 
{ 
  value = other.value; 
}

ast::constant::constant (ast::constant::trival _value):
  value (_value)
{ 
}

ostream& ast::constant::print (ostream& s) const
{
  return (value == ast::constant::STAR ? s << "*" : s << value);
}

ast::constant& ast::constant::operator= (const ast::constant &other)
{
  value = other.value;
  return *this;
}

void ast::constant::accept (ast::visitor& _visitor)
{
   _visitor.visit (this);
}

ast::node* ast::constant::clone () 
{ 
  return new ast::constant (*this); 
}

/* code */

ast::code::code (const ast::code& other) 
{ 
  for (list<statement*>::const_iterator it = other.nodes.begin ();
       it != other.nodes.end();
       it++)
    {
      nodes.push_back ((statement*)(*it)->clone ());
    }
}

ast::code::code (ast::statement* _statement)
{ 
  nodes.push_back (_statement);
}

ast::code* ast::code::push_front (ast::statement* _statement)
{
  nodes.push_front (_statement);
  return this;
}

ostream& ast::code::print (ostream& s) const
{
  for (list<ast::statement*>::const_iterator it = nodes.begin ();
       it != nodes.end();
       it++)
    {
      s << **it;
      list<ast::statement*>::const_iterator next = it;
      if (++next != nodes.end())
        s << ";";
      s<< endl;
    }
  return s;
}

ast::code& ast::code::operator= (const ast::code &other)
{
  if (&other != this)
    {
      /* first delete all elements */
      for (list<ast::statement*>::iterator it = nodes.begin ();
           it != nodes.end();
           it++)
        {
          delete *it;
        } 

      nodes.clear ();

      /* now insert clones of the nodes of other */
      for (list<ast::statement*>::const_iterator it = other.nodes.begin ();
           it != other.nodes.end();
           it++)
        {
          nodes.push_back ((ast::statement*)((*it)->clone()));
        } 
    }
  return *this;
}

void ast::code::accept (ast::visitor& _visitor)
{
  for (list<statement*>::iterator it = nodes.begin ();
       it != nodes.end();
       it++)
    {
      (*it)->accept (_visitor);
    }

   _visitor.visit (this);
}

ast::node* ast::code::clone () 
{ 
  return new ast::code (*this); 
}

ast::code::~code ()
{
  for (list<statement*>::const_iterator it = nodes.begin ();
       it != nodes.end();
       it++)
    {
      delete *it;
    }
}

/* identifiers */

ast::identifiers::identifiers (const ast::identifiers& other) 
{ 
  for (list<ast::identifier*>::const_iterator it = other.nodes.begin ();
       it != other.nodes.end();
       it++)
    {
      nodes.push_back ((ast::identifier*)(*it)->clone ());
    }
  type = other.type;
}

ast::identifiers::identifiers (ast::identifier* _identifier)
{ 
  nodes.push_back (_identifier);
}

ast::identifiers* ast::identifiers::push_front (ast::identifier* _identifier)
{
  nodes.push_front (_identifier);
  return this;
}

ostream& ast::identifiers::print (ostream& s) const
{
  for (list<ast::identifier*>::const_iterator it = nodes.begin ();
       it != nodes.end();
       it++)
    {
      s << **it;
      list<ast::identifier*>::const_iterator next = it;
      if (++next != nodes.end ())
        s << ", ";
    }
  return s;
}

ast::identifiers& ast::identifiers::operator= (const ast::identifiers &other)
{
  if (&other != this)
    {
      /* first delete all elements */
      for (list<ast::identifier*>::iterator it = nodes.begin ();
           it != nodes.end();
           it++)
        {
          delete *it;
        } 

      nodes.clear ();

      /* now insert clones of the nodes of other */
      for (list<ast::identifier*>::const_iterator it = other.nodes.begin ();
           it != other.nodes.end();
           it++)
        {
          nodes.push_back ((ast::identifier*)((*it)->clone()));
        } 
    }
  return *this;
}

void ast::identifiers::accept (ast::visitor& _visitor)
{
  for (list<ast::identifier*>::iterator it = nodes.begin ();
       it != nodes.end();
       it++)
    {
      (*it)->accept (_visitor);
    }

   _visitor.visit (this);
}

ast::node* ast::identifiers::clone () 
{ 
  return new ast::identifiers (*this); 
}

ast::identifiers::~identifiers ()
{
  for (list<ast::identifier*>::const_iterator it = nodes.begin ();
       it != nodes.end();
       it++)
    {
      delete *it;
    }
}

/* expressions */

ast::expressions::expressions (const ast::expressions& other) 
{ 
  for (list<ast::node*>::const_iterator it = other.nodes.begin ();
       it != other.nodes.end();
       it++)
    {
      nodes.push_back ((ast::node*)(*it)->clone ());
    }
}

ast::expressions::expressions (ast::node* _expression)
{ 
  nodes.push_back (_expression);
}

ast::expressions* ast::expressions::push_front (ast::node* _expression)
{
  nodes.push_front (_expression);
  return this;
}

ostream& ast::expressions::print (ostream& s) const
{
  for (list<ast::node*>::const_iterator it = nodes.begin ();
       it != nodes.end();
       it++)
    {
      s << **it;
      list<ast::node*>::const_iterator next = it;
      if (++next != nodes.end ())
        s << ", ";
    }
  return s;
}

ast::expressions& ast::expressions::operator= (const ast::expressions &other)
{
  if (&other != this)
    {
      /* first delete all elements */
      for (list<ast::node*>::iterator it = nodes.begin ();
           it != nodes.end();
           it++)
        {
          delete *it;
        } 

      nodes.clear ();

      /* now insert clones of the nodes of other */
      for (list<ast::node*>::const_iterator it = other.nodes.begin ();
           it != other.nodes.end();
           it++)
        {
          nodes.push_back ((ast::node*)((*it)->clone()));
        } 
    }
  return *this;
}

void ast::expressions::accept (ast::visitor& _visitor)
{
  for (list<ast::node*>::iterator it = nodes.begin ();
       it != nodes.end();
       it++)
    {
      (*it)->accept (_visitor);
    }

   _visitor.visit (this);
}

ast::node* ast::expressions::clone () 
{ 
  return new ast::expressions (*this); 
}

ast::expressions::~expressions ()
{
  for (list<ast::node*>::const_iterator it = nodes.begin ();
       it != nodes.end();
       it++)
    {
      delete *it;
    }
}

/* program */

ast::program::program (const ast::program& other) 
{ 
  sharedVars = (ast::assignment*)other.sharedVars;
  localVars = (ast::assignment*)other.localVars;
  main = (ast::code*)other.main;
}


ast::program::program (assignment* _shared,assignment* _local, code* _main)
{
	sharedVars = (ast::assignment*)_shared;
	localVars = (ast::assignment*)_local;
	main = _main;

	if (localVars != 0)
		localVars->clone();

	if (sharedVars != 0 )
		sharedVars->clone();

	if (main != 0)
		main->clone();

}

ostream& ast::program::print (ostream& s) const
{
	if (localVars != 0)
		s << "local " << *localVars << ";" << endl;
	if (sharedVars != 0 )
		s << "shared " << *sharedVars << ";" << endl;
	if (main != 0)
		s << "main { " << endl << *main << "}" << endl;
	return s;
}

ast::program& ast::program::operator= (const ast::program &other)
{
	if (&other != this)
	{
	      /* first delete all elements */
		if (localVars != 0)
		{
			delete localVars;
		localVars = (ast::assignment*)other.localVars->clone ();
		}

		if (sharedVars != 0 )
		{
			delete sharedVars;
			sharedVars = (ast::assignment*)other.sharedVars->clone ();
		}

		if (main != 0)
		{
			delete main;
			main = (ast::code*)other.main->clone ();
		}
    }

  	return *this;
}

void ast::program::accept (ast::visitor& _visitor)
{
	if (localVars != 0)
		localVars->accept (_visitor);

	if (sharedVars != 0 )
		sharedVars->accept (_visitor);

	if (main != 0)
		main->accept (_visitor);

   _visitor.visit (this);	
}

ast::node* ast::program::clone () 
{ 
  return new ast::program (*this); 
}

ast::program::~program ()
{
	if (localVars != 0)
		delete localVars;
	if (sharedVars != 0 )
		delete sharedVars;
	if (main != 0)
		delete main;
}

/* operator<< */

namespace ast {
  ostream& operator<< (ostream& s, const ast::node& n)
  {
    return n.print (s);
  }
}

/* visitor stuff */

ast::visitor::visitor () { }

ast::visitor::~visitor () { }
