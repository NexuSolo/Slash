#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>


#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096
#define MAX_PATH 4096
#define MAX_PRINTED_PATH 24

#include "commande.h"

static char *line = (char *)NULL;
int val_retour = 0;

int nb_mots(char *str) {
    int i = 0;
    int nb = 0;
    bool space = true;
    if(str[0] == ' ') {
        space = false;
    }
    while (str[i] != '\0') {
        if (str[i] == ' ') {
            space = true;
        } else {
            if (space) {
                nb++;
            }
            space = false;
        }
        i++;
    }
    return nb;
}

commande * getCommand(char * buffer) {
    commande * cmd = init_commande(nb_mots(buffer));
    if(!cmd) {
        return NULL;
    }
    int i = 0;
    while(buffer[i] != ' ' && buffer[i] != '\0') {
        cmd->cmd[i] = buffer[i];
        i++;
    }
    cmd->cmd[i++] = '\0';
    int j = 0;
    int k = 0;
    while(buffer[i] != '\0') {
        while(buffer[i] != ' ' && buffer[i] != '\0') {
            cmd->args[j][k++] = buffer[i++];
        }
        if(k != 0) {
            cmd->args[j++][k] = '\0';
            k = 0;
        }
        i++;
    }
    return cmd;
}

char * promptFormat() {
    char * res = malloc(MAX_ARGS_STRLEN);
    char * position = "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzaaaz";
    int taille = 0;

    //Ajoute la valeur de retour
    if (val_retour == 0) {
        strcat(res, "\033[32m[0]\033[36m");
        taille += 3;
    } else {
        strcat(res,"\033[91m[1]\033[36m");
        taille += 3;
    }
    int taille_restant = 30 - taille - 2;
    int taille_position = strlen(position);

    //Ajoute les ... si la taille de la position est trop grande
    if(taille_position > 25) {
        taille_restant -= 3;
        strcat(res, "...");
    }
    if(taille_position < taille_restant) {
        taille_restant = taille_position;
    }

    //Ajoute le chemin
    char * tmp = position + taille_position - taille_restant;
    strcat(res, tmp);

    //Ajoute le $ en blanc
    strcat(res, "\033[00m$");
    return res;
}

int main(int argc, char ** argv) {
    if(argc > 1) {
        printf("\033[91mTrop d'arguments \033[00m\n");
        return 1;
    }
    fprintf(stderr, "%s", promptFormat());
    rl_outstream = stderr;
    while(1) {
        line = readline ("");
        add_history (line);
        commande * cmd = getCommand(line);
        if (strcmp(cmd->cmd, "exit") == 0) {
            if(cmd->argc > 2) {
                printf("\033[91mTrop d'arguments \033[00m\n");
                val_retour = 1;
            }
            else if (cmd->argc == 1) {
                free_commande(cmd);
                break;
            }
            else {
                val_retour = atoi(cmd->args[0]);
                free_commande(cmd);
                break;
            }
        }        
        if(strcmp(cmd->cmd, "pwd") == 0) {
            if(execve("/bin/pwd", cmd -> args, NULL) == -1) {
                perror("pwd");
            }else{
                val_retour = 0;
                continue;
            }
        // }else if (strcmp(cmd->cmd, "cd") == 0) {
        //     if (execve("/bin/cd", cmd -> args, NULL) == -1) {
        //         perror("cd");
        //     }else{
        //         val_retour = 0;
        //         continue;
        //     }
        } else {
            val_retour = 1;
            printf("Commande inconnue\n");
        }
        printf(stderr,"%s", promptFormat());
        free_commande(cmd);
    }
    free(line);
    return val_retour;
}