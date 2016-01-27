#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <string>
#include "ast.h"
#include "parser.hpp"
using namespace std;

namespace yy
{
  class location;
  class parser;
}
class context;

# define YY_DECL					\
  int yylex (yy::parser::semantic_type* yylval, \
             yy::location* yylloc, context& driver)
         
YY_DECL;


class context {
 public:
  context ();
  virtual ~context ();

  ast::node* result;

  string file;

  bool trace_scanning;
  bool trace_parsing;

  void scan_begin ();
  void scan_end ();

  bool parse (const string&);

  void error (const yy::location&, const string&);
  void error (const string&);
};

#endif
