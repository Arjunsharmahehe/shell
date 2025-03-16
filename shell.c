#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define PROMPT "$ "
#define MAX_ARGS 1024 
#define TOKEN_SEP " \n"
#define PATH_MAX 4096

char CWD[PATH_MAX];

int parse_input(char *input, char **args){
    char *token = strtok(input, TOKEN_SEP);
    int i = 0;
    while (token != NULL && i < (MAX_ARGS - 1)) {
        args[i++] = token;
        token = strtok(NULL, TOKEN_SEP);
    }
    args[i] = NULL;
    return i;
}

int eval_commands(char *cmd, char **cmd_args){
    // printf("Executing command: %s\n", cmd);

    int status;
    pid_t pid;
    pid = fork();
    if(pid < 0){
        printf("Could not execute :/ \n");
        return -1;
    }

    if(pid == 0) execvp(cmd, cmd_args);
    else{
        if(waitpid(pid, &status, 0) != pid){
            fprintf(stderr, "Could not wait for the child \n");
            return -1;
        }
    }
    return status;
}

typedef enum Builtin {
    CD,
    PWD,
    INVALID
  } Builtin;

void builtin_impl_cd(char **args, size_t args_read);
void builtin_impl_pwd(char **args, size_t args_read);

void (*BUILTIN_TABLE[]) (char **args, size_t args_read) = {
    [CD] = builtin_impl_cd,
    [PWD] = builtin_impl_pwd,
  };

Builtin builtin_code(char *cmd) {
    if (!strncmp(cmd, "cd", 2)) {
      return CD;
    } else if (!strncmp(cmd, "pwd", 3)) {
      return PWD;
    } else {
      return INVALID;
    }
}

int is_builtin(char *cmd) {
    return builtin_code(cmd) != INVALID;
}

void execute_builtin(char *cmd, char **args, size_t args_read) {
    BUILTIN_TABLE[builtin_code(cmd)](args, args_read);
}


void refresh_cwd(void) {
    if (getcwd(CWD, sizeof(CWD)) == NULL) {
      fprintf(stderr, "Error: Could not read working dir");
      exit(1);
    }
}

void builtin_impl_cd(char **args, size_t args_read) {
    char *new_dir = *args;
    if (chdir(new_dir) != 0) {
      fprintf(stderr, "Error: Could not change directory");
      exit(1);
    }
    refresh_cwd();
}
  
void builtin_impl_pwd(char **args, size_t args_read) {
    fprintf(stdout, "%s\n", CWD);
}

int main(void){

    refresh_cwd();

    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    while(1){
        printf(PROMPT);
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            break;
        }
        int args_read = parse_input(input, args);
        
        // if(args[0] == NULL) break;
        // for(int i = 0; i < MAX_ARGS; i++){
        //     if(args[i] == NULL) break;
        //     printf("args[%d] = %s\n", i, args[i]);
        // }
        
        char *cmd = args[0];
        char **cmd_args = args;

        if(is_builtin(cmd)){
            execute_builtin(cmd, (cmd_args+1), args_read-1);
        } else{
            eval_commands(cmd, cmd_args);
        }
    }

    return 0;
}