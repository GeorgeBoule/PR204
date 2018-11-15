#include "common_impl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>

#define MX_SIZE 256

void init_addr(int* port, struct sockaddr_in* serv_addr)
{

  //clean the serv_add structure
  memset(serv_addr, 0, sizeof(struct sockaddr_in));
  //cast the port from a string to an int
  portno   = atoi(port);

  //internet family protocol
  serv_addr->sin_family = AF_INET;

  //we bind to any ip form the host
  serv_addr->sin_addr.s_addr = INADDR_ANY;

  //we bind on the tcp port specified
  serv_addr->sin_port = htons(*port);
}

int creer_socket(int prop, int *port_num)
{
  struct sockaddr_in* addr;
  init_addr(port_num, addr);
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1)
  {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  if (bind(fd, (struct sockaddr *) addr, sizeof(*addr)) == -1)
  {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  /* fonction de creation et d'attachement */
  /* d'une nouvelle socket */
  /* renvoie le numero de descripteur */
  /* et modifie le parametre port_num */

  return fd;
}


int do_listen(int sock, int nb)
{
  if(listen(sock, nb) == -1)
  {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  return 0;
}

int do_accept(int sock, struct sockaddr_in* serv_addr)
{
  int addrlen = sizeof(struct sockaddr_in); // this settle the "bad address" problem
  int sock_client = accept(sock, (struct sockaddr *)serv_addr, &addrlen);
  if (sock_client == -1)
  {
    perror("accept");
    exit(EXIT_FAILURE);
  }
  return sock_client;
}

int do_read(int sock_client, char* buf)
{
  int nb = read(sock_client, buf, MX_SIZE); // \0
  if ( nb == -1 )
  {
    perror("recv");
    exit(EXIT_FAILURE);
  }
  return 0;
}





/* Vous pouvez ecrire ici toutes les fonctions */
/* qui pourraient etre utilisees par le lanceur */
/* et le processus intermediaire. N'oubliez pas */
/* de declarer le prototype de ces nouvelles */
/* fonctions dans common_impl.h */
