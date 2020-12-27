#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "basic_funcs.h"

char *
update_image_filename_to_write(char * image_filename_to_write){
    
    char * temp_str_filtername = (char *)malloc(DEF_MEM);
    temp_str_filtername[0] = 0;
    int i = 2, j = 0;
    
    //get filtername
    while (i < 5){
        temp_str_filtername[j++] = my_options.stdin_filters_arr[my_options.stdin_filter_count-1][i];
        i++;
    }
    temp_str_filtername[j] = '\0';
    
    image_filename_to_write = get_substr(image_filename_to_write, '.', 1);
    
    strncat(image_filename_to_write, "_", DEF_MEM - strlen("_") - 1);
    strncat(image_filename_to_write, temp_str_filtername, DEF_MEM - strlen(temp_str_filtername) - 1);
    strncat(image_filename_to_write, ".xpm", DEF_MEM - strlen(".xpm") - 1);
    
    free(temp_str_filtername);
    return image_filename_to_write;
}

int
seperate_tokens_by_whitespace(char * buffer, char * token_str_arr[]){

    int i = 0, j = 0, token_str_count = 0, space_count = 0, k = 0;
    //bool more_chars_left = false;

    while (buffer[i] != '\0'){
        //more_chars_left = false;
        while(buffer[i] != ' ' && buffer[i] != '\0'){

            if (j == 0){
                token_str_arr[token_str_count] = malloc(DEF_MEM);
                token_str_arr[token_str_count][0] = 0;
            }

            token_str_arr[token_str_count][j] = buffer[i];
            j++; i++; space_count = 0;
        }

        while (buffer[i] == ' ' /*&& more_chars_left*/){
            if (space_count == 0) {
                token_str_arr[token_str_count][j] = '\0';
                token_str_count++;
            }

            j = 0;           
            i++; space_count++;
        }
    }
    
    token_str_arr[token_str_count][j] = '\0';

    return token_str_count;
}

char *
get_hex_value_from_X11_color_file(char * full_lexed_color_str){

    int token_str_count = 0, i = 0;
    char * token_str_arr[DEF_MEM];
    bool hex_value_for_str_found = false;
    char * buffer = malloc(500);
    buffer[0] = 0;

    FILE * fp = fopen(COLORS_FILENAME, "r");
    if (!fp){
        fprintf(stderr, "The required file, %s, was not found.\n", COLORS_FILENAME);
        show_usage();
    }
    
    while (fgets(buffer, 500, fp) != NULL && !hex_value_for_str_found){

        //remove \n
        if (strlen(buffer) > 1){
            buffer[strlen(buffer) - 1] = '\0';
        }

        //get tokens from file (i.e.): black #000000
        token_str_count = seperate_tokens_by_whitespace(buffer, token_str_arr);

        // if first text color token matches, get hex value of last token
        if (strcmp(token_str_arr[0], full_lexed_color_str) == 0){
            hex_value_for_str_found = true;

            free(full_lexed_color_str);
            full_lexed_color_str = malloc(DEF_MEM);
            full_lexed_color_str[0] = 0;

            strncat(full_lexed_color_str, token_str_arr[token_str_count], DEF_MEM - strlen(token_str_arr[token_str_count]) - 1); 
        }

        for (i = token_str_count; i > -1; i--){
            free(token_str_arr[i]);
        }
    }
    
    free(buffer);
    fclose(fp);

    return full_lexed_color_str;
}

char *
remove_hash(char * full_lexed_color_str){

    int i = 1, j = 0;
    char * temp_str = malloc(DEF_MEM);
    temp_str[0] = 0;

    while(full_lexed_color_str[i] != '\0'){
        temp_str[j] = full_lexed_color_str[i];
        j++; i++;
    }
    temp_str[j] = '\0';

    free(full_lexed_color_str);
    full_lexed_color_str = malloc(DEF_MEM);
    full_lexed_color_str[0] = 0;
    strncat(full_lexed_color_str, temp_str, DEF_MEM - strlen(temp_str) - 1);
    free(temp_str);

    return full_lexed_color_str;
}

bool
check_extension(char * filename, char * extension){

    int extension_char_count = strlen(extension);
    int i = 0, j = 0, met_extension_char_count;
    bool extension_met = true;

    while (filename[i] != '\0'){
        
        if (i >= strlen(filename) - extension_char_count){
            if (filename[i] != extension[j]){
                extension_met = false;
            }
            j++;
        }
        i++;
    }

    return extension_met;
}
