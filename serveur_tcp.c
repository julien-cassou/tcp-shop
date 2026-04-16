/*
 * Code du serveur
 */

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#include <string.h>

/* Port local du serveur */
#define PORT 9600
#define BUFFER_SIZE 1024

typedef struct {
    char nom[20];
    int prix;
    int stock;
} Article;

void generer_catalogue(Article inventaire[], int nb_articles, char *dest) {
    strcpy(dest, "=== BIENVENUE SUR LE BAZAR TCP ===\n");
    for(int i = 0; i < nb_articles; i++) {
        char ligne[100];
        sprintf(ligne, "[%d] %s - %d€ (Stock: %d)\n", i, inventaire[i].nom, inventaire[i].prix, inventaire[i].stock);
        strcat(dest, ligne);
    }
    strcat(dest, "Que voulez-vous acheter ? (Tapez BUY + le chiffre correspondant)\n ");
}

int main(int argc, char *argv[]) {
    /*
     * Variables du serveur
     *
     * Déclarer ici les variables suivantes :
     *  - sockfd le descripteur de socket
     *  - structure d'adresse locale du serveur
     *  - structure d'adresse du client
     *  - taille de l'adresse du client
     */

    int sockfd;
    struct sockaddr_in serv_addr;
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);

    Article inventaire[] = {
        {"Figurine", 12, 5},
        {"Livre", 5, 10},
        {"Lampe", 20, 20}
    };

    /*
     * Code du serveur
     *
     * - Ouvrir le socket du serveur
     * - Remplir la structure d'adresse locale du serveur :
     *   - la famille d'adresse
     *   - l'adresse IP
     *   - le port
     * - Spécifier l'adresse locale du socket du serveur
     */

    // Création du socket
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1 ) {
        fprintf(stderr, "SERVEUR : Echec de l'initialisation du socket\n");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;  // On accepte la connexion avec toutes les adresses
    serv_addr.sin_port = htons(PORT);

    int servAddrLength = sizeof(serv_addr);

    if (bind(sockfd, (const struct sockaddr*) &serv_addr, servAddrLength) == -1) {
        fprintf(stderr, "SERVEUR : Echec de liaison avec le socket\n");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 1) == -1) {
        fprintf(stderr, "SERVEUR : Echec de demarrage de l'ecoute des connexions entrantes\n");
        exit(EXIT_FAILURE);
    }

    printf("Serveur en attente sur le Port : %d...\n", PORT);

    int clientAddrLength = sizeof(client_addr);

    /*
     * Boucle pour accepter les connexions
     */
    while(1) {
        int newSocket = accept(sockfd, (struct sockaddr*) &client_addr, (socklen_t *) &clientAddrLength);
        if(newSocket == -1) {
            fprintf(stderr, "SERVEUR : Erreur lors de l'établissement de la connexion\n");
            exit(EXIT_FAILURE);
        }

        printf("Client connecté ! Envoi du catalogue...\n");
        char catalogue[BUFFER_SIZE] = {0};
        generer_catalogue(inventaire, 3, catalogue);
        write(newSocket, catalogue, strlen(catalogue));

        int totalAPayer = 0;
        /*
         * Boucle générale du serveur (infinie)
         */
        while (1) {
            /*
             * Code de l'intérieur de la boucle
             */
            printf("En attente de réception d'un message...\n");
            // Réception du message
            char buffer[BUFFER_SIZE] = {0};
            ssize_t n = read(newSocket, buffer, BUFFER_SIZE - 1);
            
            if (n <= 0) {
                printf("SERVEUR : Client déconnecté ou erreur\n");
                close(newSocket);
                break;
            }

            if (strncmp(buffer, "STOCK", 5) == 0) {
                char catalogue[BUFFER_SIZE] = {0};
                generer_catalogue(inventaire, 3, catalogue);
                write(newSocket, catalogue, strlen(catalogue));
            }
            else if(strncmp(buffer, "OUT", 3) == 0) {
                char finCommande[100];
                sprintf(finCommande, "Vous avez payé %d€, merci pour vos achats !\n", totalAPayer);
                write(newSocket, finCommande, strlen(finCommande));
                close(newSocket);
            }
            else if (strncmp(buffer, "BUY ", 4) == 0) {
                int index = buffer[4] - '0';
                if(index >= 0 && index < 3) {
                    if(inventaire[index].stock > 0) {
                        inventaire[index].stock--;
                        totalAPayer += inventaire[index].prix;
                        char confirmation[100];
                        sprintf(confirmation, "Ajout effectué, %s : (Prix : %d), Stock restant : %d\n", 
                            inventaire[index].nom, inventaire[index].prix, inventaire[index].stock);
                        write(newSocket, confirmation, strlen(confirmation));
                    } else {
                        write(newSocket, "Erreur : Article en rupture de stock.\n", 39);
                    }
                } else {
                    write(newSocket, "Erreur : Code article inconnu.\n", 31);
                }
            }
            else {
                char erreurMessage[100] = "Veuillez tapez STOCK, OUT, ou bien acheter un article valide !";
                write(newSocket, erreurMessage, strlen(erreurMessage));
            }
            printf("CLIENT : > %s\n", buffer);
        }
    }

    close(sockfd);
    return 0;
}