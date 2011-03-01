/**
 * @file   transfert.c
 * @author  <jc@candice>
 * @date   Fri Jun  4 11:32:36 2010
 * 
 * @brief  
 * 
 * 
 */

#include "transfert.h"

int	main(int argc, char **argv)
{
  if (argc > 2)
    {
      int	port;
      if (strcmp(argv[1], "receive") == 0)
	{
	  if ((port = atoi(argv[2])) <= 0)
	    return (fprintf(stderr, "Bad port...\n"));
	  serveur_side(port);
	}
      if (strcmp(argv[1], "send") == 0)
	{
	  if (argc > 4)
	    {
	      if ((port = atoi(argv[3])) <= 0)
		return (fprintf(stderr, "Bad port...\n"));
	      client_side(argv[2], port, argv[4]);
	    }
	  else
	    printf(USAGE);
	}
      else
	printf(USAGE);
    }
  else
    printf(USAGE);
  return (0);
}
