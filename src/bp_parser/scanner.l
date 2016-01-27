/*******************************************************************\

File   : scanner.l

\*******************************************************************/

%{
#include <cstdlib>
#include <string>
#include <map>

#include "context.h"
#include "ast.h"
#include "parser.hpp"

#undef yywrap
#define yywrap() 1

#define yyterminate() return yy::parser::token::EOL
%}

%option noyywrap nounput batch debug

id    [a-zA-Z][a-zA-Z_0-9]*
blank [ \t]

%{
#define YY_USER_INIT initialize_keyword_table();
#define YY_USER_ACTION yylloc->columns (yyleng);

typedef std::map<std::string, yy::parser::token::yytokentype> keyword_table;

static keyword_table keywords;
static void initialize_keyword_table ();
%}

%%

%{
  yylloc->step ();
%}

{blank}+				{ yylloc->step (); }
[\n]+					{ yylloc->lines (yyleng); yylloc->step (); }

"&"					{ return yy::parser::token::CONJUNCTION; }
"|"					{ return yy::parser::token::DISJUNCTION; }
"!"					{ return yy::parser::token::NEGATION;  }
"("					{ return yy::parser::token::LPAREN;  }
")"					{ return yy::parser::token::RPAREN; }
":"					{ return yy::parser::token::COLON; }
";"					{ return yy::parser::token::SEMICOLON; }
","					{ return yy::parser::token::COMMA; }
":="					{ return yy::parser::token::ASSIGN; }
"if"					{ return yy::parser::token::IF; }
"then"					{ return yy::parser::token::THEN; }
"fi"					{ return yy::parser::token::FI; }
"assert"				{ return yy::parser::token::ASSERT; }
"assume"				{ return yy::parser::token::ASSUME; }
"goto"					{ return yy::parser::token::GOTO; }
"skip"					{ return yy::parser::token::SKIP; }
"constrain"				{ return yy::parser::token::CONSTRAINT; }
"start_thread goto"			{ return yy::parser::token::START_THREAD; }
"end_thread"				{ return yy::parser::token::END_THREAD; }
"atomic_begin"				{ return yy::parser::token::ATOMIC_BEGIN; }
"atomic_end"				{ return yy::parser::token::ATOMIC_END; }
"void main()"				{ return yy::parser::token::MAIN; }
"main()"				{ return yy::parser::token::MAIN; }
"begin"					{ return yy::parser::token::BEGIN_; }
"end"					{ return yy::parser::token::END_; }
"return"					{ return yy::parser::token::BP_RETURN; }

[01\*]					{ 
							if (yytext[0] == '0')
							yylval->node = 
							 new ast::constant (ast::constant::FALSE);
							else if (yytext[0] == '1')
							yylval->node = 
							 new ast::constant (ast::constant::TRUE);
							else
							yylval->node = 
							 new ast::constant (ast::constant::STAR);

							return yy::parser::token::CONSTANT; 
						}
{id}					{ 
							keyword_table::const_iterator k_it;
							if ((k_it = keywords.find (std::string (yytext))) != keywords.end())
							return k_it->second;
							yylval->node = new ast::identifier (std::string (yytext)); 
							return yy::parser::token::IDENTIFIER; 
						}
"$"						{ return yy::parser::token::EOL; }

%%

static void initialize_keyword_table ()
{
/*
  keywords["shared"] = yy::parser::token::SHARED;
  keywords["local"] = yy::parser::token::LOCAL;
*/
  keywords["decl"] = yy::parser::token::DECL;
}

void context::scan_begin ()
{
  yy_flex_debug = trace_scanning;
  if (!(yyin = fopen (file.c_str (), "r")))
    error (std::string ("Failed to open file ") + file);
}

void context::scan_end ()
{
  fclose (yyin);
}
