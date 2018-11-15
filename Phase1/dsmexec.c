#include "common_impl.h"

/* variables globales */
#define SIZE 64

/* un tableau gerant les infos d'identification */
/* des processus dsm */
dsm_proc_t *proc_array = NULL;

/* le nombre de processus effectivement crees */
volatile int num_procs_creat = 0;

void usage(void)
{
  fprintf(stdout,"Usage : dsmexec machine_file executable arg1 arg2 ...\n");
  fflush(stdout);
  exit(EXIT_FAILURE);
}

void sigchld_handler(int sig)
{
  /* on traite les fils qui se terminent */
  /* pour eviter les zombies */
}


int main(int argc, char *argv[])
{
  if (argc < 3){
    usage();
  } else {
    pid_t pid;
    int num_procs = 0;
    int i;

    /* Mise en place d'un traitant pour recuperer les fils zombies*/
    /* XXX.sa_handler = sigchld_handler; */

    /* lecture du fichier de machines */
    FILE *fd=fopen("machine_file","r");

    /* 1- on recupere le nombre de processus a lancer */
    char line[SIZE];
    while(1){
      if(NULL == fgets(line,SIZE,fd)) break;
      num_procs++;
      memset(line,'\0',SIZE);
    }
    /* 2- on recupere les noms des machines : le nom de */
    /* la machine est un des elements d'identification 64/128 char c'est ok */
    rewind(fd);
    char machines[num_procs][SIZE];
    for(i=0;i<num_procs;i++){
      fgets(machines[i],SIZE,fd);
      machines[i][strlen(machines[i])-1] = '\0';
    }
    for(i=0;i<num_procs;i++){
      printf("machine : %s\n",machines[i]);
    }
    int* port = 0;
    /* creation de la socket d'ecoute */


    fd = creer_socket(0, port);

    /* + ecoute effective */
    do_listen(fd, num_procs);

    /* creation des fils */
    for(i = 0; i < num_procs ; i++) {

      /* creation du tube pour rediriger stdout */
      int pipefd_out[2];
      pipe(pipefd_out);

      /* creation du tube pour rediriger stderr */
      int pipefd_err[2];
      pipe(pipefd_err);

      pid = fork();
      if(pid == -1) ERROR_EXIT("fork");

      if (pid == 0) { /* fils */

      	/* redirection stdout */
      	close(pipefd_out[0]);
      	close(STDOUT_FILENO);
      	dup(pipefd_out[1]);
      	close(pipefd_out[1]);

      	/* redirection stderr */
        close(pipefd_err[0]);
      	close(STDERR_FILENO);
      	dup(pipefd_err[1]);
      	close(pipefd_err[1]);

      	/* Creation du tableau d'arguments pour le ssh */
      	char *newargv[5];
      	newargv[0] = "ssh";
      	newargv[1] = machines[i];
      	newargv[2] = "ls";
      	newargv[3] = "-a";
      	newargv[4] = NULL;
      	/* jump to new prog : */
      	execvp("ssh",newargv);

      } else  if(pid > 0) { /* pere */
      	/* fermeture des extremites des tubes non utiles */
        close(pipefd_out[1]);
        close(pipefd_err[1]);
      	num_procs_creat++;
      }
    }
    struct sockaddr_in* dist_addr[num_procs];

    for(i = 0; i < num_procs ; i++){

      if (poll(fds, NB, -1) == -1)
      {
        perror("poll");
        exit(EXIT_FAILURE);
      }
      /* on accepte les connexions des processus dsm */
      memset(dist_addr[i], 0, sizeof(struct sockaddr_in));
      do_accept(fd, dist_addr[i]);

      /*  On recupere le nom de la machine distante */
      /* 1- d'abord la taille de la chaine */

      /* 2- puis la chaine elle-meme */

      /* On recupere le pid du processus distant  */

      /* On recupere le numero de port de la socket */
      /* d'ecoute des processus distants */
    }

    /* envoi du nombre de processus aux processus dsm*/

    /* envoi des rangs aux processus dsm */

    /* envoi des infos de connexion aux processus */

    /* gestion des E/S : on recupere les caracteres */
    /* sur les tubes de redirection de stdout/stderr */
    /* while(1)
       {
       je recupere les infos sur les tubes de redirection
       jusqu'à ce qu'ils soient inactifs (ie fermes par les
       processus dsm ecrivains de l'autre cote ...)

       };
    */

    /* on attend les processus fils */

    /* on ferme les descripteurs proprement */

    /* on ferme la socket d'ecoute */
  }
  exit(EXIT_SUCCESS);
}
