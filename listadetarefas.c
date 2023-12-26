#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <string.h>

struct Item {
    int hora;
    char content[100];
    struct Item *pointi;
};

struct Lista {
    char nome[21];
    int hactive;
    struct Item *pointi;
    struct Lista *pointl;
};

void *addlist(void **list, int openlist, char nome[20], int hactive, int hd, int hu, int md, int mu, int type);

int contlist(void *list, int type);

void *editlist(void *list, int openlist, int opition, char text[30], int hactive, int hd, int hu, int md, int mu, int type);

void *dellist(void **list, int openlist, int *position, int type);

void *elementlist(void *list, int position,int type);

void printlist(void *list, int openlist, int cols, int rows, int opition, int type);

void printcontent(void *list, int openlist, int cols, int rows, int opition, char text[30], int hactive, int hd, int hu, int md, int mu, int part, int type);

void printdelet(void *list, int cols, int rows, int auxposit);

void controllist(void *listas, int *openlist, int *opition, char text[30], int *mod, int *hactive, int *auxposit, int *hd, int *hu, int *md, int *mu, int *type, int *key);

void controladd(void *listas, int openlist, int *opition, char text[30], int *mod, int *hactive, int *auxposit, int *hd, int *hu, int *md, int *mu, int *part, int type, int *key);

void controledit(void *listas, int openlist, int *opition, char text[30], int *mod, int *hactive, int *auxposit, int *hd, int *hu, int *md, int *mu, int *part, int type, int *key);

void controldelet(void *listas, int openlist, int *opition, int *mod, int *auxposit, int type, int key);

int main() {
    setlocale(LC_ALL, "");
    struct Lista *listas = NULL;
    int key, type = 0, mod = 0, part = 0, opition = 0, auxposit = 0, hactive = 0, hd = 0, hu = 0, md = 0, mu = 0, openlist = 0;
    int rows, cols;
    char text[30];

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    if (has_colors()) {
        start_color();
    }

    while(key != 27) {
        getmaxyx(stdscr, rows, cols);

        if(cols > 55 && rows > 19) {
            clear();

            switch(mod) {
                case 0:
                case 1:
                    printlist(listas, openlist, cols, rows, opition, type);
                    break;
                case 2:
                case 3:
                    printcontent(listas, openlist, cols, rows, opition, text, hactive, hd, hu, md, mu, part, type);
                    break;
                case 4:
                    printdelet(listas, cols, rows, auxposit);
                    break;
            }

            // printw("\n\nkey=%d/%c type=%d mod=%d openlist=%d part=%d opition=%d auxposit=%d, hactive=%d, hd=%d, hu=%d, md=%d, mu=%d", key, key, type, mod, openlist, part, opition, auxposit, hactive, hd, hu, md, mu);

            key = getch();
            
            switch(mod) {
                case 0:
                case 1:
                    controllist(listas, &openlist, &opition, text, &mod, &hactive, &auxposit, &hd, &hu, &md, &mu, &type, &key);
                    break;
                case 2:
                    controladd(&listas, openlist, &opition, text, &mod, &hactive, &auxposit, &hd, &hu, &md, &mu, &part, type, &key);
                    break;
                case 3:
                    controledit(listas, openlist, &opition, text, &mod, &hactive, &auxposit, &hd, &hu, &md, &mu, &part, type, &key);
                    break;
                case 4:
                    controldelet(&listas, openlist, &opition, &mod, &auxposit, type, key);
                    break;
            }

        } else {
            printw("TAMANHO INCOMPATIVEL DE TERMINAL AUMENTE!!\n\nMIN: 56x20 atual(%dx%d)", cols, rows);
            getch();
            key = 27;
        }
    }

    struct Lista *lista = listas;
    struct Lista *next_lista = NULL;
    struct Item *item = NULL;
    struct Item *next_item = NULL;
    while (lista != NULL) {
        item = lista->pointi;

        while (item != NULL) {
            next_item = item->pointi;
            free(item);
            item = next_item;
        }

        next_lista = lista->pointl;
        free(lista);
        lista = next_lista;
    }
}

void *addlist(void **list, int openlist, char nome[20], int hactive, int hd, int hu, int md, int mu, int type) {
    if(type == 1) {
        struct Lista *open = elementlist(*list, openlist, 0);
        struct Item *newitem = malloc(sizeof(struct Item));

        newitem->hora = hd * 1000 + hu * 100 + md * 10 + mu;
        strcpy(newitem->content, nome);
        newitem->pointi = NULL;

        if (open->pointi == NULL) {
            open->pointi = newitem;
        } else {
            struct Item *aux = open->pointi;
            while(aux->pointi != NULL) {
                aux = aux->pointi;
            }
            aux->pointi = newitem;
        }
    
    } else if(type == 0) {
        struct Lista *newlista = malloc(sizeof(struct Lista));

        strncpy(newlista->nome, nome, sizeof(newlista->nome));
        newlista->hactive = hactive;
        newlista->pointl = NULL;
        newlista->pointi = NULL;

        if (*list == NULL) {
            *list = newlista;
        } else {
            struct Lista *aux = *list;
            while(aux->pointl != NULL) {
                aux = aux->pointl;
            }
            aux->pointl = newlista;
        }

    } else {
        return 0;
    }

    return *list;
}

int contlist(void *list, int type) {
    int cont = 1;

    if (list != NULL) {
        if(type == 1) {
            struct Item *aux = list;
            
            while(aux->pointi != NULL) {
                aux = aux->pointi;
                cont++;
            };
        } else if(type == 0) {
            struct Lista *aux = list;

            while(aux->pointl != NULL) {
                aux = aux->pointl;
                cont++;
            };
        } else {
            return -1;
        }
    } else {
        return 0;
    }

    return cont;
}

void *editlist(void *list, int openlist, int opition, char text[30], int hactive, int hd, int hu, int md, int mu, int type) {

    if(type == 1) {
        struct Lista *open = elementlist(list, openlist, type);
        struct Item *aux = open->pointi;

        for(int i = 0; i < opition; i++) {
            aux = aux->pointi;
        }

        aux->hora = hd * 1000 + hu * 100 + md * 10 + mu;
        strcpy(aux->content, text);
            
    } else if(type == 0) {
        struct Lista *aux = list;

        for(int i = 0; i < opition; i++) {
            aux = aux->pointl;
        }

        strcpy(aux->nome, text);
        aux->hactive = hactive;
    }

    return list;
}

void *dellist(void **list, int openlist, int *position, int type) {
    if(type == 0) {
        struct Lista *aux = *list;
        struct Lista *auxx = *list;

        if (*position == 0) {
            *list = aux->pointl;
            free(aux);
        } else {
            for(int i = 1; i < *position + 1; i++) {
                aux = aux->pointl;

                if(i < *position) {
                    auxx = auxx->pointl;
                }
            }

            auxx->pointl = aux->pointl;

            if(aux->pointi != NULL) {
                struct Item *auxxx = aux->pointi;
                struct Item *auxxxx = NULL;

                for (int i = 0; i < contlist(auxxx, 1); i++) {
                    auxxxx = auxxx->pointi;
                    free(auxxx);
                    auxxx = auxxxx;
                }
            }

            free(aux);                 
        }

        if((*position == contlist(*list, type)) && contlist(*list, type) != 0) {
            *position = *position - 1;
        }
    } else {
        struct Lista *open = elementlist(*list, openlist, 0);
        struct Item *aux = open->pointi;
        struct Item *auxx = open->pointi;

        if (*position == 0) {
            open->pointi = aux->pointi;
            free(aux);
        } else {
            for(int i = 0; i < *position; i++) {
                aux = aux->pointi;

                if(i < *position - 1) {
                    auxx = auxx->pointi;
                }
            }

            auxx->pointi = aux->pointi;

            free(aux);                 
        }

        if((*position == contlist(open->pointi, type)) && contlist(open->pointi, type) != 0) {
            *position = *position - 1;
        }
    }

    return list;
}

void *elementlist(void *list, int position,int type) {
    if (list != NULL) {
        if (type == 1) {
            struct Item *aux = list;

            while (aux->pointi != NULL) {
                aux = aux->pointi;
            }

            return aux;
        } else if (type == 0) {
            struct Lista *aux = list;

            for(int i = 0; i < position; i++) {
                aux = aux->pointl;
            }
            return aux;
        }
    }

    return NULL;
}

void printlist(void *list, int openlist, int cols, int rows, int opition, int type) {
    if(type == 1) {
        struct Lista *aux = elementlist(list, openlist, 0);

        printw("Esc - Voltar");

        for (int i = 0; i < (rows/2) - 10; i++) {
            printw("\n");
        }
        
        for (int i = 0; i < (cols/2) - (strlen(aux->nome) + 8)/2; i++) {
            printw(" ");
        }

        printw("LISTA - %s\n", aux->nome);

        for (int i = 0; i < (cols/2) - 18; i++) {
            printw(" ");
        }

        printw("%d/%d\n", aux->pointi != NULL ? opition+1 : opition, contlist(aux->pointi, type));
    } else {
        printw("Esc - Sair");

        for (int i = 0; i < (rows/2) - 10; i++) {
            printw("\n");
        }
        
        for (int i = 0; i < (cols/2) - 8; i++) {
            printw(" ");
        }
    
        printw("LISTA DE TAREFAS\n");

        for (int i = 0; i < (cols/2) - 18; i++) {
            printw(" ");
        }

        printw("%d/%d\n", list != NULL ? opition+1 : opition, contlist(list, type));
    }

    for (int i = 0; i < (cols/2) - 18; i++) {
        printw(" ");
    }

    printw("┌──────────────────────────────────┐\n");  

    if (list != NULL) {
        if(type == 1) {
            struct Lista *open = elementlist(list, openlist, 0);
            struct Item *aux = open->pointi;

            if(aux != NULL) {
                if(contlist(aux, type) >= 5) {

                    for(int i = 0; i < contlist(open->pointi, type); i++) {
                        
                        if (i < 5 + opition && (i >= opition || i >= contlist(list, type) - 5)) {
                            for (int j = 0; j < 3; j++) {
                                for (int i = 0; i < (cols/2) - 18; i++) {
                                    printw(" ");
                                }

                                printw("│");

                                for (int e = 0; e < 34; e++) {
                                    if(j == 0) {
                                        if(i == opition) {
                                            if (e == 0) {
                                                printw("┌");
                                            } else if (e == 34 - 1) {
                                                printw("┐");
                                            } else {
                                                printw("─");
                                            }
                                        } else {
                                            printw(" ");
                                        }
                                    } else if (j == 1) {
                                        if(open->hactive) {
                                            if(e == 26/2 - strlen(aux->content)/2 + 9) {
                                                printw("%s", aux->content);
                                            } else {

                                                if(strlen(aux->content) % 2 == 0) {
                                                    if(e < 26/2 - strlen(aux->content)/2 + 8 || e > 34/2 + strlen(aux->content)/2 + 2) {
                                                        if(e == 0 || e == 34 - 1) {
                                                            if(i == opition) {
                                                                printw("│");
                                                            } else {
                                                                printw(" ");
                                                            }
                                                        } else if(e >= 2 && e < 8) {
                                                            int hd, hu, md, mu;

                                                            hd = (aux->hora - aux->hora % 1000)/1000;
                                                            hu = ((aux->hora - (hd * 1000)) - aux->hora % 100)/100;
                                                            md = ((aux->hora - (hd * 1000) - (hu * 100)) - aux->hora % 10)/10;
                                                            mu = aux->hora - (hd * 1000) - (hu * 100) - (md * 10);

                                                            if(e == 3) {
                                                                if(open->hactive) {
                                                                    printw("%d%d:%d%d", hd, hu, md, mu);
                                                                } else {
                                                                    printw("     ");
                                                                }
                                                            }
                                                        } else {
                                                            printw(" ");
                                                        } 
                                                    }
                                                } else {
                                                    if(e < 26/2 - strlen(aux->content)/2 + 8 || e > 34/2 + strlen(aux->content)/2 + 3) {
                                                        if(e == 0 || e == 34 - 1) {
                                                            if(i == opition) {
                                                                printw("│");
                                                            } else {
                                                                printw(" ");
                                                            }
                                                        } else if(e >= 2 && e < 8) {
                                                            int hd, hu, md, mu;

                                                            hd = (aux->hora - aux->hora % 1000)/1000;
                                                            hu = ((aux->hora - (hd * 1000)) - aux->hora % 100)/100;
                                                            md = ((aux->hora - (hd * 1000) - (hu * 100)) - aux->hora % 10)/10;
                                                            mu = aux->hora - (hd * 1000) - (hu * 100) - (md * 10);

                                                            if(e == 3) {
                                                                if(open->hactive) {
                                                                    printw("%d%d:%d%d", hd, hu, md, mu);
                                                                } else {
                                                                    printw("     ");
                                                                }
                                                            }
                                                        } else {
                                                            printw(" ");
                                                        } 
                                                    }
                                                }
                                                
                                            }
                                        } else {
                                            if(e == 34/2 - strlen(aux->content)/2) {
                                                printw("%s", aux->content);
                                            } else {

                                                if(strlen(aux->content) % 2 == 0) {
                                                    if(e < 34/2 - strlen(aux->content)/2 + 1 || e > 34/2 + strlen(aux->content)/2 - 1) {
                                                        if(e == 0 || e == 34 - 1) {
                                                            if(i == opition) {
                                                                printw("│");
                                                            } else {
                                                                printw(" ");
                                                            }
                                                        } else {
                                                            printw(" ");
                                                        } 
                                                    }
                                                } else {
                                                    if(e < 34/2 - strlen(aux->content)/2 + 1 || e > 34/2 + strlen(aux->content)/2) {
                                                        if(e == 0 || e == 34 - 1) {
                                                            if(i == opition) {
                                                                printw("│");
                                                            } else {
                                                                printw(" ");
                                                            }
                                                        } else {
                                                            printw(" ");
                                                        } 
                                                    }
                                                }
                                                
                                            }
                                        }
                                    } else {
                                        if(i == opition) {
                                            if (e == 0) {
                                                printw("└");
                                            } else if (e == 34 - 1) {
                                                printw("┘");
                                            } else {
                                                printw("─");
                                            }
                                        } else {
                                            printw(" ");
                                        }
                                    }
                                }

                                printw("│\n");
                            }
                        }

                        aux = aux->pointi;
                    }

                } else {
                    for (int i = 0; i < contlist(open->pointi, type); i++) {

                        for (int j = 0; j < 3; j++) {
                            for (int i = 0; i < (cols/2) - 18; i++) {
                                printw(" ");
                            }

                            printw("│");

                            for (int e = 0; e < 34; e++) {
                                if(j == 0) {
                                    if(i == opition) {
                                        if (e == 0) {
                                            printw("┌");
                                        } else if (e == 34 - 1) {
                                            printw("┐");
                                        } else {
                                            printw("─");
                                        }
                                    } else {
                                        printw(" ");
                                    }
                                } else if (j == 1) {
                                    if(open->hactive) {
                                        if(e == 26/2 - strlen(aux->content)/2 + 9) {
                                            printw("%s", aux->content);
                                        } else {

                                            if(strlen(aux->content) % 2 == 0) {
                                                if(e < 26/2 - strlen(aux->content)/2 + 8 || e > 34/2 + strlen(aux->content)/2 + 2) {
                                                    if(e == 0 || e == 34 - 1) {
                                                        if(i == opition) {
                                                            printw("│");
                                                        } else {
                                                            printw(" ");
                                                        }
                                                    } else if(e >= 2 && e < 8) {
                                                        int hd, hu, md, mu;

                                                        hd = (aux->hora - aux->hora % 1000)/1000;
                                                        hu = ((aux->hora - (hd * 1000)) - aux->hora % 100)/100;
                                                        md = ((aux->hora - (hd * 1000) - (hu * 100)) - aux->hora % 10)/10;
                                                        mu = aux->hora - (hd * 1000) - (hu * 100) - (md * 10);

                                                        if(e == 3) {
                                                            if(open->hactive) {
                                                                printw("%d%d:%d%d", hd, hu, md, mu);
                                                            } else {
                                                                printw("     ");
                                                            }
                                                        }
                                                    } else {
                                                        printw(" ");
                                                    } 
                                                }
                                            } else {
                                                if(e < 26/2 - strlen(aux->content)/2 + 8 || e > 34/2 + strlen(aux->content)/2 + 3) {
                                                    if(e == 0 || e == 34 - 1) {
                                                        if(i == opition) {
                                                            printw("│");
                                                        } else {
                                                            printw(" ");
                                                        }
                                                    } else if(e >= 2 && e < 8) {
                                                        int hd, hu, md, mu;

                                                        hd = (aux->hora - aux->hora % 1000)/1000;
                                                        hu = ((aux->hora - (hd * 1000)) - aux->hora % 100)/100;
                                                        md = ((aux->hora - (hd * 1000) - (hu * 100)) - aux->hora % 10)/10;
                                                        mu = aux->hora - (hd * 1000) - (hu * 100) - (md * 10);

                                                        if(e == 3) {
                                                            if(open->hactive) {
                                                                printw("%d%d:%d%d", hd, hu, md, mu);
                                                            } else {
                                                                printw("     ");
                                                            }
                                                        }
                                                    } else {
                                                        printw(" ");
                                                    } 
                                                }
                                            }
                                            
                                        }
                                    } else {
                                        if(e == 34/2 - strlen(aux->content)/2) {
                                            printw("%s", aux->content);
                                        } else {

                                            if(strlen(aux->content) % 2 == 0) {
                                                if(e < 34/2 - strlen(aux->content)/2 + 1 || e > 34/2 + strlen(aux->content)/2 - 1) {
                                                    if(e == 0 || e == 34 - 1) {
                                                        if(i == opition) {
                                                            printw("│");
                                                        } else {
                                                            printw(" ");
                                                        }
                                                    } else {
                                                        printw(" ");
                                                    } 
                                                }
                                            } else {
                                                if(e < 34/2 - strlen(aux->content)/2 + 1 || e > 34/2 + strlen(aux->content)/2) {
                                                    if(e == 0 || e == 34 - 1) {
                                                        if(i == opition) {
                                                            printw("│");
                                                        } else {
                                                            printw(" ");
                                                        }
                                                    } else {
                                                        printw(" ");
                                                    } 
                                                }
                                            }
                                            
                                        }
                                    }
                                } else {
                                    if(i == opition) {
                                        if (e == 0) {
                                            printw("└");
                                        } else if (e == 34 - 1) {
                                            printw("┘");
                                        } else {
                                            printw("─");
                                        }
                                    } else {
                                        printw(" ");
                                    }
                                }
                            }

                            printw("│\n");
                        }

                        aux = aux->pointi;
                    }
                    for (int i = 0; i < 5 - contlist(open->pointi, type); i++) {
                        for (int j = 0; j < 3; j++) {
                                for (int i = 0; i < (cols/2) - 18; i++) {
                                    printw(" ");
                                }

                                printw("│");

                                for (int e = 0; e < 34; e++) {
                                    printw(" ");
                                }

                                printw("│\n");
                        }
                    }
                }
            } else {
                for (int i = 0; i < 5; i++) {
                    for (int j = 0; j < 3; j++) {
                        for (int i = 0; i < (cols/2) - 18; i++) {
                            printw(" ");
                        }

                        printw("│");

                        for (int e = 0; e < 34; e++) {
                            if(j == 0) {
                                printw(" ");
                            } else if (j == 1) {
                                if(i == 5/2) {
                                    if(e == 34/2 - strlen("Lista vazia\n")/2 + 1) {
                                        printw("%s", "Lista vazia");
                                    } else {

                                        if(e < 34/2 - strlen("Lista vazia\n")/2 + 2 || e > 34/2 + strlen("Lista vazia\n")/2 - 1) {
                                            printw(" ");
                                        }
                                        
                                    }
                                } else {
                                    printw(" ");
                                }
                            } else {
                                printw(" ");
                            }
                            
                        }

                        printw("│\n");
                    }
                }
            }

        } else if(type == 0) {
            struct Lista *aux = list;

            if(contlist(list, type) >= 5) {

                for (int i = 0; i < contlist(list, type); i++) {
                    
                    if (i < 5 + opition && (i >= opition || i >= contlist(list, type) - 5)) {
                        for (int j = 0; j < 3; j++) {
                            for (int i = 0; i < (cols/2) - 18; i++) {
                                printw(" ");
                            }

                            printw("│");

                            for (int e = 0; e < 34; e++) {
                                if(j == 0) {
                                    if(i == opition) {
                                        if (e == 0) {
                                            printw("┌");
                                        } else if (e == 34 - 1) {
                                            printw("┐");
                                        } else {
                                            printw("─");
                                        }
                                    } else {
                                        printw(" ");
                                    }
                                } else if (j == 1) {
                                    if(e == 34/2 - strlen(aux->nome)/2) {
                                        printw("%s", aux->nome);
                                    } else {

                                        if(strlen(aux->nome) % 2 == 0) {
                                            if(e < 34/2 - strlen(aux->nome)/2 + 1 || e > 34/2 + strlen(aux->nome)/2 - 1) {
                                                if(e == 0 || e == 34 - 1) {
                                                    if(i == opition) {
                                                        printw("│");
                                                    } else {
                                                        printw(" ");
                                                    }
                                                } else {
                                                    if(aux->hactive == 1 && e == 31){
                                                        printw("H");
                                                    } else {
                                                        printw(" ");
                                                    }
                                                } 
                                            }
                                        } else {
                                            if(e < 34/2 - strlen(aux->nome)/2 + 1 || e > 34/2 + strlen(aux->nome)/2) {
                                                if(e == 0 || e == 34 - 1) {
                                                    if(i == opition) {
                                                        printw("│");
                                                    } else {
                                                        printw(" ");
                                                    }
                                                } else {
                                                    if(aux->hactive == 1 && e == 31){
                                                        printw("H");
                                                    } else {
                                                        printw(" ");
                                                    }
                                                } 
                                            }
                                        }
                                        
                                    }

                                } else {
                                    if(i == opition) {
                                        if (e == 0) {
                                            printw("└");
                                        } else if (e == 34 - 1) {
                                            printw("┘");
                                        } else {
                                            printw("─");
                                        }
                                    } else {
                                        printw(" ");
                                    }
                                }
                            }

                            printw("│\n");
                        }
                    }

                    aux = aux->pointl;
                }

            } else {
                for (int i = 0; i < contlist(list, type); i++) {
                    for (int j = 0; j < 3; j++) {
                            for (int i = 0; i < (cols/2) - 18; i++) {
                                printw(" ");
                            }

                            printw("│");

                            for (int e = 0; e < 34; e++) {
                                if(j == 0) {
                                    if(i == opition) {
                                        if (e == 0) {
                                            printw("┌");
                                        } else if (e == 34 - 1) {
                                            printw("┐");
                                        } else {
                                            printw("─");
                                        }
                                    } else {
                                        printw(" ");
                                    }
                                } else if (j == 1) {
                                    if(e == 34/2 - strlen(aux->nome)/2) {
                                        printw("%s", aux->nome);
                                    } else {

                                        if(strlen(aux->nome) % 2 == 0) {
                                            if(e < 34/2 - strlen(aux->nome)/2 + 1 || e > 34/2 + strlen(aux->nome)/2 - 1) {
                                                if(e == 0 || e == 34 - 1) {
                                                    if(i == opition) {
                                                        printw("│");
                                                    } else {
                                                        printw(" ");
                                                    }
                                                } else {
                                                    if(aux->hactive == 1 && e == 31){
                                                        printw("H");
                                                    } else {
                                                        printw(" ");
                                                    }
                                                } 
                                            }
                                        } else {
                                            if(e < 34/2 - strlen(aux->nome)/2 + 1 || e > 34/2 + strlen(aux->nome)/2) {
                                                if(e == 0 || e == 34 - 1) {
                                                    if(i == opition) {
                                                        printw("│");
                                                    } else {
                                                        printw(" ");
                                                    }
                                                } else {
                                                    if(aux->hactive == 1 && e == 31){
                                                        printw("H");
                                                    } else {
                                                        printw(" ");
                                                    }
                                                } 
                                            }
                                        }
                                        
                                    }
                                } else {
                                    if(i == opition) {
                                        if (e == 0) {
                                            printw("└");
                                        } else if (e == 34 - 1) {
                                            printw("┘");
                                        } else {
                                            printw("─");
                                        }
                                    } else {
                                        printw(" ");
                                    }
                                }
                            }

                            printw("│\n");
                    }

                    aux = aux->pointl;
                }
                for (int i = 0; i < 5 - contlist(list, type); i++) {
                    for (int j = 0; j < 3; j++) {
                        for (int i = 0; i < (cols/2) - 18; i++) {
                            printw(" ");
                        }

                        printw("│");

                        for (int e = 0; e < 34; e++) {
                            printw(" ");
                        }

                        printw("│\n");
                    }
                }
            }

        }
    } else {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 3; j++) {
                for (int i = 0; i < (cols/2) - 18; i++) {
                    printw(" ");
                }

                printw("│");

                for (int e = 0; e < 34; e++) {
                    if(j == 0) {
                        printw(" ");
                    } else if (j == 1) {
                        if(i == 5/2) {
                            if(e == 34/2 - strlen("Lista vazia\n")/2 + 1) {
                                printw("%s", "Lista vazia");
                            } else {

                                if(e < 34/2 - strlen("Lista vazia\n")/2 + 2 || e > 34/2 + strlen("Lista vazia\n")/2 - 1) {
                                    printw(" ");
                                }
                                
                            }
                        } else {
                            printw(" ");
                        }
                    } else {
                        printw(" ");
                    }
                    
                }

                printw("│\n");
            }
        }
        
    }

    for (int i = 0; i < (cols/2) - 18; i++) {
        printw(" ");
    }

    printw("└──────────────────────────────────┘\n");

    for (int i = 0; i < (cols/2) - 25; i++) {
        printw(" ");
    }

    printw("c - Criar | e - Edit | Enter - Abrir | d - Deletar");

}

void printcontent(void *list, int openlist, int cols, int rows, int opition, char text[30], int hactive, int hd, int hu, int md, int mu, int part, int type) {
    if (type == 0) {

        for (int i = 0; i < (rows/2) - 3; i++) {
            printw("\n");
        }
        
        for (int i = 0; i < (cols/2) - 8; i++) {
            printw(" ");
        }

        printw("Criar Nova Lista:\n\n");

        for (int i = 0; i < (cols/2) - 22; i++) {
            printw(" ");
        }
        printw("┌──────────────────────────────────────────┐\n");
        for (int i = 0; i < (cols/2) - 22; i++) {
            printw(" ");
        }
        printw("│  nome:          %2d/20     ativar hora?   │\n", (int) strlen(text));
        for (int i = 0; i < (cols/2) - 22; i++) {
            printw(" ");
        }
        printw("│  ");
        
        printw("%s", text);

        for(int i = 0; i < 20 - strlen(text); i++) {
            printw("_");
        }

        if(!hactive) {
            printw("          ☐         │\n");
        } else {
            printw("          ☑         │\n");
        }

        for (int i = 0; i < (cols/2) - 22; i++) {
            printw(" ");
        }

        printw("└──────────────────────────────────────────┘\n");
        
        for (int i = 0; i < (cols/2) - 27; i++) {
            printw(" ");
        }

        printw("Esc - cancelar | Ctrl + h - Ativ. Hora | Enter - Criar");
    } else {
        struct Lista *aux = elementlist(list, openlist, 0);

        for (int i = 0; i < (rows/2) - 3; i++) {
            printw("\n");
        }
        
        for (int i = 0; i < (cols/2) - 8; i++) {
            printw(" ");
        }

        printw("Criar Novo Item:\n\n");

        for (int i = 0; i < (cols/2) - 22; i++) {
            printw(" ");
        }
        printw("┌──────────────────────────────────────────┐\n");
        for (int i = 0; i < (cols/2) - 22; i++) {
            printw(" ");
        }

        if(aux->hactive == 1) {
            printw("│  texto:         %2d/20        HORAS       │\n", (int) strlen(text));
            for (int i = 0; i < (cols/2) - 22; i++) {
                printw(" ");
            }

            if(part == 0) {
                printw("│ |");
            } else {
                printw("│  ");
            }
            
            printw("%s", text);

            for(int i = 0; i < 20 - strlen(text); i++) {
                printw("_");
            }

            if(part == 0) {
                printw("|");
            } else {
                printw(" ");
            }

            if(part == 1) {
                printw("   |%d|%d  :  %d %d    │\n", hd, hu, md, mu);
            } else if(part == 2) {
                printw("    %d|%d| :  %d %d    │\n", hd, hu, md, mu);
            } else if(part == 3) {
                printw("    %d %d  : |%d|%d    │\n", hd, hu, md, mu);
            } else if(part == 4) {
                printw("    %d %d  :  %d|%d|   │\n", hd, hu, md, mu);
            } else {
                printw("    %d %d  :  %d %d    │\n", hd, hu, md, mu);
            }

            for (int i = 0; i < (cols/2) - 22; i++) {
                printw(" ");
            }

            printw("└──────────────────────────────────────────┘\n");
            
            for (int i = 0; i < (cols/2) - 27; i++) {
                printw(" ");
            }

            printw("Esc - cancelar | ⇅ - Hora | ⇄ - selecao | Enter - Criar");
        } else {
            printw("│           texto:         %2d/20           │\n", (int) strlen(text));
            for (int i = 0; i < (cols/2) - 22; i++) {
                printw(" ");
            }
            printw("│           ");
            
            printw("%s", text);

            for(int i = 0; i < 20 - strlen(text); i++) {
                printw("_");
            }

            printw("           │\n");

            for (int i = 0; i < (cols/2) - 22; i++) {
                printw(" ");
            }

            printw("└──────────────────────────────────────────┘\n");
            
            for (int i = 0; i < (cols/2) - 15; i++) {
                printw(" ");
            }

            printw("Esc - cancelar | Enter - Criar");
        }
    }
}

void printdelet(void *list, int cols, int rows, int auxposit) {
    for (int i = 0; i < (rows/2) - 3; i++) {
        printw("\n");
    }
    
    for (int i = 0; i < (cols/2) - 8; i++) {
        printw(" ");
    }

    printw("VOCE QUER APAGAR?\n\n");

    if(auxposit == 0) {
        for (int i = 0; i < (cols/2) - 10; i++) {
            printw(" ");
        }

        printw("┌───────┐            \n");

        for (int i = 0; i < (cols/2) - 10; i++) {
            printw(" ");
        }

        printw("│ S I M │     N Ã O  \n");
        
        for (int i = 0; i < (cols/2) - 10; i++) {
            printw(" ");
        }
        
        printw("└───────┘            \n");
    } else {
        for (int i = 0; i < (cols/2) - 10; i++) {
            printw(" ");
        }

        printw("            ┌───────┐\n");

        for (int i = 0; i < (cols/2) - 10; i++) {
            printw(" ");
        }

        printw("  S I M     │ N Ã O │\n");
        
        for (int i = 0; i < (cols/2) - 10; i++) {
            printw(" ");
        }
        
        printw("            └───────┘\n");
    }
}

void controllist(void *listas, int *openlist, int *opition, char text[30], int *mod, int *hactive, int *auxposit, int *hd, int *hu, int *md, int *mu, int *type, int *key) {
    switch(*key) {
        case 10:
            if(*type == 0 && contlist(listas, *type) > 0) {
                *openlist = *opition;
                *mod = 1;
                *type = 1;
                *opition = 0;
            }
            break;
        case 27:
            if(*mod == 1) {
                *opition = *openlist;
                *mod = 0;
                *type = 0;
                *key = 0;
                *openlist = 0;
            }
            break;
        case KEY_UP:
            if(*opition > 0) {
                *opition = *opition - 1;
            }
            break;
        case KEY_DOWN:
            if(*type == 1) {
                struct Lista *open = elementlist(listas, *openlist, 0);
                struct Item *aux = open->pointi;
                
                if(*opition < contlist(aux, *type) - 1) {
                    *opition = *opition + 1;
                }
            } else {
                if(*opition < contlist(listas, *type) - 1) {
                    *opition = *opition + 1;
                }
            }
            break;
        case 'c':
            *mod = 2;
            break;
        case 'e':
            if(*type == 0) {
                struct Lista *aux = NULL;

                if(contlist(listas, *type) > 0) {
                    aux = elementlist(listas, *opition,*type);
                    strcpy(text, aux->nome);
                    *hactive = aux->hactive;
                    *auxposit = strlen(text);
                    *mod = 3;
                }
            } else {
                struct Lista *open = elementlist(listas, *openlist, 0);
                struct Item *aux = NULL;

                if(contlist(open->pointi, *type) > 0) {
                    aux = elementlist(open->pointi, *opition,*type);
                    strcpy(text, aux->content);
                    *hd = (aux->hora - aux->hora % 1000)/1000;
                    *hu = ((aux->hora - (*hd * 1000)) - aux->hora % 100)/100;
                    *md = ((aux->hora - (*hd * 1000) - (*hu * 100)) - aux->hora % 10)/10;
                    *mu = aux->hora - (*hd * 1000) - (*hu * 100) - (*md * 10);
                    *auxposit = strlen(text);
                    *mod = 3;
                }
            }

            break;
        case 'd':
            if(*type == 0) {
                if(contlist(listas, *type) > 0) {
                    *mod = 4;
                    *auxposit = 1;
                }
            } else {
                struct Lista *open = elementlist(listas, *openlist, 0);

                if(contlist(open->pointi, *type) > 0) {
                    *mod = 4;
                    *auxposit = 1;
                }
            }
            break;
    }
}

void controladd(void *listas, int openlist, int *opition, char text[30], int *mod, int *hactive, int *auxposit, int *hd, int *hu, int *md, int *mu, int *part, int type, int *key) {
    switch(*key) {
        case 10:
            listas = addlist(listas, openlist, text, *hactive, *hd, *hu, *md, *mu, type);
            if(type == 1) {
                *mod = 1;
            } else {
                *mod = 0;
            }
            *hactive = 0;
            *auxposit = 0;
            *part = 0;
            *hd = 0;
            *hu = 0;
            *md = 0;
            *mu = 0;
            for(int i = strlen(text) - 1; i >= 0; i--) {
                text[i] = '\0';
            }
            if(type == 0) {
                *opition = contlist(listas, type) != 0 ? contlist(listas, type) - 1 : contlist(listas, type);
            }else {
                struct Lista *open = elementlist(listas, openlist, 0);
                struct Item *aux = open->pointi;

                *opition = contlist(aux, type) != 0 ? contlist(aux, type) - 1 : contlist(aux, type);
            }
            break;
        case 8:
            if(*hactive) {
                *hactive = 0;
            } else {
                *hactive = 1;
            }
            break;
        case KEY_BACKSPACE:
            if(*auxposit > 0) {
                *auxposit = *auxposit - 1;
            }
            text[*auxposit] = '\0';
            break;
        case KEY_LEFT:
            if(type == 1 && *part > 0) {
                *part = *part - 1;
            }
            break;
        case KEY_RIGHT:
            if(type == 1 && *part < 4) {
                *part = *part + 1;
            }
            break;
        case KEY_UP:
            if(*part == 1) {
                if(*hd < 2) {
                    *hd = *hd + 1;
                }
            } else if(*part == 2) {
                if(*hu < (*hd == 2 ? 3 : 9)) {
                    *hu = *hu + 1;
                }
            } else if(*part == 3) {
                if(*md < 5) {
                    *md = *md + 1;
                }
            } else if(*part == 4) {
                if(*mu < 9) {
                    *mu = *mu + 1;
                }
            }
            break;
        case KEY_DOWN:
            if(*part == 1) {
                if(*hd > 0) {
                    *hd = *hd - 1;
                }
            } else if(*part == 2) {
                if(*hu > 0) {
                    *hu = *hu - 1;
                }
            } else if(*part == 3) {
                if(*md > 0) {
                    *md = *md - 1;
                }
            } else if(*part == 4) {
                if(*mu > 0) {
                    *mu = *mu - 1;
                }
            }
            break;
        case 27:
            if(type == 1) {
                *mod = 1;
            } else {
                *mod = 0;
            }
            *hactive = 0;
            *auxposit = 0;
            *part = 0;
            *hd = 0;
            *hu = 0;
            *md = 0;
            *mu = 0;
            for(int i = strlen(text) - 1; i >= 0; i--) {
                text[i] = '\0';
            }
            *key = 0;
            break;
        default:
            
            if(*part == 0) {
                if((*key >= 90 && *key <= 125) || (*key >= 32 && *key <= 67) || *key == 32) {
                    if(*auxposit < 20) {
                        text[*auxposit] = *key;
                        *auxposit = *auxposit + 1;
                    } 
                }
            }
            break;
    }
}

void controledit(void *listas, int openlist, int *opition, char text[30], int *mod, int *hactive, int *auxposit, int *hd, int *hu, int *md, int *mu, int *part, int type, int *key) {
    switch(*key) {
        case 10:
            listas = editlist(listas, openlist, *opition, text, *hactive, *hd, *hu, *md, *mu, type);
            if(type == 1) {
                *mod = 1;
            } else {
                *mod = 0;
            }
            *hactive = 0;
            *auxposit = 0;
            for(int i = strlen(text) - 1; i >= 0; i--) {
                text[i] = '\0';
            }
            break;
        case 8:
            if(*hactive) {
                *hactive = 0;
            } else {
                *hactive = 1;
            }
            break;
        case KEY_BACKSPACE:
            if(*auxposit > 0) {
                *auxposit = *auxposit - 1;
            }
            text[*auxposit] = '\0';
            break;
        case KEY_LEFT:
            if(type == 1 && *part > 0) {
                *part = *part - 1;
            }
            break;
        case KEY_RIGHT:
            if(type == 1 && *part < 4) {
                *part = *part + 1;
            }
            break;
        case KEY_UP:
            if(*part == 1) {
                if(*hd < 2) {
                    *hd = *hd + 1;
                }
            } else if(*part == 2) {
                if(*hu < (*hd == 2 ? 3 : 9)) {
                    *hu = *hu + 1;
                }
            } else if(*part == 3) {
                if(*md < 5) {
                    *md = *md + 1;
                }
            } else if(*part == 4) {
                if(*mu < 9) {
                    *mu = *mu + 1;
                }
            }
            break;
        case KEY_DOWN:
            if(*part == 1) {
                if(*hd > 0) {
                    *hd = *hd - 1;
                }
            } else if(*part == 2) {
                if(*hu > 0) {
                    *hu = *hu - 1;
                }
            } else if(*part == 3) {
                if(*md > 0) {
                    *md = *md - 1;
                }
            } else if(*part == 4) {
                if(*mu > 0) {
                    *mu = *mu - 1;
                }
            }
            break;
        case 27:
            if(type == 1) {
                *mod = 1;
            } else {
                *mod = 0;
            }
            *hactive = 0;
            *auxposit = 0;
            for(int i = strlen(text) - 1; i >= 0; i--) {
                text[i] = '\0';
            }
            *key = 0;
            break;
        default:
            
            if((*key >= 90 && *key <= 125) || (*key >= 32 && *key <= 67) || *key == 32) {
                if(*auxposit < 20) {
                    text[*auxposit] = *key;
                    *auxposit = *auxposit + 1;
                } 
            }
            break;
    }
}

void controldelet(void *listas, int openlist, int *opition, int *mod, int *auxposit, int type, int key) {
    switch(key) {
        case 10:
            if(*auxposit == 0) {
                listas = dellist(listas, openlist, opition, type);
            }

            if(type == 1) {
                *mod = 1;
            } else {
                *mod = 0;
            }
            *auxposit = 0;
            break;
        case KEY_LEFT:
            *auxposit = 0;
            break;
        case KEY_RIGHT:
            *auxposit = 1;
            break;
    }
}
