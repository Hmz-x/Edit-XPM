

int *
divide_into_tokens(char * buffer, int * image_metadata_ptr){

    int i = 0, j = 0, metadata_met_count = 0;
    char * only_num_str = malloc(DEF_MEM);
    char * temp_str_token = malloc(DEF_MEM);
    only_num_str[0] = 0;
    temp_str_token[0] = 0;

    //remove " and ",
    for (; i < strlen(buffer); i++){
        if (i != 0 && i != strlen(buffer) - 2 && i != strlen(buffer) - 1){
            only_num_str[j] = buffer[i];
            j++;
        }
    }
    only_num_str[j] = '\0';

    // cut out spaces & tokenize into numbers
    i = 0, j = 0;
    while (only_num_str[i] != '\0'){

        while(only_num_str[i] != ' ' && only_num_str[i] != '\0'){
            temp_str_token[j] = only_num_str[i];
            j++; i++;
        }

        while (only_num_str[i] == ' '){
            temp_str_token[j] = '\0';
            image_metadata_ptr[metadata_met_count++] = atoi(temp_str_token);

            free(temp_str_token);
            temp_str_token = malloc(DEF_MEM);
            temp_str_token[0] = 0;
            j = 0;

            i++;
        }

    }

    temp_str_token[j] = '\0';
    image_metadata_ptr[metadata_met_count++] = atoi(temp_str_token);

    free(only_num_str);
    free(temp_str_token);
    return image_metadata_ptr;
}

int *
read_metadata_from_xpm_file(int * image_metadata_ptr){

    int line_count = 0;
    char * temp_str = NULL;
    char * buffer = malloc(500);
    buffer[0] = 0;

    FILE * fp = fopen(my_options.stdin_file, "r");

    while (fgets(buffer, 500, fp) != NULL){
        
        if (line_count == 3){
            buffer[strlen(buffer) - 1] = '\0';
            image_metadata_ptr = divide_into_tokens(buffer, image_metadata_ptr);
        }

        line_count++;
    }

    fclose(fp);    
    free(buffer);

    return image_metadata_ptr;
}

char *
read_rgb_from_file(char * buffer, int rgb_char_length){

    int i = 0, j = 0;
    char * rgb_str = malloc(DEF_MEM);
    char * full_lexed_color_str = malloc(DEF_MEM);
    char * semi_lexed_str = malloc(DEF_MEM);
    rgb_str[0] = 0;
    full_lexed_color_str[0] = 0;
    semi_lexed_str[0] = 0;

    //remove " and ",
    for (; i < strlen(buffer); i++){
        if (i != 0 && i != strlen(buffer) - 2 && i != strlen(buffer) - 1){
            semi_lexed_str[j] = buffer[i];
            j++;
        }
    }
    semi_lexed_str[j] = '\0';

    //assign the color str to full_lexed_color_str, which starts before 2 + rgb_char_length chars
    i = 0; j = 0;
    while (semi_lexed_str[i] != '\0'){
        if (i > 2 + rgb_char_length){
            full_lexed_color_str[j++] = semi_lexed_str[i];
        }

        i++;
    }
    full_lexed_color_str[j] = '\0';
    free(semi_lexed_str);

    // determine if hex color or text color: #000000 or black
    if (full_lexed_color_str[0] != '#'){
        full_lexed_color_str = get_hex_value_from_X11_color_file(full_lexed_color_str);
    } 

    while (strlen(full_lexed_color_str) < 6){
        strcat(full_lexed_color_str, "0");
    }

    full_lexed_color_str = remove_hash(full_lexed_color_str);
    strncat(rgb_str, full_lexed_color_str, DEF_MEM - strlen(full_lexed_color_str) - 1);

    free(full_lexed_color_str);

    return rgb_str;
}

void
write_rgb_to_file(char * buffer, char * rgb_str, FILE * fp_write, int rgb_char_length){

    int i = 0;
    while (i < rgb_char_length + 4){
        fputc(buffer[i++], fp_write);
    }

    fputc('#', fp_write);
    fputs(rgb_str, fp_write);
    fputs("\",\n", fp_write);
}