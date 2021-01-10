%{
/*
Available filters: --inv=0-100, --sat=-100-100, --shd=-100-100, --swp=r/g/b{3}
*/
#define STDIN_FILTERS_MODE 3
#define CONVERT_ENABLED_MODE 2
#define OUTFILE_MODE 1
#define INFILE_MODE 0

void accesses_struct_element(char * yytext, int mode);
void check_for_errors(char * yytext, int mode);
%}

long_option_and_val ^(-{2})([a-zA-Z]+)=(-?[0-9]+)$
long_option_no_val ^(-{2})([a-zA-Z]+)$
short_option ^(-{1})([a-zA-Z]+)$
filename .+
%%

"--inv="([0-9]|[1-9][0-9]|100)     { accesses_struct_element(yytext, STDIN_FILTERS_MODE); }
"--sat="(-?)([0-9]|[1-9][0-9]|100) { accesses_struct_element(yytext, STDIN_FILTERS_MODE); }
"--shd="(-?)([0-9]|[1-9][0-9]|100) { accesses_struct_element(yytext, STDIN_FILTERS_MODE); }
"--swp="[rgb]{3}                   { accesses_struct_element(yytext, STDIN_FILTERS_MODE); } 
"--flp="[tblr]                     { accesses_struct_element(yytext, STDIN_FILTERS_MODE); }

-c                               { check_for_errors(yytext, CONVERT_ENABLED_MODE); accesses_struct_element(yytext, CONVERT_ENABLED_MODE); }
-o=.+                            { check_for_errors(yytext, OUTFILE_MODE); accesses_struct_element(yytext, OUTFILE_MODE); }
.+                               { check_for_errors(yytext, INFILE_MODE); accesses_struct_element(yytext, INFILE_MODE); }

\n                               ;

%%

int yywrap(void){ return -1; }