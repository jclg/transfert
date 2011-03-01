/**
 * @file   network.c
 * @author  <jc@candice>
 * @date   Sat Jun 19 16:50:31 2010
 * 
 * @brief  
 * 
 * 
 */


#include "transfert.h"


char                    *network_try_resolve_hostname(char *host)
{
  struct hostent        *h;
  struct in_addr        addr;

  if ((h = gethostbyname(host)))
    {
      bcopy(h->h_addr, &addr, sizeof(addr));
      return (inet_ntoa(addr));
    }
  return (host);
}



/** 
 * create a socket for a client
 * 
 * @param host 
 * @param port 
 * 
 * @return the socket or a negative value on error
 */
int			network_connect(char *host, int port)
{
  int			s;
  struct sockaddr_in	sin;

  if ((s = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
      fprintf(stderr, "Failed to create the socket...\n");
      return (-1);
    }
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = inet_addr(network_try_resolve_hostname(host));
  if (connect(s, (struct sockaddr *)&sin, sizeof (struct sockaddr_in)) == -1)
    {
      fprintf(stderr, "Can't connect to server %s:%d\n", host, port);
      return (-2);
    }
  return (s);
}


/** 
 * create a listen socket for a server
 * 
 * @param port 
 * 
 * @return the socket or a negative value on error
 */
int			network_listen(int port)
{
  int			s;
  struct sockaddr_in    sin;

  if ((s = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
      fprintf(stderr, "Failed to create the socket...\n");
      return (-1);
    }
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = INADDR_ANY;
  if (bind(s, (struct sockaddr*)&sin, sizeof(sin)) == -1)
    {
      fprintf(stderr, "Adress is already in use or not available...\n");
      return (-2);
    }
  if (listen(s, 42) == -1)
    {
      fprintf(stderr, "Listen failed...\n");
      return (-3);
    }
  return (s);
}
