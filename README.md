# Edit-XPM
Edit XPM files via CLI. Uses no GUI, designed to for allowing use within scripts/programs.

Compiling and Execution Instructions:
lex cli_arg_lexer.l
gcc lex.yy.c edit_xpm.c -o ed-xpm
./ed-xpm --inv=100 <xpm_file>
