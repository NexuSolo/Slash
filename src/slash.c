#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096
#define MAX_PATH 4096
#define MAX_PRINTED_PATH 24

#include "commande.h"
// #include "pwd.c"

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
    int mot = nb_mots(buffer);
    if(mot == 0) {
        return NULL;
    }
    commande * cmd = malloc(sizeof(commande));
    if(!cmd) {
        return NULL;
    }
    cmd->argc = mot;
    cmd->args = malloc(sizeof(char *) * mot);
    if(!cmd->args) {
        free(cmd);
        return NULL;
    }
    int debut = 0;
    int fin = 0;
    int i = 0;
    while(buffer[fin] != '\0') {
        if(buffer[fin] == ' ') {
            cmd->args[i] = malloc(fin - debut + 1);
            if(!cmd->args[i]) {
                return NULL;
            }
            strncpy(cmd->args[i], buffer + debut, fin - debut);
            cmd->args[i][fin - debut] = '\0';
            debut = fin + 1;
            i++;
        }
        fin++;
    }
    cmd->args[i] = malloc(fin - debut + 1);
    if(!cmd->args[i]) {
        return NULL;
    }
    strncpy(cmd->args[i], buffer + debut, fin - debut);
    cmd->args[i][fin - debut] = '\0';
    cmd->cmd = cmd->args[0];
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
    strcat(res, "\033[00m$ ");
    return res;
}

int main(int argc, char ** argv) {
    if(argc > 1) {
        printf("\033[91mTrop d'arguments \033[00m\n");
        return 1;
    }
    rl_outstream = stderr;
    while(1) {
        line = readline (promptFormat());
        add_history (line);
        commande * cmd = getCommand(line);
        if (strcmp(cmd->cmd, "exit") == 0) {
            if(cmd->argc > 2) {
                printf("\033[91mTrop d'arguments \033[00m\n");
                val_retour = 1;
                free_commande(cmd);
                continue;
            }
            else if(cmd->argc != 1) {
                val_retour = atoi(cmd->args[0]);
            }
            printf("%s\n",cmd->args[0]);
            free_commande(cmd);
            break;
        }
        else if(strcmp(cmd->cmd, "pwd") == 0) {
            // pwd(cmd->argc, cmd->args);
            // }else if (strcmp(cmd->cmd, "cd") == 0) {
            //     if (execve("/bin/cd", cmd -> args, NULL) == -1) {
            //         perror("cd");
            //     }else{
            //         val_retour = 0;
            //         continue;
            //     }
            free_commande(cmd);
        }else{
            printf("Commande inexistante\n");
            val_retour = 1;
        }
    }
    free(line);
    return val_retour;
}