#include <stdbool.h>

#define PRG_NAME "ed-xpm"
#ifndef BASIC_FUNCS
#define BASIC_FUNCS

//temp defines
#define SAT_VAL -20
#define SHD_VAL 0
//#define DEBUG_STDIN

#define DEF_MEM 128
#define EXTRA_SPACE_COUNT 1
#define MIN_ARGC_COUNT 3
#define AVAILABLE_FILTER_COUNT 3

#define CONVERT_OUT_FORMAT ".png"
#define COLORS_FILENAME "X11_colors.txt"
#define ARGS_IN_STREAM_FILENAME "args_input_stream_for_lexer.txt"

#define STDIN_FILTERS_MODE 3
#define CONVERT_ENABLED_MODE 2
#define OUTFILE_MODE 1
#define INFILE_MODE 0

void show_usage();

struct options {

    bool pixel_filter_applied;
    bool convert_enabled;
    char * stdin_file;
    char * stdout_file;
    int stdin_filter_count;
    char * stdin_filters_arr[];

} my_options;

char *
get_substr(char * str, char ch, int mode){

    //mode 0: get substr after char, 1: get before char

    bool char_met = false;
    int i = strlen(str) - 1, substr_start_index = 0, j = 0;
    char * temp_str = malloc(DEF_MEM);
    temp_str[0] = 0;

    while (i > -1){
        if (str[i] == ch){
            char_met = true;
            substr_start_index = i + 1;
        }
        i--;
    }

    //get substr after char
    if (mode == 0){
        i = substr_start_index;
        while (str[i] != '\0'){
            temp_str[j] = str[i];
            i++; j++;
        }
        temp_str[j] = '\0';
    }
    // get substr before char
    else{
        i = 0;
        while(i < substr_start_index - 1){
            temp_str[j] = str[i];
            i++; j++;
        }
        temp_str[j] = '\0';
    }
    
    return temp_str;
}

void
accesses_struct_element(char * yytext, int mode){
    /*
    static size_t str_mem = DEF_MEM;
    static size_t int_mem = DEF_MEM;
    */
    char * temp_str = NULL;
   
    if (mode == INFILE_MODE){
        my_options.stdin_file = malloc(DEF_MEM);
        my_options.stdin_file[0] = 0;
        strncat(my_options.stdin_file, yytext, DEF_MEM - strlen(yytext) - 1);
    } else if (mode == OUTFILE_MODE){

        my_options.stdout_file = malloc(DEF_MEM);
        my_options.stdout_file[0] = 0;

        //get everything after '='
        temp_str = get_substr(yytext, '=', 0);

        strncat(my_options.stdout_file, temp_str, DEF_MEM - strlen(temp_str) - 1);
        free(temp_str);

    } else if (mode == CONVERT_ENABLED_MODE){
        my_options.convert_enabled = true;
    }
    else{
        if (strncmp(yytext, "--flp", strlen("--flp")) == 0){
            my_options.pixel_filter_applied = true;
        }

        my_options.stdin_filters_arr[my_options.stdin_filter_count] = malloc(DEF_MEM);
        my_options.stdin_filters_arr[my_options.stdin_filter_count][0] = 0;

        strncat(my_options.stdin_filters_arr[my_options.stdin_filter_count], yytext, DEF_MEM - strlen(yytext) - 1);
        my_options.stdin_filter_count++;
    }
    
}

void
cleanup_struct_element(){

    int i = my_options.stdin_filter_count - 1;
    while (i > -1){
        free(my_options.stdin_filters_arr[i]);
        i--;
    }

    free(my_options.stdin_file);
    if (my_options.stdout_file){
        free(my_options.stdout_file);
    }
}

void
check_for_errors(char * yytext, int mode){

    static int stdin_file_count = 0;
    static int stdout_file_count = 0;
    static int convert_enabled_count = 0;
    int i = 0;

    // <filename>
    if (mode == INFILE_MODE){

        FILE * fp = fopen(yytext, "r");
        if (!fp) {
            fprintf(stderr, "File %s doesn't exist or is invalid input/option.\n", yytext);
            show_usage();
        }
        fclose(fp);

        // check if filename has extension
        if (strstr(yytext, ".") == NULL){
            fprintf(stderr, "Input filename must contain an extension.\n");
            show_usage();
        }

        stdin_file_count++;
        //invalid input or filename entered multiple times
        if (stdin_file_count > 1){
            fprintf(stderr, "Invalid input/option or attempt at accessing multiple files." \
            " Only one file can be accessed at a time.\n");
            show_usage();
        }
    }
    // -o=abc
    else if (mode == OUTFILE_MODE){

        // check if filename has extension
        if (strstr(yytext, ".") == NULL){
            fprintf(stderr, "Output filename must contain an extension.\n");
            show_usage();
        }

        stdout_file_count++;
        if (stdout_file_count > 1){ 
            fprintf(stderr, "There can be only one output file.\n");
            show_usage();
        }
    }
    // -c
    else{
        convert_enabled_count++;
        if (convert_enabled_count > 1){ 
            fprintf(stderr, "\'convert\' can only be used once.\n");
            show_usage();
        }
    }

}

void
show_usage(){
    fprintf(stderr, "\nusage: %s [OPTION] <FILE> <FILTER...> - Written by Hamza Kerem Mumcu (hamzamusic34@gmail.com)\n", PRG_NAME);
    
    fprintf(stderr, "-c: convert image file to .xpm format using \'convert\'\n");
    fprintf(stderr, "<FILE>: image file to be processed\n");
    fprintf(stderr, "<FILTER>: filter to apply to image (can use multiple back to back)\n");
    fprintf(stderr, "Available filters: \'--inv=<range>\' (inverted colors, range: 0-100), \'--sat=<range>\' (saturate, range:-100-100),\n");
    fprintf(stderr, "\'--flp=<side>\' (flip given side, side: t/b for top/bottom).\n");
    fprintf(stderr, "Filters can be chained back to back in order to apply processing one after the other.\n\n");

    fprintf(stderr, "Example usage:\n");
    fprintf(stderr, "%s --flp=t img.xpm                       # Flip top half of xpm image file\n", PRG_NAME);
    fprintf(stderr, "%s -c --inv=20 --sat=5 img.jpg           # Convert image to xpm, then apply inversion and saturation filter\n", PRG_NAME);
    exit(1);
}

#endif

