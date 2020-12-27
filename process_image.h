#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

int * read_metadata_from_xpm_file(int * image_metadata_ptr);
char * read_rgb_from_file(char * buffer, int rgb_char_length);
void write_rgb_to_file(char * buffer, char * rgb_str, FILE * fp_write, int rgb_char_length);

int
get_int_filter_val(int stdin_filters_arr_index){

    char * temp_str = get_substr(my_options.stdin_filters_arr[stdin_filters_arr_index], '=', 0);
    int int_filter_val = atoi(temp_str);
    free(temp_str);
    
    return int_filter_val;
}

int
add_zeros(long int rgb_int_val){

    int i = 0, j = 0;
    char * temp_rgb_str2 = malloc(DEF_MEM);
    char * temp_rgb_str = malloc(DEF_MEM);
    temp_rgb_str[0] = 0;
    temp_rgb_str2[0] = 0;

    sprintf(temp_rgb_str, "%ld", rgb_int_val);
    /*
    if (strlen(temp_rgb_str) < 8){
        //printf("%s: ", temp_rgb_str);
        while(i < 8 - strlen(temp_rgb_str)){
            strcat(temp_rgb_str2, "0");
            i++;
        }

        strncat(temp_rgb_str2, temp_rgb_str, DEF_MEM - strlen(temp_rgb_str) - 1);
        //printf("%s - %s\n", temp_rgb_str, temp_rgb_str2);
        rgb_int_val = atoi(temp_rgb_str2);
        printf("%ld\n", rgb_int_val);
    }
    */
    while (strlen(temp_rgb_str) < 8){
        strcat(temp_rgb_str, "0");
    }
    if (strlen(temp_rgb_str) < 8){
        printf("%s\n",temp_rgb_str);
    }

    rgb_int_val = atoi(temp_rgb_str);

    free(temp_rgb_str);
    free(temp_rgb_str2);

    return rgb_int_val;
}

char *
apply_filter(char * rgb_val_str, int stdin_filters_arr_index){
    
    int i = 0, j = 0;
    unsigned long int rgb_int_val = 0x000000;
    int * rgb_decimal_ptr = malloc(sizeof(int) * 3);
    int int_filter_val = 0;
    char * str_filter_val = NULL;
    char * temp_rgb_str = NULL;
    char * rgb_index_str = NULL;

    //neccessary standarts
    while(rgb_val_str[i] != '\0'){
        rgb_val_str[i] = toupper(rgb_val_str[i]);
        i++;
    }
    /*
    while (strlen(rgb_val_str) < 6){
        strcat(rgb_val_str, "0");
    }
    */
    //printf("rgb_val_str: %s\n", rgb_val_str);

    // determine filter
    if (strncmp(my_options.stdin_filters_arr[stdin_filters_arr_index], "--inv", strlen("--inv")) == 0){

        int_filter_val = get_int_filter_val(stdin_filters_arr_index);
        rgb_int_val = strtol(rgb_val_str, NULL, 16); // convert hex to decimal
        
        //rgb_int_val = add_zeros(rgb_int_val); // add zeros to decimals if neccessary
        
        //only invert int_filter_val percent of the time
        //printf("int_filter_val: %d, rand_num: %d\n", int_filter_val, rand_num);
        if (int_filter_val > rand() % 100){
            rgb_int_val = 0xffffff - rgb_int_val;
        }
        
        sprintf(rgb_val_str, "%lx", rgb_int_val); // convert decimal to hex 
        while (strlen(rgb_val_str) < 6){
            strcat(rgb_val_str, "0");
        }

    } else if (strncmp(my_options.stdin_filters_arr[stdin_filters_arr_index], "--sat", strlen("--sat")) == 0){
        
        int_filter_val = get_int_filter_val(stdin_filters_arr_index);
        rgb_int_val = strtol(rgb_val_str, NULL, 16);
        rgb_int_val += int_filter_val; 
        
        if (rgb_int_val > 0xffffff){
            rgb_int_val = 0xffffff;
        } else if (rgb_int_val < 0x000000){
            rgb_int_val = 0x000000;
        }

        sprintf(rgb_val_str, "%lx", rgb_int_val); 
        while (strlen(rgb_val_str) < 6){
            strcat(rgb_val_str, "0");
        }

    } else if (strncmp(my_options.stdin_filters_arr[stdin_filters_arr_index], "--swp", strlen("--swp")) == 0){

        temp_rgb_str = malloc(DEF_MEM);
        rgb_index_str = malloc(DEF_MEM);
        rgb_index_str[0] = 0;
        temp_rgb_str[0] = 0;
        strncat(rgb_index_str, "rgb", DEF_MEM - strlen("rgb") - 1);
        strncat(temp_rgb_str, rgb_val_str, DEF_MEM - strlen(rgb_val_str) - 1);

        str_filter_val = get_substr(my_options.stdin_filters_arr[stdin_filters_arr_index], '=', 0);

        for (i = 0; i < 6; i+=2){
            //j = 0;
            if (str_filter_val[i/2] == rgb_index_str[0]){
                rgb_val_str[i/2] = temp_rgb_str[0];
                rgb_val_str[i/2+1] = temp_rgb_str[1];
            } else if (str_filter_val[i/2] == rgb_index_str[1]){
                rgb_val_str[i/2] = temp_rgb_str[2];
                rgb_val_str[i/2+1] = temp_rgb_str[3];
            } else if (str_filter_val[i/2] == rgb_index_str[2]){
                rgb_val_str[i/2] = temp_rgb_str[4];
                rgb_val_str[i/2+1] = temp_rgb_str[5];
            }
        }

        free(str_filter_val);
        free(rgb_index_str);
        free(temp_rgb_str);

    } else if (strncmp(my_options.stdin_filters_arr[stdin_filters_arr_index], "--shd", strlen("--shd")) == 0){
        /*
        rgb_decimal_ptr = convert_rgb_to_decimal(rgb_val_str, rgb_decimal_ptr);

        for (; i < 3; i++){
            rgb_decimal_ptr[i] += SHD_VAL;

            if (rgb_decimal_ptr[i] > 255){
                rgb_decimal_ptr[i] = 255;
            } else if (rgb_decimal_ptr[i] < 0){
                rgb_decimal_ptr[i] = 0;
            }
        }

        //convert decimal values back into hex & add together
        rgb_val_str = convert_decimal_rgb_to_hex_str(rgb_val_str, rgb_decimal_ptr);
        */
    }
    
    free(rgb_decimal_ptr);

    return rgb_val_str;
}

char *
process_image(char * image_filename_to_write, int stdin_filters_arr_index, int * image_metadata_ptr){

    int i = 0, line_count = 0;
    bool line_met = false;
    char * rgb_str = NULL;
    char * buffer = malloc(500);
    buffer[0] = 0;

    // read from last modified file
    FILE * fp_read = fopen(image_filename_to_write, "r");
    image_filename_to_write = update_image_filename_to_write(image_filename_to_write);
    
    // write to new file
    FILE * fp_write = fopen(image_filename_to_write, "w");
    
    while (fgets(buffer, 500, fp_read) != NULL){

        // write out first 4 lines to new processed img file
        if (line_count < 4) {
            fputs(buffer, fp_write);
        }
        
        //check if hex color sequence is over, if it is, write the pixels on to the file
        if (strcmp(buffer, "/* pixels */\n") == 0) { line_met = true; }
        if (line_met){ fputs(buffer, fp_write); }

        // read out color, if text convert to hex, then write inverted form to new file
        if (line_count > 3 && !line_met){

            //remove \n
            if (strlen(buffer) > 1){
                buffer[strlen(buffer) - 1] = '\0';
            }

            rgb_str = read_rgb_from_file(buffer, image_metadata_ptr[3]);
            rgb_str = apply_filter(rgb_str, stdin_filters_arr_index);
            write_rgb_to_file(buffer, rgb_str, fp_write, image_metadata_ptr[3]);

            free(rgb_str);
        }          

        line_count++;
    }
  
    fclose(fp_read);
    fclose(fp_write);
    
    free(buffer);
    
    return image_filename_to_write;
}

