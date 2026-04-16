# Projet L2 Réseaux G3 | Julien Cassou 
**Contact :** julien.cassou@universite-paris-saclay.fr

## Projet UDP

### Compilation UDP
```bash
make udp
```

### Exécution UDP (2 Terminals)

**Terminal 1 - Serveur UDP**
```bash
./server_udp
```

**Terminal 2 - Client UDP**
```bash
./client_udp 127.0.0.1
```


---


## Projet TCP

Voici le Bazar TCP. Vous pouvez acheter toutes sortes de choses 
(notre stock est limité pour l'instant).

### Compilation TCP
```bash
make tcp
```

### Exécution TCP (2 Terminals)

**Terminal 1 - Serveur TCP**
```bash
./server_tcp
```

**Terminal 2 - Client TCP**
```bash
./client_tcp 127.0.0.1
```

### 📋 Commandes disponibles (Côté Client)
Le client doit saisir les commandes suivantes dans la console :

* STOCK : Affiche la liste des articles, leurs prix et les quantités restantes en magasin.

* BUY [n] : Ajoute l'article d'index n à votre panier (ex: BUY 0). Le serveur décrémente le stock et met à jour votre total.

* OUT : Termine la session, affiche le montant total de la facture et ferme la connexion.

## Ce que j'ai appris

* Création d'un protocole : Pour le "Bazar", j'ai dû créer ma propre logique de communication : analyser les messages du client avec strncmp (pour BUY, STOCK, etc.) et formater les réponses avec sprintf.

* Gestion de stock : J'ai vu comment faire pour que le serveur garde en mémoire des variables (le stock global et prix total) pendant toute la durée d'une session.

* Fiabilité : Mise en place d'une gestion d'erreurs systématique sur chaque appel de fonction pour assurer la stabilité du serveur même en cas de déconnexion brutale du client.

* Manipulation des Sockets : J'ai appris à configurer les structures sockaddr_in, à utiliser htons pour le formatage des ports

* La gestion des sockets : Compréhension des étapes cruciales (socket, bind, listen, accept pour TCP / sendto, recvfrom pour UDP).