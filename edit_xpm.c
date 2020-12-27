#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#include "tasks.h"
#include "basic_funcs.h"
#include "process_image.h"
#include "xpm_file_io.h"
#include "pixellib.h"

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
    printf("stdin_filters_arr[0]: %s\n", my_options.stdin_filters_arr[0]);
    printf("\n");
    #endif

}

void
convert_to_non_xpm(char * image_filename_to_write){

    char * image_viewer_system_str = NULL;
    char * system_str = malloc(DEF_MEM * 4);
    system_str[0] = 0;
    
    strncat(system_str, "convert ", DEF_MEM * 4 - strlen("convert ") - 1);
    strncat(system_str, image_filename_to_write, DEF_MEM * 4 - strlen(image_filename_to_write) - 1);
    strncat(system_str, " ", DEF_MEM * 4 - strlen(" ") - 1);
    
    image_filename_to_write = get_substr(image_filename_to_write,'.', 1);

    strncat(system_str, image_filename_to_write, DEF_MEM * 4 - strlen(image_filename_to_write) - 1);
    strncat(system_str, CONVERT_OUT_FORMAT, DEF_MEM * 4 - strlen(CONVERT_OUT_FORMAT) - 1);
    
    if (my_options.convert_enabled){
        system(system_str);

        //check if xdg-open is installed
        FILE * fp = fopen("/usr/bin/xdg-open", "r");
        if (fp) {
            image_viewer_system_str = malloc(DEF_MEM * 4);
            image_viewer_system_str[0] = 0;

            strncat(image_viewer_system_str, "xdg-open", DEF_MEM * 4 - strlen("xdg-open") - 1);
            strncat(image_viewer_system_str, " ", DEF_MEM * 4 - strlen(" ") - 1);
            strncat(image_viewer_system_str, image_filename_to_write, DEF_MEM * 4 - strlen(image_filename_to_write) - 1);
            strncat(image_viewer_system_str, CONVERT_OUT_FORMAT, DEF_MEM * 4 - strlen(CONVERT_OUT_FORMAT) - 1);
            
            system(image_viewer_system_str);
            free(image_viewer_system_str);
        }
        fclose(fp);
    }

    free(system_str);
}

void
convert_to_xpm(){

    char * system_str = NULL;

    //check if convert is installed
    FILE * fp = fopen("/usr/bin/convert", "r");
    if (fp == NULL) { fprintf(stderr, "\'convert\' is not installed in /usr/bin\n"); show_usage(); }
    fclose(fp); 

    system_str = malloc(DEF_MEM * 2);
    system_str[0] = 0;
    strncat(system_str, "convert ", DEF_MEM * 2 - strlen("convert ") - 1);

    strncat(system_str, my_options.stdin_file, DEF_MEM * 2 - strlen(my_options.stdin_file) - 1);
    strncat(system_str, " ", DEF_MEM * 2 - strlen(" ") - 1);

    my_options.stdin_file = get_substr(my_options.stdin_file, '.', 1);
    strncat(my_options.stdin_file, ".xpm", DEF_MEM * 2 - strlen(".xpm") - 1);
    strncat(system_str, my_options.stdin_file, DEF_MEM * 2 - strlen(my_options.stdin_file) - 1);
    
    system(system_str);
    free(system_str);
}

int
main(int argc, char ** argv){
    
    // init struct elements
    my_options.pixel_filter_applied = false;
    my_options.convert_enabled = false;
    my_options.stdin_filter_count = 0;

    // init local vars
    int * xy_pos_and_mode_ptr = malloc(sizeof(unsigned int) * 3);
    int * image_metadata_ptr = malloc(sizeof(unsigned int) * 4);
    //char * pixel_and_rgb_arr[DEF_MEM * 16];
    char * image_filename_to_write = malloc(DEF_MEM);
    image_filename_to_write[0] = 0;
    int i = 0, j = 0;
    srand(time(0));
    xy_pos_and_mode_ptr[0] = 0; // x pos
    xy_pos_and_mode_ptr[1] = 0; // y pos
    xy_pos_and_mode_ptr[2] = 0; // mode: 0 init, 1: write pixel, 2: write pixel and finish

    // check for user error
    if (argc < MIN_ARGC_COUNT){
        show_usage();
    }

    // get arguments
    write_to_file_and_lex(argc, argv);

    // convert if necessary, if not make sure it was a xpm file
    if (my_options.convert_enabled) {
       convert_to_xpm();
    } else if (!(check_extension(my_options.stdin_file, ".xpm"))) {
        fprintf(stderr, "Attempting to read a non-xpm file. Please use " \
        "the \'-c\' flag to convert to xpm first.\n");
        show_usage();
    }
    
    // assign stdin_file to image_filename_to_write & get metadata from file
    strncat(image_filename_to_write, my_options.stdin_file, DEF_MEM - strlen(my_options.stdin_file) - 1);
    image_metadata_ptr = read_metadata_from_xpm_file(image_metadata_ptr);
    //printf("[0]: %d [1]: %d [2]: %d [3]: %d\n", image_metadata_ptr[0], image_metadata_ptr[1], image_metadata_ptr[2], image_metadata_ptr[3]);

    // begin processing
    char * pixel_2d_arr[image_metadata_ptr[0]][image_metadata_ptr[1]];
    get_pixel(image_metadata_ptr[1], pixel_2d_arr, image_filename_to_write, image_metadata_ptr);

    //get_pixel_and_rgb_arr(image_filename_to_write, pixel_and_rgb_arr, image_metadata_ptr[3]);
    if (my_options.pixel_filter_applied){
        image_filename_to_write = change_pixel(xy_pos_and_mode_ptr, NULL, image_filename_to_write, image_metadata_ptr);
    }

    i = 0;  
    while (i < my_options.stdin_filter_count){

        if (strncmp(my_options.stdin_filters_arr[i], "--flp", strlen("--flp")) == 0){

            apply_pixel_filter(my_options.stdin_filters_arr[i][strlen(my_options.stdin_filters_arr[i]) - 1], xy_pos_and_mode_ptr, image_metadata_ptr[1], pixel_2d_arr, image_filename_to_write, image_metadata_ptr);

        }
        // apply color filters 
        else{
            image_filename_to_write = process_image(image_filename_to_write, i, image_metadata_ptr);
        }
        i++;
    }
    
    convert_to_non_xpm(image_filename_to_write);

    //cleanup
    cleanup_struct_element();
    free(image_filename_to_write);

    for (i = image_metadata_ptr[1] - 1; i > -1; i--){
        for (j = image_metadata_ptr[0] - 2; j > -1; j--){
            free(pixel_2d_arr[j][i]);
        }
    }

    free(image_metadata_ptr);
    free(xy_pos_and_mode_ptr);

    return 0;
}