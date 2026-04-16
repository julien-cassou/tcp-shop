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
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1 ) {
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
        if(recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &client_addr, (socklen_t *) &len) == -1) {
            fprintf(stderr, "SERVEUR : Echec de reception du message du client\n");
            exit(EXIT_FAILURE);
        }
        printf("Message reçu : \n");

        write(STDOUT_FILENO, buffer, strlen(buffer));
    }

    close(sockfd);
    return 0;
}