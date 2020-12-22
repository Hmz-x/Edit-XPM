#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

int * read_metadata_from_xpm_file(int * image_metadata_ptr);
char * read_rgb_from_file(char * buffer, int rgb_char_length);
void write_rgb_to_file(char * buffer, char * rgb_str, FILE * fp_write, int rgb_char_length);
char *
process_image(char * image_filename_to_write){

    int i = 0, line_count = 0;
    bool line_met = false;
    char * rgb_str = NULL;
    char * buffer = malloc(500);
    buffer[0] = 0;

    int * image_metadata_ptr = (int *)malloc(sizeof(unsigned int) * 4);
    //image_metadata_ptr[0]: height [1]: width [2]: num of colors [3]: num of chars per color 
    image_metadata_ptr = read_metadata_from_xpm_file(image_metadata_ptr);
    //printf("[0]: %d [1]: %d [2]: %d [3]: %d\n", image_metadata_ptr[0], image_metadata_ptr[1], image_metadata_ptr[2], image_metadata_ptr[3]);
    
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
            //rgb_str = apply_filter();
            write_rgb_to_file(buffer, rgb_str, fp_write, image_metadata_ptr[3]);

            free(rgb_str);
        }          

        line_count++;
    }

    
    fclose(fp_read);
    fclose(fp_write);
    
    free(image_metadata_ptr);
    free(buffer);

    return image_filename_to_write;
}

