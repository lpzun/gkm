/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++

   Copyright (C) 2002, 2003, 2004, 2005, 2006 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


#include "parser.hpp"

/* User implementation prologue.  */
#line 36 "parser.y"

#include "context.h"
#include "ast.h"
#line 41 "parser.y"

	void yyerror (const char *s)
	{
	  fprintf (stderr, "%s\n", s);
	}


/* Line 317 of lalr1.cc.  */
#line 53 "parser.cpp"

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG							\
  for (bool yydebugcond_ = yydebug_; yydebugcond_; yydebugcond_ = false)	\
    (*yycdebug_)

/* Enable debugging if requested.  */
#if YYDEBUG

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab

namespace yy
{
#if YYERROR_VERBOSE

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              /* Fall through.  */
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

#endif

  /// Build a parser object.
  parser::parser (context& ctx_yyarg)
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
      ctx (ctx_yyarg)
  {
  }

  parser::~parser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  parser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
        case 3: /* "\"constant\"" */
#line 49 "parser.y"
	{ debug_stream() << (*(yyvaluep->node)); };
#line 183 "parser.cpp"
	break;
      case 4: /* "\"variable or label\"" */
#line 49 "parser.y"
	{ debug_stream() << (*(yyvaluep->node)); };
#line 188 "parser.cpp"
	break;
       default:
	  break;
      }
  }


  void
  parser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif /* ! YYDEBUG */

  void
  parser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
        case 3: /* "\"constant\"" */
#line 51 "parser.y"
	{ delete (yyvaluep->node);  };
#line 223 "parser.cpp"
	break;
      case 4: /* "\"variable or label\"" */
#line 51 "parser.y"
	{ delete (yyvaluep->node);  };
#line 228 "parser.cpp"
	break;
      case 35: /* "identifiers" */
#line 51 "parser.y"
	{ delete (yyvaluep->node);  };
#line 233 "parser.cpp"
	break;
      case 36: /* "code" */
#line 51 "parser.y"
	{ delete (yyvaluep->node);  };
#line 238 "parser.cpp"
	break;
      case 37: /* "statement" */
#line 51 "parser.y"
	{ delete (yyvaluep->node);  };
#line 243 "parser.cpp"
	break;
      case 38: /* "assignment" */
#line 51 "parser.y"
	{ delete (yyvaluep->node);  };
#line 248 "parser.cpp"
	break;
      case 39: /* "expressions" */
#line 51 "parser.y"
	{ delete (yyvaluep->node);  };
#line 253 "parser.cpp"
	break;
      case 40: /* "expr" */
#line 51 "parser.y"
	{ delete (yyvaluep->node);  };
#line 258 "parser.cpp"
	break;

	default:
	  break;
      }
  }

  void
  parser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }


  int
  parser::parse ()
  {
    /// Look-ahead and look-ahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the look-ahead.
    semantic_type yylval;
    /// Location of the look-ahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location yyerror_range[2];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* User initialization code.  */
    #line 22 "parser.y"
{
  // Initialize the location
  yylloc.begin.filename = yylloc.end.filename = &ctx.file;
}
  /* Line 555 of yacc.c.  */
#line 340 "parser.cpp"
    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;
    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without look-ahead.  */
    yyn = yypact_[yystate];
    if (yyn == yypact_ninf_)
      goto yydefault;

    /* Read a look-ahead token.  */
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = yylex (&yylval, &yylloc, ctx);
      }


    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yyn == 0 || yyn == yytable_ninf_)
	goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Accept?  */
    if (yyn == yyfinal_)
      goto yyacceptlab;

    /* Shift the look-ahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted unless it is eof.  */
    if (yychar != yyeof_)
      yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	  case 2:
#line 128 "parser.y"
    { ctx.result = new ast::program ((ast::assignment*)(yysemantic_stack_[(6) - (1)].node),(ast::assignment*)(yysemantic_stack_[(6) - (4)].node), (ast::code*)(yysemantic_stack_[(6) - (5)].node)); ;}
    break;

  case 3:
#line 134 "parser.y"
    { ctx.result = new ast::program ((ast::assignment*)(yysemantic_stack_[(5) - (1)].node),0, (ast::code*)(yysemantic_stack_[(5) - (4)].node)); ;}
    break;

  case 4:
#line 140 "parser.y"
    { ctx.result = new ast::program (0,(ast::assignment*)(yysemantic_stack_[(5) - (3)].node), (ast::code*)(yysemantic_stack_[(5) - (4)].node)); ;}
    break;

  case 5:
#line 143 "parser.y"
    {(yyval.node) = (yysemantic_stack_[(1) - (1)].node);;}
    break;

  case 6:
#line 146 "parser.y"
    {
				std::list<ast::identifier*>::const_iterator itVar;
				std::list<ast::node*>::const_iterator itValue;

				ast::identifiers* vars = ((ast::assignment*)(yysemantic_stack_[(2) - (1)].node))->getVars();
				ast::expressions* exprs = ((ast::assignment*)(yysemantic_stack_[(2) - (1)].node))->getExprs();
				itVar = vars->begin();
				itValue = exprs->begin();
				for(;itVar!= vars->end() && itValue!= exprs->end(); )
				{
					((ast::assignment*)(yysemantic_stack_[(2) - (2)].node))->getVars()->push_front(*itVar++);
					((ast::assignment*)(yysemantic_stack_[(2) - (2)].node))->getExprs()->push_front(*itValue++);
				}
				(yyval.node) = (yysemantic_stack_[(2) - (2)].node);
			;}
    break;

  case 7:
#line 162 "parser.y"
    {(yyval.node) = (yysemantic_stack_[(3) - (2)].node);;}
    break;

  case 8:
#line 165 "parser.y"
    { 
				ast::expressions* expr_result = new ast::expressions((ast::node*)new ast::constant(ast::constant::STAR));
				for(unsigned i = 1; i < ((ast::identifiers*)(yysemantic_stack_[(3) - (2)].node))->size(); i++)
					expr_result->push_front((ast::node*)new ast::constant(ast::constant::STAR));
				(yyval.node) = new ast::assignment((yysemantic_stack_[(3) - (2)].node), expr_result);
			;}
    break;

  case 9:
#line 173 "parser.y"
    { (yyval.node) = new ast::identifiers ((ast::identifier*)(yysemantic_stack_[(1) - (1)].node)); ;}
    break;

  case 10:
#line 176 "parser.y"
    { (yyval.node) = ((ast::identifiers*)(yysemantic_stack_[(3) - (3)].node))->push_front((ast::identifier*)(yysemantic_stack_[(3) - (1)].node)); ;}
    break;

  case 11:
#line 179 "parser.y"
    { (yyval.node) = new ast::code ((ast::statement*)(yysemantic_stack_[(2) - (1)].node)); ;}
    break;

  case 12:
#line 182 "parser.y"
    { (yyval.node) = ((ast::code*)(yysemantic_stack_[(3) - (3)].node))->push_front ((ast::statement*)(yysemantic_stack_[(3) - (1)].node)); ;}
    break;

  case 13:
#line 185 "parser.y"
    { 	
				ast::code* result = (ast::code*)(yysemantic_stack_[(3) - (3)].node);
				result->front()->labels.push_front ((ast::node*)(yysemantic_stack_[(3) - (1)].node));
				(yyval.node) = result;
			;}
    break;

  case 14:
#line 192 "parser.y"
    { (yyval.node) = new ast::statement ((ast::assignment*)(yysemantic_stack_[(1) - (1)].node));;}
    break;

  case 15:
#line 195 "parser.y"
    { (yyval.node) = new ast::statement (new ast::jump ((ast::identifiers*)(yysemantic_stack_[(2) - (2)].node)));;}
    break;

  case 16:
#line 198 "parser.y"
    { (yyval.node) = new ast::statement (new ast::start_thread ((ast::identifiers*)(yysemantic_stack_[(2) - (2)].node)));;}
    break;

  case 17:
#line 201 "parser.y"
    { (yyval.node) = new ast::statement (new ast::end_thread());;}
    break;

  case 18:
#line 204 "parser.y"
    { (yyval.node) = new ast::statement (new ast::bp_assume ((ast::expressions*)(yysemantic_stack_[(2) - (2)].node)));;}
    break;

  case 19:
#line 207 "parser.y"
    { (yyval.node) = new ast::statement (new ast::skip());;}
    break;

  case 20:
#line 210 "parser.y"
    { (yyval.node) = new ast::statement (new ast::bp_return());;}
    break;

  case 21:
#line 213 "parser.y"
    { (yyval.node) = new ast::statement (new ast::atomic_begin());;}
    break;

  case 22:
#line 216 "parser.y"
    { (yyval.node) = new ast::statement (new ast::atomic_end());;}
    break;

  case 23:
#line 219 "parser.y"
    { (yyval.node) = new ast::statement (new ast::if_begin ((ast::node*)(yysemantic_stack_[(3) - (2)].node)));;}
    break;

  case 24:
#line 222 "parser.y"
    { (yyval.node) = new ast::statement (new ast::if_end());;}
    break;

  case 25:
#line 225 "parser.y"
    { (yyval.node) = new ast::statement (new ast::bp_assert((ast::node*)(yysemantic_stack_[(2) - (2)].node)));;}
    break;

  case 26:
#line 229 "parser.y"
    { 
				if( (((ast::identifiers*)(yysemantic_stack_[(3) - (1)].node))->size()) == (((ast::expressions*)(yysemantic_stack_[(3) - (3)].node))->size()) ) 
					(yyval.node) = new ast::assignment((yysemantic_stack_[(3) - (1)].node), (yysemantic_stack_[(3) - (3)].node));
				else
				{
					yyerror("incorrect assignment is detected in ");
					std::cout << *(yysemantic_stack_[(3) - (1)].node) << " = " << *(yysemantic_stack_[(3) - (3)].node) << ";" << std::endl;
					YYERROR; 
				}
			;}
    break;

  case 27:
#line 241 "parser.y"
    { 
				if( (((ast::identifiers*)(yysemantic_stack_[(5) - (1)].node))->size()) == (((ast::expressions*)(yysemantic_stack_[(5) - (3)].node))->size()) ) 
					(yyval.node) = new ast::assignment((yysemantic_stack_[(5) - (1)].node), (yysemantic_stack_[(5) - (3)].node), (yysemantic_stack_[(5) - (5)].node));
				else
				{
					yyerror("incorrect assignment is detected in ");
					std::cout << *(yysemantic_stack_[(5) - (1)].node) << " = " << *(yysemantic_stack_[(5) - (3)].node) << *(yysemantic_stack_[(5) - (5)].node) << ";" << std::endl;
					YYERROR; 
				}
			;}
    break;

  case 28:
#line 253 "parser.y"
    { (yyval.node) = new ast::expressions((ast::node*)(yysemantic_stack_[(1) - (1)].node));;}
    break;

  case 29:
#line 254 "parser.y"
    { (yyval.node) = ((ast::expressions*)(yysemantic_stack_[(3) - (3)].node))->push_front((ast::node*)(yysemantic_stack_[(3) - (1)].node));;}
    break;

  case 30:
#line 257 "parser.y"
    { (yyval.node) = new ast::conj((yysemantic_stack_[(3) - (1)].node), (yysemantic_stack_[(3) - (3)].node));  ;}
    break;

  case 31:
#line 258 "parser.y"
    { (yyval.node) = new ast::disj ((yysemantic_stack_[(3) - (1)].node), (yysemantic_stack_[(3) - (3)].node));  ;}
    break;

  case 32:
#line 259 "parser.y"
    { (yyval.node) = new ast::neg ((yysemantic_stack_[(2) - (2)].node)); ;}
    break;

  case 33:
#line 260 "parser.y"
    { (yyval.node) = (yysemantic_stack_[(3) - (2)].node); ;}
    break;

  case 34:
#line 261 "parser.y"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;

  case 35:
#line 262 "parser.y"
    { (yyval.node) = (yysemantic_stack_[(1) - (1)].node); ;}
    break;


    /* Line 675 of lalr1.cc.  */
#line 668 "parser.cpp"
	default: break;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[0] = yylloc;
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

	if (yychar <= yyeof_)
	  {
	  /* Return failure if at end of input.  */
	  if (yychar == yyeof_)
	    YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
	    yychar = yyempty_;
	  }
      }

    /* Else will try to reuse look-ahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[0] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (yyn != yypact_ninf_)
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	YYABORT;

	yyerror_range[0] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    if (yyn == yyfinal_)
      goto yyacceptlab;

    yyerror_range[1] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the look-ahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		   &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyeof_ && yychar != yyempty_)
      yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval, &yylloc);

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystate_stack_.height () != 1)
      {
	yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
	yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (int yystate, int tok)
  {
    std::string res;
    YYUSE (yystate);
#if YYERROR_VERBOSE
    int yyn = yypact_[yystate];
    if (yypact_ninf_ < yyn && yyn <= yylast_)
      {
	/* Start YYX at -YYN if negative to avoid negative indexes in
	   YYCHECK.  */
	int yyxbegin = yyn < 0 ? -yyn : 0;

	/* Stay within bounds of both yycheck and yytname.  */
	int yychecklim = yylast_ - yyn + 1;
	int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
	int count = 0;
	for (int x = yyxbegin; x < yyxend; ++x)
	  if (yycheck_[x + yyn] == x && x != yyterror_)
	    ++count;

	// FIXME: This method of building the message is not compatible
	// with internationalization.  It should work like yacc.c does it.
	// That is, first build a string that looks like this:
	// "syntax error, unexpected %s or %s or %s"
	// Then, invoke YY_ on this string.
	// Finally, use the string as a format to output
	// yytname_[tok], etc.
	// Until this gets fixed, this message appears in English only.
	res = "syntax error, unexpected ";
	res += yytnamerr_ (yytname_[tok]);
	if (count < 5)
	  {
	    count = 0;
	    for (int x = yyxbegin; x < yyxend; ++x)
	      if (yycheck_[x + yyn] == x && x != yyterror_)
		{
		  res += (!count++) ? ", expecting " : " or ";
		  res += yytnamerr_ (yytname_[x]);
		}
	  }
      }
    else
#endif
      res = YY_("syntax error");
    return res;
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char parser::yypact_ninf_ = -23;
  const signed char
  parser::yypact_[] =
  {
         1,    14,    38,    49,    27,    22,    22,    45,    -3,    46,
     -23,    37,   -23,    44,    38,   -23,    19,   -23,    25,    24,
      38,   -23,    19,   -23,    19,    19,    38,   -23,   -23,   -23,
     -23,    56,    50,    64,   -23,   -23,   -23,   -23,    19,    19,
      51,     9,    44,    52,    44,   -23,     3,     6,     6,   -23,
     -23,    44,   -23,    62,    19,    19,    19,    19,    54,   -23,
     -23,   -23,   -23,   -23,     6,   -23,    71,   -23,   -23
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  parser::yydefact_[] =
  {
         0,     0,     0,     0,     0,     5,     0,     9,     0,     0,
       1,     0,     6,     0,     0,     8,     0,     7,     0,     9,
       0,    19,     0,    24,     0,     0,     0,    17,    20,    21,
      22,     0,     0,     0,    14,    10,    35,    34,     0,     0,
      26,    28,     0,     0,     0,    15,     0,    25,    18,    16,
       4,    11,    32,     0,     0,     0,     0,     0,     0,     3,
      13,    23,    12,    33,    27,    30,    31,    29,     2
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  parser::yypgoto_[] =
  {
       -23,   -23,     0,   -23,    -1,   -14,   -23,    78,    26,   -22
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  parser::yydefgoto_[] =
  {
        -1,     3,     4,     5,    31,    32,    33,    34,    40,    41
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char parser::yytable_ninf_ = -1;
  const unsigned char
  parser::yytable_[] =
  {
        46,     8,    47,    48,    43,    12,    13,    15,    55,    56,
      16,    55,    56,    35,    55,    56,    52,    53,    42,    45,
      61,    57,    36,    37,     1,    49,    38,    39,    58,    19,
      60,     2,    64,    65,    66,    44,    14,    62,     6,    20,
      21,    22,     7,    23,    24,    25,    26,    27,    19,    10,
      11,    28,     2,    29,    30,     2,    17,    14,    20,    21,
      22,    18,    23,    24,    25,    26,    27,    55,    56,    16,
      28,    63,    29,    30,    51,    50,    55,    59,    54,    68,
       9,     0,     0,    67
  };

  /* YYCHECK.  */
  const signed char
  parser::yycheck_[] =
  {
        22,     2,    24,    25,    18,     5,     6,    10,     5,     6,
      13,     5,     6,    14,     5,     6,    38,    39,    18,    20,
      17,    12,     3,     4,    23,    26,     7,     8,    42,     4,
      44,    30,    54,    55,    56,    11,    12,    51,    24,    14,
      15,    16,     4,    18,    19,    20,    21,    22,     4,     0,
      23,    26,    30,    28,    29,    30,    10,    12,    14,    15,
      16,    24,    18,    19,    20,    21,    22,     5,     6,    13,
      26,     9,    28,    29,    10,    25,     5,    25,    27,    25,
       2,    -1,    -1,    57
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  parser::yystos_[] =
  {
         0,    23,    30,    32,    33,    34,    24,     4,    35,    38,
       0,    23,    33,    33,    12,    10,    13,    10,    24,     4,
      14,    15,    16,    18,    19,    20,    21,    22,    26,    28,
      29,    35,    36,    37,    38,    35,     3,     4,     7,     8,
      39,    40,    33,    36,    11,    35,    40,    40,    40,    35,
      25,    10,    40,    40,    27,     5,     6,    12,    36,    25,
      36,    17,    36,     9,    40,    40,    40,    39,    25
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  parser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  parser::yyr1_[] =
  {
         0,    31,    32,    32,    32,    33,    33,    34,    34,    35,
      35,    36,    36,    36,    37,    37,    37,    37,    37,    37,
      37,    37,    37,    37,    37,    37,    38,    38,    39,    39,
      40,    40,    40,    40,    40,    40
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  parser::yyr2_[] =
  {
         0,     2,     6,     5,     5,     1,     2,     3,     3,     1,
       3,     2,     3,     3,     1,     2,     2,     1,     2,     1,
       1,     1,     1,     3,     1,     2,     3,     5,     1,     3,
       3,     3,     2,     3,     1,     1
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const parser::yytname_[] =
  {
    "\"end of file\"", "error", "$undefined", "\"constant\"",
  "\"variable or label\"", "\"conjunction\"", "\"disjunction\"",
  "\"negation\"", "\"left parenthesis\"", "\"right parenthesis\"",
  "\"semicolon\"", "\"colon\"", "\"comma\"", "\"assignment\"",
  "\"jump statement\"", "\"skip statement\"", "\"if statement begin\"",
  "\"then\"", "\"if statement end\"", "\"assertion\"", "\"assume\"",
  "\"start_thread\"", "\"start_end\"", "\"main method\"", "\"begin\"",
  "\"end\"", "\"bp_return\"", "\"constraint\"", "\"atomic_begin\"",
  "\"atomic_end\"", "\"declarations\"", "$accept", "program",
  "declarations", "declaration", "identifiers", "code", "statement",
  "assignment", "expressions", "expr", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const parser::rhs_number_type
  parser::yyrhs_[] =
  {
        32,     0,    -1,    33,    23,    24,    33,    36,    25,    -1,
      33,    23,    24,    36,    25,    -1,    23,    24,    33,    36,
      25,    -1,    34,    -1,    34,    33,    -1,    30,    38,    10,
      -1,    30,    35,    10,    -1,     4,    -1,     4,    12,    35,
      -1,    37,    10,    -1,    37,    10,    36,    -1,     4,    11,
      36,    -1,    38,    -1,    14,    35,    -1,    21,    35,    -1,
      22,    -1,    20,    40,    -1,    15,    -1,    26,    -1,    28,
      -1,    29,    -1,    16,    40,    17,    -1,    18,    -1,    19,
      40,    -1,    35,    13,    39,    -1,    35,    13,    39,    27,
      40,    -1,    40,    -1,    40,    12,    39,    -1,    40,     5,
      40,    -1,    40,     6,    40,    -1,     7,    40,    -1,     8,
      40,     9,    -1,     4,    -1,     3,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  parser::yyprhs_[] =
  {
         0,     0,     3,    10,    16,    22,    24,    27,    31,    35,
      37,    41,    44,    48,    52,    54,    57,    60,    62,    65,
      67,    69,    71,    73,    77,    79,    82,    86,    92,    94,
      98,   102,   106,   109,   113,   115
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  parser::yyrline_[] =
  {
         0,   123,   123,   130,   136,   143,   145,   162,   164,   172,
     175,   178,   181,   184,   191,   194,   197,   200,   203,   206,
     209,   212,   215,   218,   221,   224,   228,   240,   253,   254,
     257,   258,   259,   260,   261,   262
  };

  // Print the state stack on the debug stream.
  void
  parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "), ";
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  parser::token_number_type
  parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int parser::yyeof_ = 0;
  const int parser::yylast_ = 83;
  const int parser::yynnts_ = 10;
  const int parser::yyempty_ = -2;
  const int parser::yyfinal_ = 10;
  const int parser::yyterror_ = 1;
  const int parser::yyerrcode_ = 256;
  const int parser::yyntokens_ = 31;

  const unsigned int parser::yyuser_token_number_max_ = 285;
  const parser::token_number_type parser::yyundef_token_ = 2;

} // namespace yy

#line 264 "parser.y"


void
yy::parser::error (const yy::parser::location_type& location,
                   const std::string& message)
{
  ctx.error (location, message);
}




