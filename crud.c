// Add validation for read_input()

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *commands[] = { "help", "h", "exit", "q", "clear", "c", "add", "a"};
#define COMMAND_COUNT ( sizeof(commands) / sizeof(commands[0]) )
#define MAX_INPUT_SIZE 51
#define MAX_USERNAME_LENGTH 21
#define MAX_PASSWORD_LENGTH 21
#define MAX_USERS 100

struct user_scheme {
    char name[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

struct app_scheme {
    struct user_scheme users[MAX_USERS];
    int max_users;
    int user_count; 
    char input[MAX_INPUT_SIZE];
    int max_input_size;
    char *commands[COMMAND_COUNT];
    int command_count;
    int max_username_length;
    int max_password_length;
} APP;

void init(void);
void clear_screen(void);
void print_prompt(void);
int read_input(char *input, int input_size);
void remove_whitespace(char *str);
int check_if_input_is_a_command(void);
char get_command_code(void);
void create_user(void);

int main(int argc, char *argv[]) {
    if (argc > 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return 1;
    }
    
    init();

    while(1) {
        print_prompt();

        int input_read_successfully = read_input(APP.input, APP.max_input_size);
        if (input_read_successfully != 0) continue;

        int is_command = check_if_input_is_a_command();
        if (is_command != 0) continue;
        
        char command_code = get_command_code();
        switch (command_code) {
            case 'h':
                printf("printing help...\n");
                break;
            case 'q':
                printf("user issued exit command, exiting...\n");
                return 0;
            case 'c':
                clear_screen();
                break;
            case 'a':
                create_user();
                break;
            default:
                break;
        } 
    }
    
    return 0;
}

void init(void) {
    clear_screen();
    APP.user_count = 0;
    APP.command_count = COMMAND_COUNT;
    APP.max_input_size = MAX_INPUT_SIZE; 
    APP.max_username_length = MAX_USERNAME_LENGTH; 
    APP.max_password_length = MAX_PASSWORD_LENGTH; 
    APP.max_users = MAX_USERS; 

    for (int i = 0; i < APP.command_count; i++)
        APP.commands[i] = commands[i];

    for (int i = 0; i < MAX_USERS; i++) {
        APP.users[i].name[0] = '\0';
        APP.users[i].password[0] = '\0';
    }
    
    printf("Commands:\n  help (h) - show available commands\n  exit (q) - quit the program\n\n");
}

int read_input(char *input, int input_size) {
    if (fgets(input, input_size, stdin) == NULL) {
        perror("fgets() failed");
        return 1;
    }
    input[strcspn(input, "\n")] = '\0';
    remove_whitespace(input);
    
    return 0;
}

int check_if_input_is_a_command(void) {
    int input_length = strlen(APP.input); 
    int is_command = 0;

    if (input_length == 0)
        return 1;
    
    for (int i = 0; i < APP.command_count; i++) {
        if (strcmp(APP.commands[i], APP.input) == 0) {
            is_command = 1;
            break;
        } 
    }       

    if (!is_command) {
        printf("%s: command not found\n", APP.input);
        return 1;
    }

    return 0;
}

char get_command_code() {
    char command_code;
    
    if (strcmp(APP.input, "clear") == 0 || strcmp(APP.input, "c") == 0)
        command_code = 'c';
    else if (strcmp(APP.input, "exit") == 0 || strcmp(APP.input, "q") == 0) 
        command_code = 'q';
    else if (strcmp(APP.input, "help") == 0 || strcmp(APP.input, "h") == 0) 
        command_code = 'h';
    else if (strcmp(APP.input, "add") == 0 || strcmp(APP.input, "a") == 0) 
        command_code = 'a';
    
    return command_code;
}

void create_user() {
   printf("Username: ");

   if (read_input(APP.input, APP.max_username_length) != 0) return; 

   if (APP.input[0] == '\0') {
       fprintf(stderr, "Error: empty input\n");
       return;
   }

   for (int i = 0; i < APP.max_users; i++) {
        if (strcmp(APP.users[i].name, APP.input) == 0) {
            fprintf(stderr, "Error: username is taken\n");
            return;
        }
   }
   
   char temp_username[APP.max_username_length];
   strcpy(temp_username, APP.input);

   printf("Password: ");

   if (read_input(APP.input, APP.max_password_length) != 0) return;

   if (APP.input[0] == '\0') {
       fprintf(stderr, "Error: empty input\n");
       return;
   }

   char temp_password[APP.max_password_length];
   strcpy(temp_password, APP.input);

   printf("Repeat password: ");

   if (read_input(APP.input, APP.max_password_length) != 0) return;

   if (APP.input[0] == '\0') {
       fprintf(stderr, "Error: empty input\n");
       return;
   }
   
   if (strcmp(temp_password, APP.input) != 0) {
       fprintf(stderr, "Error: passwords don't match\n");
       return;
   }

   strcpy(APP.users[APP.user_count].name, temp_username); 
   strcpy(APP.users[APP.user_count].password, temp_password); 

   printf("----------------------------------------------------\n");
   printf("User successfully created\n");
   printf("User ID: %d\n", APP.user_count);
   printf("Username: %s\n", APP.users[APP.user_count].name);
   printf("----------------------------------------------------\n");

   APP.user_count++; 

   return;
}


void remove_whitespace(char *str) {
    int i = 0, j = 0;
    int prev_char_space = 0;

    for (;str[i] != '\0'; i++) {
        if (str[i] == ' ' || str[i] == '\t') {
            if (!prev_char_space) {
                str[j] = ' ';
                j++;
            }
            prev_char_space = 1;
        }
        else {
            str[j] = str[i];
            j++;
            prev_char_space = 0;
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

void clear_screen(void){ system("clear"); }
void print_prompt(void) { printf("# "); }
