/*
   IGraph library.
   Copyright (C) 2006-2012  Gabor Csardi <csardi.gabor@gmail.com>
   334 Harvard st, Cambridge, MA, 02138 USA

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc.,  51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301 USA

*/

%{

/*
   IGraph library.
   Copyright (C) 2006-2012  Gabor Csardi <csardi.gabor@gmail.com>
   334 Harvard st, Cambridge, MA, 02138 USA

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc.,  51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301 USA

*/

#include "igraph_types.h"
#include "igraph_memory.h"
#include "igraph_error.h"

#include "io/ncol-header.h"
#include "io/parsers/ncol-parser.h"
#include "io/parsers/ncol-lexer.h"
#include "io/parse_utils.h"
#include "internal/hacks.h"

#include <stdio.h>
#include <string.h>

int igraph_ncol_yyerror(YYLTYPE* locp,
                        igraph_i_ncol_parsedata_t *context,
                        const char *s);

#define scanner context->scanner
%}

%pure-parser
/* bison: do not remove the equals sign; macOS XCode ships with bison 2.3, which
 * needs the equals sign */
%name-prefix="igraph_ncol_yy"
%defines
%locations
%error-verbose
%parse-param { igraph_i_ncol_parsedata_t* context }
%lex-param { void *scanner }

%union {
  igraph_integer_t edgenum;
  igraph_real_t weightnum;
}

%type <edgenum>   edgeid
%type <weightnum> weight

%token ALNUM    "alphanumeric"
%token NEWLINE  "end of line"
%token END 0    "end of file" /* friendly name for $end */
%token ERROR

%%

input :    /* empty */
         | input NEWLINE
         | input edge
;

edge :   edgeid edgeid NEWLINE        {
           IGRAPH_YY_CHECK(igraph_vector_int_push_back(context->vector, $1));
           IGRAPH_YY_CHECK(igraph_vector_int_push_back(context->vector, $2));
           IGRAPH_YY_CHECK(igraph_vector_push_back(context->weights, 0.0));
       }
       | edgeid edgeid weight NEWLINE {
           IGRAPH_YY_CHECK(igraph_vector_int_push_back(context->vector, $1));
           IGRAPH_YY_CHECK(igraph_vector_int_push_back(context->vector, $2));
           IGRAPH_YY_CHECK(igraph_vector_push_back(context->weights, $3));
           context->has_weights = 1;
       }
;

edgeid : ALNUM  {
  igraph_integer_t trie_id;
  IGRAPH_YY_CHECK(igraph_trie_get_len(context->trie,
    igraph_ncol_yyget_text(scanner),
    igraph_ncol_yyget_leng(scanner),
    &trie_id
  ));
  $$ = trie_id;
};

weight : ALNUM  {
    igraph_real_t val;
    IGRAPH_YY_CHECK(igraph_i_parse_real(igraph_ncol_yyget_text(scanner),
                                        igraph_ncol_yyget_leng(scanner),
                                        &val));
    $$=val;
} ;

%%

int igraph_ncol_yyerror(YYLTYPE* locp,
            igraph_i_ncol_parsedata_t *context,
            const char *s) {
    snprintf(context->errmsg, sizeof(context->errmsg)/sizeof(char)-1,
            "Parse error in NCOL file, line %i (%s)",
            locp->first_line, s);
    return 0;
}
