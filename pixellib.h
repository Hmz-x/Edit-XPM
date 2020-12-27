#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "basic_funcs.h"

char * change_pixel(int * xy_pos_and_mode_ptr, char * pixel_str, char * image_filename_to_write, int * image_metadata_ptr);

void
copy_pixels_on_top_half(int * xy_pos_and_mode_ptr, int y_val, char * pixel_2d_arr[][y_val], char * image_filename_to_write, int * image_metadata_ptr){

    int i = 0, j = 0;
    for (j = 0; j < image_metadata_ptr[1] / 2; j++){
        for (i = -1; i < image_metadata_ptr[0] + 1; i++){

            xy_pos_and_mode_ptr[0] = i; // x pos
            xy_pos_and_mode_ptr[1] = j; // y pos
            
            if (i > -1 && i < image_metadata_ptr[0] - 1){
                change_pixel(xy_pos_and_mode_ptr, pixel_2d_arr[i][j], image_filename_to_write, image_metadata_ptr);
            } else {
                change_pixel(xy_pos_and_mode_ptr, "  ", image_filename_to_write, image_metadata_ptr);
            }
        }
    }
}

void
copy_pixels_on_bottom_half(int * xy_pos_and_mode_ptr, int y_val, char * pixel_2d_arr[][y_val], char * image_filename_to_write, int * image_metadata_ptr){

    int i = 0, j = 0;
    for (j = image_metadata_ptr[1] / 2; j > -1; j--){
        for (i = -1; i < image_metadata_ptr[0] + 1; i++){

            xy_pos_and_mode_ptr[0] = i; // x pos
            xy_pos_and_mode_ptr[1] = j; // y pos
            
            if (i > -1 && i < image_metadata_ptr[0] - 1){
                change_pixel(xy_pos_and_mode_ptr, pixel_2d_arr[i][j], image_filename_to_write, image_metadata_ptr);
            } else {
                change_pixel(xy_pos_and_mode_ptr, "  ", image_filename_to_write, image_metadata_ptr);
            }
        }
    }
}

void
copy_pixels_on_left_half(int * xy_pos_and_mode_ptr, int y_val, char * pixel_2d_arr[][y_val], char * image_filename_to_write, int * image_metadata_ptr){

    int i = 0, j = 0;
    for (j = 0; j < image_metadata_ptr[1]; j++){
        for (i = -1; i < (image_metadata_ptr[0] + 1) / 2; i++){

            xy_pos_and_mode_ptr[0] = i; // x pos
            xy_pos_and_mode_ptr[1] = j; // y pos
            
            if (i > -1 && i < image_metadata_ptr[0] - 1){
                change_pixel(xy_pos_and_mode_ptr, pixel_2d_arr[i][j], image_filename_to_write, image_metadata_ptr);
            } else {
                change_pixel(xy_pos_and_mode_ptr, "  ", image_filename_to_write, image_metadata_ptr);
            }

            xy_pos_and_mode_ptr[0] = image_metadata_ptr[0] + 1 - i; // x pos
            //xy_pos_and_mode_ptr[1] = image_metadata_ptr[1] - j; // y pos

            if (i > -1 && i < image_metadata_ptr[0] - 1){
                change_pixel(xy_pos_and_mode_ptr, pixel_2d_arr[i][j], image_filename_to_write, image_metadata_ptr);
            } else {
                change_pixel(xy_pos_and_mode_ptr, "  ", image_filename_to_write, image_metadata_ptr);
            }

        }
        /*
        for (i = image_metadata_ptr[0] + 1; i > image_metadata_ptr[0] / 2 - 1; i--){

            xy_pos_and_mode_ptr[0] = i; // x pos
            xy_pos_and_mode_ptr[1] = j; // y pos
            
            if (i > -1 && i < image_metadata_ptr[0] - 1){
                change_pixel(xy_pos_and_mode_ptr, pixel_2d_arr[i][j], image_filename_to_write, image_metadata_ptr);
            } else {
                change_pixel(xy_pos_and_mode_ptr, "  ", image_filename_to_write, image_metadata_ptr);
            }
        }
        */
    }
}

void
copy_pixels_on_right_half(int * xy_pos_and_mode_ptr, int y_val, char * pixel_2d_arr[][y_val], char * image_filename_to_write, int * image_metadata_ptr){

    int i = 0, j = 0;
    for (j = 0; j < image_metadata_ptr[1]; j++){
        for (i = image_metadata_ptr[0] + 1 / 2; i < image_metadata_ptr[0] + 1; i++){

            xy_pos_and_mode_ptr[0] = i; // x pos
            xy_pos_and_mode_ptr[1] = j; // y pos
            
            if (i > -1 && i < image_metadata_ptr[0] - 1){
                change_pixel(xy_pos_and_mode_ptr, pixel_2d_arr[i][j], image_filename_to_write, image_metadata_ptr);
            } else {
                change_pixel(xy_pos_and_mode_ptr, "  ", image_filename_to_write, image_metadata_ptr);
            }

        }
    }
}

void
apply_pixel_filter(char flip_option, int * xy_pos_and_mode_ptr, int y_val, char * pixel_2d_arr[][y_val], char * image_filename_to_write, int * image_metadata_ptr){

    //printf("flip_option: %c\n", flip_option);

    int i = 0, j = 0;//, x = 0, y = 0;
    xy_pos_and_mode_ptr[2] = 1; // change mode to write pixel

    if (flip_option == 't'){
        copy_pixels_on_top_half(xy_pos_and_mode_ptr, image_metadata_ptr[1], pixel_2d_arr, image_filename_to_write, image_metadata_ptr);
        copy_pixels_on_bottom_half(xy_pos_and_mode_ptr, image_metadata_ptr[1], pixel_2d_arr, image_filename_to_write, image_metadata_ptr);

        FILE * fp = fopen(image_filename_to_write, "a");
        fputs("};\n", fp);
        fclose(fp);
    } else if (flip_option == 'b'){  
        copy_pixels_on_bottom_half(xy_pos_and_mode_ptr, image_metadata_ptr[1], pixel_2d_arr, image_filename_to_write, image_metadata_ptr);
        copy_pixels_on_top_half(xy_pos_and_mode_ptr, image_metadata_ptr[1], pixel_2d_arr, image_filename_to_write, image_metadata_ptr);

        FILE * fp = fopen(image_filename_to_write, "a");
        fputs("};\n", fp);
        fclose(fp);
    } else if (flip_option == 'l'){  
        copy_pixels_on_left_half(xy_pos_and_mode_ptr, image_metadata_ptr[1], pixel_2d_arr, image_filename_to_write, image_metadata_ptr);
        //copy_pixels_on_right_half(xy_pos_and_mode_ptr, image_metadata_ptr[1], pixel_2d_arr, image_filename_to_write, image_metadata_ptr);
    }
    
}

char *
init_pixel_copy(char * image_filename_to_write){

    int i = 0, line_count = 0;
    bool line_met = false;
    char * rgb_str = NULL;
    char * buffer = malloc(500);
    buffer[0] = 0;

    FILE * fp_read = fopen(image_filename_to_write, "r");
    image_filename_to_write = update_image_filename_to_write(image_filename_to_write);
    FILE * fp_write = fopen(image_filename_to_write, "w");
    
    while (fgets(buffer, 500, fp_read) != NULL){

        fputs(buffer, fp_write);        
        if (strcmp(buffer, "/* pixels */\n") == 0) { break; }
    }

    fclose(fp_read);
    fclose(fp_write);
    free(buffer);

    return image_filename_to_write;
}

void
write_pixel(int * xy_pos_and_mode_ptr, char * pixel_str, char * image_filename_to_write, int * image_metadata_ptr){

    bool condition_met = false;
    FILE * fp = fopen(image_filename_to_write, "a");

    if (xy_pos_and_mode_ptr[0] == -1 && xy_pos_and_mode_ptr[1] != image_metadata_ptr[1]){
        fputs("\"", fp); // start line 
    } else if (xy_pos_and_mode_ptr[0] == image_metadata_ptr[0] && xy_pos_and_mode_ptr[1] != image_metadata_ptr[1] - 1){
        fputs("\",\n", fp); // end all lines except last
    } else if (xy_pos_and_mode_ptr[0] == image_metadata_ptr[0] && xy_pos_and_mode_ptr[1] == image_metadata_ptr[1] - 1){
        fputs("\"\n};\n", fp); // end last line 
    } else if (xy_pos_and_mode_ptr[0] == -1 && xy_pos_and_mode_ptr[1] == image_metadata_ptr[1]){
        //fputs("", fp); // end array
    } else {
        fputs(pixel_str, fp);
    }

    fclose(fp);
}

char *
change_pixel(int * xy_pos_and_mode_ptr, char * pixel_str, char * image_filename_to_write, int * image_metadata_ptr){

    if (xy_pos_and_mode_ptr[2] == 0){
        image_filename_to_write = init_pixel_copy(image_filename_to_write);
    } else if (xy_pos_and_mode_ptr[2] == 1){
        write_pixel(xy_pos_and_mode_ptr, pixel_str, image_filename_to_write, image_metadata_ptr);
    }

    return image_filename_to_write;
}

void
get_pixel(int y_val, char * pixel_2d_arr[][y_val], char * image_filename_to_write, int * image_metadata_ptr){
    
    int i = 0, j = 0,k = 0, line_count = 0;
    bool pixel_section_begun = false;
    char * pixel_str = (char *)malloc(image_metadata_ptr[3] + 1);
    char * buffer = (char *)malloc(4096);
    char * no_extra_char_str = (char *)malloc(4096);
    pixel_str[0] = 0;
    no_extra_char_str[0] = 0;
    buffer[0] = 0;

    FILE * fp = fopen(image_filename_to_write, "r");
    while (fgets(buffer, 4096, fp) != NULL){
        if (pixel_section_begun){
            
            if (strlen(buffer) > 1){
                buffer[strlen(buffer) - 1] = '\0';
            }
            if (strcmp(buffer, "};") == 0) { break; }
            
            //remove " and ",
            for (i = 0, j = 0; i < strlen(buffer); i++){
                if (i != 0 && i != strlen(buffer) - 2 && i != strlen(buffer) - 1){
                    no_extra_char_str[j] = buffer[i];
                    j++;
                }
            }
            no_extra_char_str[j] = '\0';
            //printf("A%sA\n", no_extra_char_str);
            
            i = 0; j = 0, k = 0;
            while (no_extra_char_str[i] != '\0'){
                pixel_str[j] = no_extra_char_str[i];
                j++; i++;

                if (j == image_metadata_ptr[3]){
                    pixel_str[j] = '\0';

                    pixel_2d_arr[k][line_count] = malloc(DEF_MEM);
                    pixel_2d_arr[k][line_count][0] = 0;

                    strncat(pixel_2d_arr[k][line_count], pixel_str, DEF_MEM - strlen(pixel_str) - 1);
                    pixel_2d_arr[k][line_count][j] = '\0';

                    j = 0;
                    k++;
                }
            }

            line_count++;
            k = 0;
        }
        if (strcmp(buffer, "/* pixels */\n") == 0) { pixel_section_begun = true; }
    }

    fclose(fp);
    free(no_extra_char_str);
    free(buffer);
    free(pixel_str);
}

int
tokenize_pixel_and_rgb_str(char * buffer, char * pixel_and_rgb_arr[], int rgb_char_length){

    static int pixel_and_rgb_arr_index = 0;
    pixel_and_rgb_arr[pixel_and_rgb_arr_index] = (char *)malloc(DEF_MEM);
    pixel_and_rgb_arr[pixel_and_rgb_arr_index+1] = (char *)malloc(DEF_MEM);
    pixel_and_rgb_arr[pixel_and_rgb_arr_index][0] = 0;
    pixel_and_rgb_arr[pixel_and_rgb_arr_index+1][0] = 0;

    int i = 0, j = 0;
    char * no_extra_char_str = (char *)malloc(DEF_MEM);
    no_extra_char_str[0] = 0;

    //remove " and ",
    for (; i < strlen(buffer); i++){
        if (i != 0 && i != strlen(buffer) - 2 && i != strlen(buffer) - 1){
            no_extra_char_str[j] = buffer[i];
            j++;
        }
    }
    no_extra_char_str[j] = '\0';        

    i = 0; j = 0;
    for (; i < rgb_char_length; i++){
        pixel_and_rgb_arr[pixel_and_rgb_arr_index][i] = no_extra_char_str[i];
    }
    pixel_and_rgb_arr[pixel_and_rgb_arr_index][2] = '\0';

    i = 0; j = 0;
    while (no_extra_char_str[i] != '\0'){
        if (i > 2 + rgb_char_length){
            pixel_and_rgb_arr[pixel_and_rgb_arr_index+1][j++] = no_extra_char_str[i];
        }
        i++;
    }
    pixel_and_rgb_arr[pixel_and_rgb_arr_index+1][j] = '\0';

    free(no_extra_char_str);

    /*
    // determine if hex color or text color: #000000 or black
    if (full_lexed_color_str[0] != '#'){
        full_lexed_color_str = get_hex_value_from_X11_color_file(full_lexed_color_str);
    }
    printf("%s - %s\n", pixel_and_rgb_arr[pixel_and_rgb_arr_index],pixel_and_rgb_arr[pixel_and_rgb_arr_index+1]);
    */

    pixel_and_rgb_arr_index += 2;
    return pixel_and_rgb_arr_index;
}

char **
get_pixel_and_rgb_arr(char * image_filename_to_write, char * pixel_and_rgb_arr[], int rgb_char_length){

    //bool line_met = false;
    int line_count = 0;
    char * buffer = (char *)malloc(500);
    buffer[0] = 0;

    FILE * fp = fopen(image_filename_to_write, "r");

    while (fgets(buffer, 500, fp) != NULL){
        if (strcmp(buffer, "/* pixels */\n") == 0) { break; }

        if (line_count > 3){
            if (strlen(buffer) > 1){
                buffer[strlen(buffer) - 1] = '\0';
            }

            tokenize_pixel_and_rgb_str(buffer, pixel_and_rgb_arr, rgb_char_length);
        }

        line_count++;
    }

    fclose(fp);
    free(buffer);

    return pixel_and_rgb_arr;
}