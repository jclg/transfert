/**
 * @file   transfert.c
 * @author jc <jean-charles.le-goff@epitech.eu>
 * @date   Fri Jun  4 11:32:36 2010
 * 
 * @brief  
 * 
 * 
 */

#include "transfert.h"

int	main(int argc, char **argv)
{
  int	port;

  if (argc > 2 && strcmp(argv[1], "receive") == 0)
    {
      if ((port = atoi(argv[2])) <= 0)
	return (fprintf(stderr, "Bad port...\n"));
      serveur_side(port);
      return (1);
    }
  else if (argc > 4 && strcmp(argv[1], "send") == 0)
    {
      if ((port = atoi(argv[3])) <= 0)
	return (fprintf(stderr, "Bad port...\n"));
      client_side(argv[2], port, argv[4]);
      return (2);
    }
  printf(USAGE);
  return (0);
}
