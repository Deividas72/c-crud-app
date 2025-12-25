#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *valid_options[] = { "help", "h", "exit", "q", "clear", "c"};
int valid_opts_len = sizeof(valid_options) / sizeof(valid_options[0]);

void remove_whitespace(char *str);
int validate_input(char *input);
void clear_screen();

int main(int argc, char *argv[]) {
    char *PROGRAM_NAME = argv[0];

    if (argc > 1) {
        fprintf(stderr, "Error: too many args\nUsage: %s\n", PROGRAM_NAME);
        return 1;
    }

    clear_screen();
    printf("Commands:\n  help (h) - show available commands\n  exit (q) - quit the program\n\n");

    char input[1024];

    while(1) {
        printf("%s # ", PROGRAM_NAME);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("fgets() failed");
            return 2;
        }
        input[strcspn(input, "\n")] = '\0';
        remove_whitespace(input);
        if (validate_input(input) != 0) continue;
    }
    
    return 0;
}

int validate_input(char *input) {
    int input_length = strlen(input);
    if (input_length == 0) {
        printf("empty string was passed");
        return 1;
    }
    
    for (int i = 0; i < input_length; i++) {
        if (input[i] == ' ') {
            input[i] = '\0';     
            break;
        }
    }
    
    bool is_valid_input = false;
    for (int i = 0; i < valid_opts_len; i++) {
        if (strcmp(valid_options[i], input) == 0) {
            is_valid_input = true;
            break;
        } 
    }       

    if (!is_valid_input) {
        input[10] = '\0';
        printf("%s: command not found\n", input);
        return 1;
    }

    return 0;
}

void remove_whitespace(char *str) {
    int i = 0, j = 0;
    bool prev_char_space = false;

    for (;str[i] != '\0'; i++) {
        if (str[i] == ' ' || str[i] == '\t') {
            if (!prev_char_space) {
                str[j] = ' ';
                j++;
            }
            prev_char_space = true;
        }
        else {
            str[j] = str[i];
            j++;
            prev_char_space = false;
        }
    }

    str[j] = '\0';

    if (strlen(str) > 1) {
       if (str[0] == ' ') {
           int k = 1;
           int l = 0;
           for (; str[k] != '\0'; k++) {
                str[l] = str[k];
                l++;
           }
           str[l] = '\0';
       }
       if (str[strlen(str) - 1] == ' ') {
           str[strlen(str) - 1] = '\0';
       }
    }

    if (strlen(str) == 1 && str[0] == ' ') str[0] = '\0';
}

void clear_screen(){
    system("clear");
}
