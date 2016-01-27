#include "context.h"
#include "parser.hpp"

context::context (): trace_scanning (false), trace_parsing (false)
{
}

context::~context ()
{
}

bool context::parse (const string &_file)
{
  int success;
  file = _file;
  scan_begin ();
  yy::parser parser (*this);
  success = parser.parse ();
  scan_end ();
  return (success==0);
}

void context::error (const yy::location& location, 
                     const string& message)
{
  cerr << location << ": " << message << endl;
}

void context::error (const string& message)
{
  cerr << message << endl;
}
