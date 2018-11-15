
int do_socket(int domain, int type, int protocol)
{
  int sock;
  sock = socket(domain, type, protocol);
  if (sock == -1)
  {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  return sock;
}
