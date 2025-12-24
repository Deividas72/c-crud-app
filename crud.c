#include <stdio.h>
#include <string.h>

void remove_whitespace(char *str);

int main(int argc, char *argv[]) {
    if (argc > 1) {
        fprintf(stderr, "Error: too many args\nUsage: %s\n", argv[0]);
        return 1;
    }

    char input[1024];

    while(1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("fgets() failed");
            return 2;
        }
        input[strcspn(input, "\n")] = '\0';
        //printf("\"%s\"\n", input);
        //printf("length: %zu\n", strlen(input));
        remove_whitespace(input);
        //printf("\"%s\"\n", input);
        //printf("length: %zu\n", strlen(input));
        if (strlen(input) == 0) {
            printf("Enter an option. For option information type help or h.\n");
            continue;
        }
        printf("ok\n");

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

