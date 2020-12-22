#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "tasks.h"
#include "basic_funcs.h"
#include "process_image.h"
#include "xpm_file_io.h"

extern int yylex();
extern FILE * yyin;
extern int yylineno;
extern char * yytext;


void
write_to_file_and_lex(int argc, char ** argv){

    int i = 1;
    FILE * fp = fopen(ARGS_IN_STREAM_FILENAME, "w");

    for (;i < argc; i++){
        fputs(argv[i], fp);
        fputs("\n", fp);
    }

    // assign fp's stream to lexer
    fclose(fp);
    yyin = fopen("args_input_stream_for_lexer.txt", "r");
    yylex();
    
    #ifdef DEBUG_STDIN
    printf("\nconvert_enabled: %d\n", my_options.convert_enabled);
    printf("stdin_file: %s\n", my_options.stdin_file);
    printf("stdout_file: %s\n\n", my_options.stdout_file);
    printf("stdin_filter_count: %d\n", my_options.stdin_filter_count);
    i = 0;
    while (i < my_options.stdin_filter_count){
        printf("stdin_filters_arr[%d]: %s\n", i, my_options.stdin_filters_arr[i]);
        i++;
    }
    printf("\n");
    #endif
    
}

int
main(int argc, char ** argv){
    
    // init struct elements
    my_options.convert_enabled = false;
    my_options.stdin_filter_count = 0;

    // init local vars
    char * image_filename_to_write = malloc(DEF_MEM);
    image_filename_to_write[0] = 0;
    int i = 0;

    // check for user error
    if (argc < MIN_ARGC_COUNT){
        show_usage();
    }

    // get arguments
    write_to_file_and_lex(argc, argv);

    // convert if necessary, if not make sure it was a xpm file
    if (my_options.convert_enabled) {
       // convert_to_xpm();
    } 

    // begin processing
    strncat(image_filename_to_write, my_options.stdin_file, DEF_MEM - strlen(my_options.stdin_file) - 1);

    i = 0;
    while (i < my_options.stdin_filter_count){
        image_filename_to_write = process_image(image_filename_to_write);
        i++;
    }
    
    //convert_to_non_xpm(image_filename_to_write);


    //cleanup
    free(image_filename_to_write);
    cleanup_struct_element();
    return 0;
}