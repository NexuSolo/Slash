#ifndef COMMANDE_H
#define COMMANDE_H

extern int nbc;

typedef struct commande {
    char *cmd;
    char **args;
    int argc;
    int option;
} commande;


typedef struct commandeListe {
    commande **cList;
    int nbCmd;
} commandeListe;

void free_commande(commande *cmd);
void printCom(commande *cmd);
void free_commande_list(commandeListe * cmdList);
commande * getCommand(char * buffer);
commandeListe * getCommandList(char * buffer);

#endif