#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define USER_MAX_LENGTH 21 // actually it stores 20 chars, but 21st is needed for \0
#define PASSWORD_MAX_LENGTH 21 // same here
#define MAX_USERS 100

struct user_scheme {
    char name[USER_MAX_LENGTH];
    char password[PASSWORD_MAX_LENGTH];
};

struct app_scheme {
    struct user_scheme users[MAX_USERS];
    int user_count;
} APP;



char *valid_options[] = { "help", "h", "exit", "q", "clear", "c", "create"};
int valid_opts_len = sizeof(valid_options) / sizeof(valid_options[0]);


int do_command(char *input);
void remove_whitespace(char *str);
int validate_input(char *input);
void clear_screen(void);
int create_user(char *input);
int read_input(char *input, int input_size);

int main(int argc, char *argv[]) {
    char *program_name = argv[0];

    if (argc > 1) {
        fprintf(stderr, "Error: too many args\nUsage: %s\n", program_name);
        return 1;
    }

    APP.user_count = 0;
    for (int i = 0; i < MAX_USERS; i++) {
        APP.users[i].name[0] = '\0'; 
        APP.users[i].password[0] = '\0'; 
    } 

    clear_screen();
    printf("Commands:\n  help (h) - show available commands\n  exit (q) - quit the program\n\n");

    int input_size = 100;
    char input[input_size];

    while(1) {
        printf("APP # ");
        if (read_input(input, input_size) != 0) continue;
        if (validate_input(input) != 0) continue;
        int command_result = do_command(input);
        if (command_result == 1) break;
    }
    
    return 0;
}

int read_input(char *input, int input_size) {
    if (fgets(input, input_size, stdin) == NULL) {
        perror("fgets() failed");
        return 1;
    }

    if (!strchr(input, '\n')) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;

        fprintf(stderr, "Error: input too long (max %d chars)\n", input_size - 1);
        return 1;
    }

    input[strcspn(input, "\n")] = '\0';
    remove_whitespace(input);
    return 0;
}

int do_command(char *input) {
     if (strcmp(input, "clear") == 0 || strcmp(input, "c") == 0) {
        clear_screen();
        return 0;
     }
     if (strcmp(input, "help") == 0 || strcmp(input, "h") == 0) {
        printf("printing help...\n");
        return 0;
     }
     if (strcmp(input, "exit") == 0 || strcmp(input, "q") == 0) {
        printf("Program received exit command\n");
        return 1;
     }
     if (strcmp(input, "create") == 0) {
        create_user(input);
        return 0;
     }
     return 1;
}

int create_user(char *input) {
   printf("Username: ");
   if (read_input(input, USER_MAX_LENGTH) != 0) return 1; 
   
   if (strlen(input) == 0) {
       fprintf(stderr, "Error: empty input\n");
       return 1;
   }

   for (int i = 0; i < MAX_USERS; i++) {
        if (strcmp(APP.users[i].name, input) == 0) {
            fprintf(stderr, "Username is taken\n");
            return 1;
        }
   }
   
   char temp_username[USER_MAX_LENGTH];
   strcpy(temp_username, input);

   printf("Password: ");
   if (read_input(input, PASSWORD_MAX_LENGTH) != 0) return 1; 
   if (strlen(input) == 0) {
       fprintf(stderr, "Error: empty input\n");
       return 1;
   }
   char temp_password[PASSWORD_MAX_LENGTH];
   strcpy(temp_password, input);

   printf("Repeat password: ");
   if (read_input(input, PASSWORD_MAX_LENGTH) != 0) return 1; 
   if (strlen(input) == 0) {
       fprintf(stderr, "Error: empty input\n");
       return 1;
   }
   
   if (strcmp(temp_password, input) != 0) {
       fprintf(stderr, "Error: passwords don't match\n");
       return 1;
   }

   strcpy(APP.users[APP.user_count].name, temp_username); 
   strcpy(APP.users[APP.user_count].password, temp_password); 
   APP.user_count++; 
   return 0;
}


int validate_input(char *input) {
    int input_length = strlen(input);
    if (input_length == 0) {
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

void clear_screen(void){
    system("clear");
}
