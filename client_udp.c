/*
 * Code du client
 */

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define SIZE 100
#define PORT 9600

int main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(stderr, "Usage : %s <hostname>\n", argv[0]);
    }
    /*
     * Variables du client
     *
     * Déclarer ici les variables suivantes :
     *  - sockfd le descripteur de socket
     *  - structure d'adresse du serveur
     *  - pointeur vers la structure descriptive de machine (hostent)
     *  - zone de mémoire destinée à accueillir la chaîne entrée au clavier
     *  - taille de la chaîne à envoyer
     */
    int sockfd;
    struct sockaddr_in serv_addr;
    struct sockaddr_in client_addr;
    struct hostent *serveur;    
    char BUFFER[SIZE];

    /*
     * Code du client
     *
     * - Ouvrir le socket du client
     * - Récupérer l'adresse IP du serveur à partir de son nom donné en ligne de
     * commande
     * - Remplir la structure d'adresse du serveur
     * - Lire une ligne de l'entrée standard
     * - Envoyer la chaîne lue au serveur
     */

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1 ) {
        fprintf(stderr, "CLIENT : Echec de l'initialisation du socket\n");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    serveur = gethostbyname(argv[1]);
    if(serveur == NULL) {
        fprintf(stderr, "CLIENT : Erreur, serveur introuvable\n");
        exit(EXIT_FAILURE);
    }
    memcpy(&serv_addr.sin_addr, serveur->h_addr_list[0], serveur->h_length);
    
    int servAddrLength = sizeof(serv_addr);

    printf("Entrez un message: \n");
    
    if(read(STDIN_FILENO, BUFFER, SIZE) == -1) {
        fprintf(stderr, "CLIENT : Echec, le message n'a pas pu être lu\n");
        exit(EXIT_FAILURE);
    }

    if(sendto(sockfd, BUFFER, strlen(BUFFER), 0, (const struct sockaddr *) &serv_addr, (socklen_t) servAddrLength) == -1) {
        fprintf(stderr, "CLIENT : Echec, le message n'a pas pu être envoyer au serveur\n");
        exit(EXIT_FAILURE);
    }

    close(sockfd);
    return 0;
}