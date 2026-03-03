%{
#include <stdlib.h>
#include "y.tab.h"

void yyerror(char*);
%}

%%

[0-9]+        {
                yylval.i = atoi(yytext);
                return INT;
              }
[\-+\*/]      {
                yylval.c = yytext[0];
                return OPE;
              }

"("           return '(';
")"           return ')';

[ \n\t]+ ;


%%
