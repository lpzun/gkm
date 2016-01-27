/*******************************************************************\

File   : boolean.y

\*******************************************************************/

%skeleton "lalr1.cc"
%define "parser_class_name" "parser"
%defines
%{
#include <string>
#include <list>
#include "ast.h"
class context;
%}

%parse-param { context& ctx }
%lex-param { context& ctx }

%locations
%initial-action
{
  // Initialize the location
  @$.begin.filename = @$.end.filename = &ctx.file;
};

%debug
%error-verbose

%union {
  ast::node* node;
}



%{
#include "context.h"
#include "ast.h"
%}

%{
	void yyerror (const char *s)
	{
	  fprintf (stderr, "%s\n", s);
	}
%}


%printer { debug_stream() << (*$$); } CONSTANT IDENTIFIER

%destructor { delete $$;  } CONSTANT IDENTIFIER expr assignment identifiers code expressions statement

%token <node> CONSTANT "constant"
%token <node> IDENTIFIER "variable or label"
%token CONJUNCTION "conjunction"
%token DISJUNCTION "disjunction"
%token NEGATION "negation"

%token LPAREN "left parenthesis"
%token RPAREN "right parenthesis"

%token SEMICOLON "semicolon"
%token COLON "colon"
%token COMMA "comma"
%token ASSIGN "assignment"
%token GOTO "jump statement"
%token SKIP "skip statement"
%token IF "if statement begin"
%token THEN "then"
%token FI "if statement end"
%token ASSERT "assertion"
%token ASSUME "assume"
%token START_THREAD "start_thread"
%token END_THREAD "start_end"

%token MAIN "main method"
%token BEGIN_ "begin"
%token END_ "end"
%token BP_RETURN "bp_return"

%token CONSTRAINT "constraint"
%token ATOMIC_BEGIN "atomic_begin"
%token ATOMIC_END "atomic_end"

/*
%token SHARED "shared declarations"
%token LOCAL "local declarations"
*/
%token DECL "declarations"
%token EOL 0 "end of file"

%left DISJUNCTION
%left CONJUNCTION
%nonassoc NEGATION

%type <node> expr
%type <node> declaration
%type <node> declarations
%type <node> assignment
%type <node> statement
%type <node> code
%type <node> expressions
%type <node> identifiers

%start program

%% /* Grammar rules and actions follow */

/*
program:	SHARED assignment SEMICOLON
			LOCAL  assignment SEMICOLON code 
			{ ctx.result = new ast::program ((ast::assignment*)$2,(ast::assignment*)$5, (ast::code*)$7); }

		|	SHARED assignment SEMICOLON
			code 
			{ ctx.result = new ast::program ((ast::assignment*)$2, 0 , (ast::code*)$4); }

		|	LOCAL assignment SEMICOLON
			code 
			{ ctx.result = new ast::program (0, (ast::assignment*)$2, (ast::code*)$4); }
*/

program:	declarations
		MAIN BEGIN_
		declarations
		code 
		END_
			{ ctx.result = new ast::program ((ast::assignment*)$1,(ast::assignment*)$4, (ast::code*)$5); }

		|	declarations
			MAIN BEGIN_
			code 
			END_
				{ ctx.result = new ast::program ((ast::assignment*)$1,0, (ast::code*)$4); }

		|	MAIN BEGIN_
			declarations
			code 
			END_
				{ ctx.result = new ast::program (0,(ast::assignment*)$3, (ast::code*)$4); }


declarations:	declaration {$$ = $1;}

		|	declaration declarations
			{
				std::list<ast::identifier*>::const_iterator itVar;
				std::list<ast::node*>::const_iterator itValue;

				ast::identifiers* vars = ((ast::assignment*)$1)->getVars();
				ast::expressions* exprs = ((ast::assignment*)$1)->getExprs();
				itVar = vars->begin();
				itValue = exprs->begin();
				for(;itVar!= vars->end() && itValue!= exprs->end(); )
				{
					((ast::assignment*)$2)->getVars()->push_front(*itVar++);
					((ast::assignment*)$2)->getExprs()->push_front(*itValue++);
				}
				$$ = $2;
			}

declaration:	DECL assignment SEMICOLON {$$ = $2;}
		
		|	DECL identifiers SEMICOLON
			{ 
				ast::expressions* expr_result = new ast::expressions((ast::node*)new ast::constant(ast::constant::STAR));
				for(unsigned i = 1; i < ((ast::identifiers*)$2)->size(); i++)
					expr_result->push_front((ast::node*)new ast::constant(ast::constant::STAR));
				$$ = new ast::assignment($2, expr_result);
			}

identifiers:	IDENTIFIER                 
			{ $$ = new ast::identifiers ((ast::identifier*)$1); }
		
		|	IDENTIFIER COMMA identifiers	
			{ $$ = ((ast::identifiers*)$3)->push_front((ast::identifier*)$1); }

code:		statement SEMICOLON              
			{ $$ = new ast::code ((ast::statement*)$1); } 
			
		|	statement SEMICOLON code		
			{ $$ = ((ast::code*)$3)->push_front ((ast::statement*)$1); }

		|	IDENTIFIER COLON code
			{ 	
				ast::code* result = (ast::code*)$3;
				result->front()->labels.push_front ((ast::node*)$1);
				$$ = result;
			}

statement:	assignment
			{ $$ = new ast::statement ((ast::assignment*)$1);}
	
		|	GOTO identifiers
			{ $$ = new ast::statement (new ast::jump ((ast::identifiers*)$2));}

		|	START_THREAD identifiers
			{ $$ = new ast::statement (new ast::start_thread ((ast::identifiers*)$2));}

		|	END_THREAD
			{ $$ = new ast::statement (new ast::end_thread());}

		|	ASSUME expr 
			{ $$ = new ast::statement (new ast::bp_assume ((ast::expressions*)$2));}

		|	SKIP
			{ $$ = new ast::statement (new ast::skip());}

		|	BP_RETURN
			{ $$ = new ast::statement (new ast::bp_return());}

		|	ATOMIC_BEGIN
			{ $$ = new ast::statement (new ast::atomic_begin());}

		|	ATOMIC_END
			{ $$ = new ast::statement (new ast::atomic_end());}

		|	IF expr THEN
			{ $$ = new ast::statement (new ast::if_begin ((ast::node*)$2));}

		|	FI
			{ $$ = new ast::statement (new ast::if_end());}

		|	ASSERT expr
			{ $$ = new ast::statement (new ast::bp_assert((ast::node*)$2));}


assignment: identifiers ASSIGN expressions	
			{ 
				if( (((ast::identifiers*)$1)->size()) == (((ast::expressions*)$3)->size()) ) 
					$$ = new ast::assignment($1, $3);
				else
				{
					yyerror("incorrect assignment is detected in ");
					std::cout << *$1 << " = " << *$3 << ";" << std::endl;
					YYERROR; 
				}
			}
		
		|	identifiers ASSIGN expressions CONSTRAINT expr
			{ 
				if( (((ast::identifiers*)$1)->size()) == (((ast::expressions*)$3)->size()) ) 
					$$ = new ast::assignment($1, $3, $5);
				else
				{
					yyerror("incorrect assignment is detected in ");
					std::cout << *$1 << " = " << *$3 << *$5 << ";" << std::endl;
					YYERROR; 
				}
			}


expressions:expr				{ $$ = new ast::expressions((ast::node*)$1);}
		|	expr COMMA expressions	{ $$ = ((ast::expressions*)$3)->push_front((ast::node*)$1);}


expr:		expr CONJUNCTION expr   	{ $$ = new ast::conj($1, $3);  }
		|	expr DISJUNCTION expr   { $$ = new ast::disj ($1, $3);  }
		|	NEGATION expr           { $$ = new ast::neg ($2); }
		|	LPAREN expr RPAREN      { $$ = $2; }
		|	IDENTIFIER              { $$ = $1; }
		|	CONSTANT                { $$ = $1; }
;
%%

void
yy::parser::error (const yy::parser::location_type& location,
                   const std::string& message)
{
  ctx.error (location, message);
}



